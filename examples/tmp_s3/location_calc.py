#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path

#run examples/install_requirements.py -sdai

datasetDefault = str((Path(__file__).parent / Path('../models/dataset')).resolve().absolute())
if not Path(datasetDefault).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

left = datasetDefault + '/' + '0' + '/' + 'in_left' + '.png'
right = datasetDefault + '/' + '0' + '/' + 'in_right' + '.png'

leftImg = cv2.imread(left, cv2.IMREAD_GRAYSCALE)
rightImg = cv2.imread(right, cv2.IMREAD_GRAYSCALE)

cv2.imshow("leftImg", leftImg)
cv2.imshow("rightImg", rightImg)
cv2.waitKey(1)

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.XLinkIn)
monoRight = pipeline.create(dai.node.XLinkIn)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

spatialLocationCalculator = pipeline.create(dai.node.SpatialLocationCalculator)
xoutSpatialData = pipeline.create(dai.node.XLinkOut)
xinSpatialCalcConfig = pipeline.create(dai.node.XLinkIn)

monoLeft.setStreamName("inLeft")
monoRight.setStreamName("inRight")
xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDepth.setStreamName("depth")

xoutSpatialData.setStreamName("spatialData")
xinSpatialCalcConfig.setStreamName("spatialCalcConfig")

monoLeft.setMaxDataSize(1280 * 720)
monoRight.setMaxDataSize(1280 * 720)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.depth.link(xoutDepth.input)

stereo.depth.link(spatialLocationCalculator.inputDepth)
spatialLocationCalculator.out.link(xoutSpatialData.input)
xinSpatialCalcConfig.out.link(spatialLocationCalculator.inputConfig)

# Config
topLeft = dai.Point2f(0.3, 0.4)
bottomRight = dai.Point2f(0.6, 0.7)

config = dai.SpatialLocationCalculatorConfigData()
config.depthThresholds.lowerThreshold = 100
config.depthThresholds.upperThreshold = 10000
config.roi = dai.Rect(topLeft, bottomRight)

spatialLocationCalculator.inputConfig.setWaitForMessage(False)
spatialLocationCalculator.initialConfig.addROI(config)

stereo.setInputResolution(1280, 720)


stereo.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_LEFT)
# stereo.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_RIGHT)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qInLeft = device.getInputQueue("inLeft")
    qInRight = device.getInputQueue("inRight")
    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("depth", 4, False)
    spatialCalcQueue = device.getOutputQueue(name="spatialData", maxSize=4, blocking=False)
    spatialCalcConfigInQueue = device.getInputQueue("spatialCalcConfig")

    cnt = 0

    width = 1280
    height = 720
    color = (255, 255, 255)

    while True:

        data = cv2.resize(leftImg, (width, height), interpolation=cv2.INTER_AREA)
        data = data.reshape(height*width)
        img = dai.ImgFrame()
        img.setData(data)
        img.setInstanceNum(1)
        img.setType(dai.ImgFrame.Type.RAW8)
        img.setWidth(width)
        img.setHeight(height)
        qInLeft.send(img)

        data = cv2.resize(rightImg, (width, height), interpolation=cv2.INTER_AREA)
        data = data.reshape(height*width)
        img = dai.ImgFrame()
        img.setData(data)
        img.setInstanceNum(2)
        img.setType(dai.ImgFrame.Type.RAW8)
        img.setWidth(width)
        img.setHeight(height)
        qInRight.send(img)



        spatialData = spatialCalcQueue.get().getSpatialLocations()
        inLeft = qLeft.get()
        frameLeft = inLeft.getCvFrame()
        cv2.imshow("left", frameLeft)

        inRight = qRight.get()
        frameRight = inRight.getCvFrame()
        cv2.imshow("right", frameRight)

        inDepth = qDepth.get()
        depthFrame = inDepth.getCvFrame()
        # cv2.imshow("depth", frameDepth)
        depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
        depthFrameColor = cv2.equalizeHist(depthFrameColor)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)
        for depthData in spatialData:
            roi = depthData.config.roi
            roi = roi.denormalize(width=depthFrameColor.shape[1], height=depthFrameColor.shape[0])
            xmin = int(roi.topLeft().x)
            ymin = int(roi.topLeft().y)
            xmax = int(roi.bottomRight().x)
            ymax = int(roi.bottomRight().y)

            depthMin = depthData.depthMin
            depthMax = depthData.depthMax

            fontType = cv2.FONT_HERSHEY_TRIPLEX
            cv2.rectangle(depthFrameColor, (xmin, ymin), (xmax, ymax), color, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX)
            cv2.putText(depthFrameColor, f"X: {int(depthData.spatialCoordinates.x)} mm", (xmin + 10, ymin + 20), fontType, 0.5, 255)
            cv2.putText(depthFrameColor, f"Y: {int(depthData.spatialCoordinates.y)} mm", (xmin + 10, ymin + 35), fontType, 0.5, 255)
            cv2.putText(depthFrameColor, f"Z: {int(depthData.spatialCoordinates.z)} mm", (xmin + 10, ymin + 50), fontType, 0.5, 255)
        # Show the frame

        cv2.imshow("disp", depthFrameColor)

        cnt += 1

        if cv2.waitKey(1) == ord('q'):
            break
