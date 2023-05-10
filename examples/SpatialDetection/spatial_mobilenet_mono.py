#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
import time

'''
Mobilenet SSD device side decoding demo
  The "mobilenet-ssd" model is a Single-Shot multibox Detection (SSD) network intended
  to perform object detection. This model is implemented using the Caffe* framework.
  For details about this model, check out the repository <https://github.com/chuanqi305/MobileNet-SSD>.
'''

# Get argument first
nnPath = str((Path(__file__).parent / Path('../models/mobilenet-ssd_openvino_2021.4_6shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    nnPath = sys.argv[1]

if not Path(nnPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

# MobilenetSSD label texts
labelMap = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
            "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

syncNN = True

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
spatialDetectionNetwork = pipeline.create(dai.node.MobileNetSpatialDetectionNetwork)
imageManip = pipeline.create(dai.node.ImageManip)

xoutManip = pipeline.create(dai.node.XLinkOut)
nnOut = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)

xoutManip.setStreamName("right")
nnOut.setStreamName("detections")
xoutDepth.setStreamName("depth")

# Properties
imageManip.initialConfig.setResize(300, 300)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
imageManip.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("left")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setCamera("right")

# StereoDepth
stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.setSubpixel(True)

# Define a neural network that will make predictions based on the source frames
spatialDetectionNetwork.setConfidenceThreshold(0.5)
spatialDetectionNetwork.setBlobPath(nnPath)
spatialDetectionNetwork.input.setBlocking(False)
spatialDetectionNetwork.setBoundingBoxScaleFactor(0.5)
spatialDetectionNetwork.setDepthLowerThreshold(100)
spatialDetectionNetwork.setDepthUpperThreshold(5000)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

imageManip.out.link(spatialDetectionNetwork.input)
if syncNN:
    spatialDetectionNetwork.passthrough.link(xoutManip.input)
else:
    imageManip.out.link(xoutManip.input)

spatialDetectionNetwork.out.link(nnOut.input)

stereo.rectifiedRight.link(imageManip.inputImage)
stereo.depth.link(spatialDetectionNetwork.inputDepth)
spatialDetectionNetwork.passthroughDepth.link(xoutDepth.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues will be used to get the rgb frames and nn data from the outputs defined above
    previewQueue = device.getOutputQueue(name="right", maxSize=4, blocking=False)
    detectionNNQueue = device.getOutputQueue(name="detections", maxSize=4, blocking=False)
    depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    rectifiedRight = None
    detections = []

    startTime = time.monotonic()
    counter = 0
    fps = 0
    color = (255, 255, 255)

    while True:
        inRectified = previewQueue.get()
        inDet = detectionNNQueue.get()
        inDepth = depthQueue.get()

        counter += 1
        currentTime = time.monotonic()
        if (currentTime - startTime) > 1:
            fps = counter / (currentTime - startTime)
            counter = 0
            startTime = currentTime

        rectifiedRight = inRectified.getCvFrame()

        depthFrame = inDepth.getFrame() # depthFrame values are in millimeters

        depth_downscaled = depthFrame[::4]
        min_depth = np.percentile(depth_downscaled[depth_downscaled != 0], 1)
        max_depth = np.percentile(depth_downscaled, 99)
        depthFrameColor = np.interp(depthFrame, (min_depth, max_depth), (0, 255)).astype(np.uint8)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

        detections = inDet.detections

        # If the rectifiedRight is available, draw bounding boxes on it and show the rectifiedRight
        height = rectifiedRight.shape[0]
        width = rectifiedRight.shape[1]
        for detection in detections:
            roiData = detection.boundingBoxMapping
            roi = roiData.roi
            roi = roi.denormalize(depthFrameColor.shape[1], depthFrameColor.shape[0])
            topLeft = roi.topLeft()
            bottomRight = roi.bottomRight()
            xmin = int(topLeft.x)
            ymin = int(topLeft.y)
            xmax = int(bottomRight.x)
            ymax = int(bottomRight.y)
            cv2.rectangle(depthFrameColor, (xmin, ymin), (xmax, ymax), color, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX)

            # Denormalize bounding box
            x1 = int(detection.xmin * width)
            x2 = int(detection.xmax * width)
            y1 = int(detection.ymin * height)
            y2 = int(detection.ymax * height)

            try:
                label = labelMap[detection.label]
            except:
                label = detection.label

            cv2.putText(rectifiedRight, str(label), (x1 + 10, y1 + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(rectifiedRight, "{:.2f}".format(detection.confidence*100), (x1 + 10, y1 + 35), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(rectifiedRight, f"X: {int(detection.spatialCoordinates.x)} mm", (x1 + 10, y1 + 50), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(rectifiedRight, f"Y: {int(detection.spatialCoordinates.y)} mm", (x1 + 10, y1 + 65), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(rectifiedRight, f"Z: {int(detection.spatialCoordinates.z)} mm", (x1 + 10, y1 + 80), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)

            cv2.rectangle(rectifiedRight, (x1, y1), (x2, y2), color, cv2.FONT_HERSHEY_SIMPLEX)

        cv2.putText(rectifiedRight, "NN fps: {:.2f}".format(fps), (2, rectifiedRight.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color)
        cv2.imshow("depth", depthFrameColor)
        cv2.imshow("rectified right", rectifiedRight)

        if cv2.waitKey(1) == ord('q'):
            break
