#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np

# Get argument first
nnPath = str((Path(__file__).parent / Path('models/mobilenet-ssd_openvino_2021.2_6shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    nnPath = sys.argv[1]

if not Path(nnPath).exists():
    raise FileNotFoundError("Required file/s not found, please run 'python3 install_requirements.py'")

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - mono (grayscale) cameras
left = pipeline.createMonoCamera()
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
left.setBoardSocket(dai.CameraBoardSocket.LEFT)

right = pipeline.createMonoCamera()
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create a node that will produce the depth map (using disparity output as it's easier to visualize depth this way)
stereo = pipeline.createStereoDepth()
stereo.setOutputRectified(True)  # The rectified streams are horizontally mirrored by default
stereo.setConfidenceThreshold(255)
stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout from rectification (black stripe on the edges)

left.out.link(stereo.left)
right.out.link(stereo.right)

# Create a node to convert the grayscale frame into the nn-acceptable form
manip = pipeline.createImageManip()
manip.initialConfig.setResize(300, 300)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)
stereo.rectifiedRight.link(manip.inputImage)

# Define a neural network that will make predictions based on the source frames
nn = pipeline.createMobileNetDetectionNetwork()
nn.setConfidenceThreshold(0.5)
nn.setBlobPath(nnPath)
nn.setNumInferenceThreads(2)
nn.input.setBlocking(False)
manip.out.link(nn.input)

# Create outputs
depthOut = pipeline.createXLinkOut()
depthOut.setStreamName("depth")

stereo.disparity.link(depthOut.input)

xoutRight = pipeline.createXLinkOut()
xoutRight.setStreamName("rectifiedRight")
manip.out.link(xoutRight.input)

nnOut = pipeline.createXLinkOut()
nnOut.setStreamName("nn")
nn.out.link(nnOut.input)

# MobilenetSSD label nnLabels
labelMap = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
            "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale / depth frames and nn data from the outputs defined above
    qRight = device.getOutputQueue("rectifiedRight", maxSize=4, blocking=False)
    qDepth = device.getOutputQueue("depth", maxSize=4, blocking=False)
    qDet = device.getOutputQueue("nn", maxSize=4, blocking=False)

    rightFrame = None
    depthFrame = None
    detections = []
    offsetX = (right.getResolutionWidth() - right.getResolutionHeight()) // 2
    croppedFrame = np.zeros((right.getResolutionHeight(), right.getResolutionHeight()))

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

    while True:
        # Instead of get (blocking), we use tryGet (nonblocking) which will return the available data or None otherwise
        inRight = qRight.tryGet()
        inDet = qDet.tryGet()
        inDepth = qDepth.tryGet()

        if inRight is not None:
            rightFrame = inRight.getCvFrame()

        if inDet is not None:
            detections = inDet.detections

        if inDepth is not None:
            # Frame is transformed, the color map will be applied to highlight the depth info
            depthFrame = cv2.flip(inDepth.getFrame(), 1)
            # Available color maps: https://docs.opencv.org/3.4/d3/d50/group__imgproc__colormap.html
            depthFrame = cv2.applyColorMap(depthFrame, cv2.COLORMAP_JET)

        if rightFrame is not None:
            for detection in detections:
                bbox = frameNorm(rightFrame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
                cv2.rectangle(rightFrame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
                cv2.putText(rightFrame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
                cv2.putText(rightFrame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.imshow("rectified right", rightFrame)

        if depthFrame is not None:
            for detection in detections:
                bbox = frameNorm(croppedFrame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
                bbox[::2] += offsetX
                cv2.rectangle(depthFrame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
                cv2.putText(depthFrame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
                cv2.putText(depthFrame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.imshow("depth", depthFrame)

        if cv2.waitKey(1) == ord('q'):
            break
