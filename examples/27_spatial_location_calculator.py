#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

stepSize = 0.05

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) cameras
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
stereo = pipeline.createStereoDepth()
spatialLocationCalculator = pipeline.createSpatialLocationCalculator()

xoutDepth = pipeline.createXLinkOut()
xoutSpatialData = pipeline.createXLinkOut()
xinSpatialCalcConfig = pipeline.createXLinkIn()

xoutDepth.setStreamName("depth")
xoutSpatialData.setStreamName("spatialData")
xinSpatialCalcConfig.setStreamName("spatialCalcConfig")

# MonoCamera
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

outputDepth = True
outputRectified = False
lrcheck = True
subpixel = True

# StereoDepth
stereo.setOutputDepth(outputDepth)
stereo.setOutputRectified(outputRectified)
stereo.setConfidenceThreshold(255)

stereo.setLeftRightCheck(lrcheck)
stereo.setSubpixel(subpixel)

monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

stereo.depth.link(xoutDepth.input)
stereo.depth.link(spatialLocationCalculator.inputDepth)

bbXmin = 0.4
bbXmax = 0.6
bbYmin = 0.4
bbYmax = 0.6

spatialLocationCalculator.setWaitForConfigInput(False)
config = dai.SpatialLocationCalculatorConfigData()
config.depthThresholds.lowerThreshold = 100
config.depthThresholds.upperThreshold = 10000
config.roi = dai.Rect(bbXmin, bbYmin, bbXmax, bbYmax)
spatialLocationCalculator.initialConfig.addROI(config)
spatialLocationCalculator.out.link(xoutSpatialData.input)
xinSpatialCalcConfig.out.link(spatialLocationCalculator.inputConfig)

# Pipeline defined, now the device is assigned and pipeline is started
device = dai.Device(pipeline)
device.startPipeline()

# Output queue will be used to get the depth frames from the outputs defined above
depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
spatialCalcQueue = device.getOutputQueue(name="spatialData", maxSize=4, blocking=False)
spatialCalcConfigInQueue = device.getInputQueue("spatialCalcConfig")

color = (255, 255, 255)

print("Use WASD keys to move ROI!")

while True:
    inDepth = depthQueue.get() # blocking call, will wait until a new data has arrived
    inDepthAvg = spatialCalcQueue.get() # blocking call, will wait until a new data has arrived
    
    depthFrame = inDepth.getFrame()
    depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
    depthFrameColor = cv2.equalizeHist(depthFrameColor)
    depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

    spatialData = inDepthAvg.getSpatialLocations()
    for depthData in spatialData:
        roi = depthData.config.roi
        xmin = int(roi.xmin * inDepth.getWidth())
        ymin = int(roi.ymin * inDepth.getHeight())
        xmax = int(roi.xmax * inDepth.getWidth())
        ymax = int(roi.ymax * inDepth.getHeight())

        fontType = cv2.FONT_HERSHEY_TRIPLEX
        cv2.rectangle(depthFrameColor, (xmin, ymin), (xmax, ymax), color, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX)
        cv2.putText(depthFrameColor, f"Avg: {int(depthData.depthAverage)} mm", (xmin + 10, ymin + 20), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"X: {int(depthData.spatialCoordinates.x)} mm", (xmin + 10, ymin + 35), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"Y: {int(depthData.spatialCoordinates.y)} mm", (xmin + 10, ymin + 50), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"Z: {int(depthData.spatialCoordinates.z)} mm", (xmin + 10, ymin + 65), fontType, 0.5, color)


    cv2.imshow("depth", depthFrameColor)

    newConfig = False
    key = cv2.waitKey(1)
    if key == ord('q'):
        break
    elif key == ord('w'):
        if bbYmin - stepSize >= 0:
            bbYmin -= stepSize
            bbYmax -= stepSize
            newConfig = True
    elif key == ord('a'):
        if bbXmin - stepSize >= 0:
            bbXmin -= stepSize
            bbXmax -= stepSize
            newConfig = True
    elif key == ord('s'):
        if bbYmax + stepSize <= 1:
            bbYmin += stepSize
            bbYmax += stepSize
            newConfig = True
    elif key == ord('d'):
        if bbXmax + stepSize <= 1:
            bbXmin += stepSize
            bbXmax += stepSize
            newConfig = True

    if newConfig:
        config.roi = dai.Rect(bbXmin, bbYmin, bbXmax, bbYmax)
        cfg = dai.SpatialLocationCalculatorConfig()
        cfg.addROI(config)
        spatialCalcConfigInQueue.send(cfg)