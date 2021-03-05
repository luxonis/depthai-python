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

# MobilenetSSD label texts
labelMap = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
             "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

syncNN = True

# Get argument first
mobilenet_path = str((Path(__file__).parent / Path('models/mobilenet.blob')).resolve().absolute())
if len(sys.argv) > 1:
    mobilenet_path = sys.argv[1]

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
colorCam = pipeline.createColorCamera()
colorCam.setPreviewSize(300, 300)
colorCam.setInterleaved(False)
# colorCam.setPreviewKeepAspectRatio(False)

# Define a neural network that will make predictions based on the source frames
detectionNetwork = pipeline.createMobileNetDetectionNetworkDepth()
detectionNetwork.setConfidenceThreshold(0.5)
detectionNetwork.setBlobPath(mobilenet_path)
detectionNetwork.input.setBlocking(False)
detectionNetwork.setBoundingBoxScaleFactor(0.7)
detectionNetwork.setDepthLowerThresholdLimit(100)
detectionNetwork.setDepthUpperThresholdLimit(5000)

colorCam.preview.link(detectionNetwork.input)

# Create outputs
xoutRgb = pipeline.createXLinkOut()
xoutRgb.setStreamName("rgb")
if(syncNN):
    detectionNetwork.passthrough.link(xoutRgb.input)
else:
    colorCam.preview.link(xoutRgb.input)

depthRoiMap = pipeline.createXLinkOut()
depthRoiMap.setStreamName("depthRoiMap")

xoutDepth = pipeline.createXLinkOut()
xoutDepth.setStreamName("depth")

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("detections")
detectionNetwork.out.link(xout_nn.input)
detectionNetwork.passthroughRoi.link(depthRoiMap.input)

monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
stereo = pipeline.createStereoDepth()
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
stereo.setOutputDepth(True)
stereo.setConfidenceThreshold(255)

monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

stereo.depth.link(detectionNetwork.inputDepth)
stereo.depth.link(xoutDepth.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the rgb frames and nn data from the outputs defined above
    previewQueue = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    detectionNNQueue = device.getOutputQueue(name="detections", maxSize=4, blocking=False)
    depthRoiMap = device.getOutputQueue(name="depthRoiMap", maxSize=4, blocking=False)
    depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    frame = None
    detections = []

    start_time = time.time()
    counter = 0
    fps = 0
    color = (255, 255, 255)

    while True:
        in_rgb = previewQueue.get()
        in_nn = detectionNNQueue.get()
        depth = depthQueue.get()

        counter+=1
        current_time = time.time()
        if (current_time - start_time) > 1 :
            fps = counter / (current_time - start_time)
            counter = 0
            start_time = current_time
        
        frame = in_rgb.getCvFrame()

        depthFrame = depth.getFrame()

        depthFrameColor = cv2.normalize(depthFrame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8UC1)
        depthFrameColor = cv2.equalizeHist(depthFrameColor)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)
        detections = in_nn.detections
        if len(detections) != 0:
            passthroughRoi = depthRoiMap.get()
            roiDatas = passthroughRoi.getConfigData()

            for roiData in roiDatas:
                roi = roiData.roi
                xmin = int(roi.xmin * depth.getWidth())
                ymin = int(roi.ymin * depth.getHeight())
                xmax = int(roi.xmax * depth.getWidth())
                ymax = int(roi.ymax * depth.getHeight())

                cv2.rectangle(depthFrameColor, (xmin, ymin), (xmax, ymax), color, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX)


        # if the frame is available, draw bounding boxes on it and show the frame
        height = frame.shape[0]
        width  = frame.shape[1]
        for detection in detections:
            #denormalize bounging box
            x1 = int(detection.xmin * width)
            x2 = int(detection.xmax * width)
            y1 = int(detection.ymin * height)
            y2 = int(detection.ymax * height)
            try:
                label = labelMap[detection.label]
            except:
                label = detection.label
            cv2.putText(frame, str(label), (x1 + 10, y1 + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.putText(frame, "{:.2f}".format(detection.confidence*100), (x1 + 10, y1 + 35), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.putText(frame, f"X: {int(detection.xdepth)}", (x1 + 10, y1 + 50), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.putText(frame, f"Y: {int(detection.ydepth)}", (x1 + 10, y1 + 65), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.putText(frame, f"Z: {int(detection.zdepth)}", (x1 + 10, y1 + 80), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)

            cv2.rectangle(frame, (x1, y1), (x2, y2), color, cv2.FONT_HERSHEY_SIMPLEX)

        cv2.putText(frame, "NN fps: {:.2f}".format(fps), (2, frame.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color)
        cv2.imshow("depth", depthFrameColor)
        cv2.imshow("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
