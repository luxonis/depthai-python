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
depthCalculator = pipeline.createDepthCalculator()

xoutDepth = pipeline.createXLinkOut()
xoutDepthData = pipeline.createXLinkOut()
xinDepthCalcConfig = pipeline.createXLinkIn()

xoutDepth.setStreamName("depth")
xoutDepthData.setStreamName("depthCalcData")
xinDepthCalcConfig.setStreamName("depthCalcConfig")

# MonoCamera
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

outputDepth = True
outputRectified = False
lrcheck = False
extended = False
subpixel = False

# StereoDepth
stereo.setOutputDepth(outputDepth)
stereo.setOutputRectified(outputRectified)
stereo.setConfidenceThreshold(255)

stereo.setLeftRightCheck(lrcheck)
stereo.setExtendedDisparity(extended)
stereo.setSubpixel(subpixel)

monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

stereo.depth.link(xoutDepth.input)
stereo.depth.link(depthCalculator.depthInput)

bbXmin = 0.4
bbXmax = 0.6
bbYmin = 0.4
bbYmax = 0.6

depthCalculator.setWaitForConfigInput(False)
config = dai.DepthCalculatorConfigData()
config.lower_threshold = 100
config.upper_threshold = 10000
config.roi = dai.Rect(bbXmin, bbYmin, bbXmax, bbYmax)
depthCalculator.initialConfig.addROI(config)
depthCalculator.out.link(xoutDepthData.input)
xinDepthCalcConfig.out.link(depthCalculator.inputConfig)

# Pipeline defined, now the device is assigned and pipeline is started
device = dai.Device(pipeline)
device.startPipeline()

# Output queue will be used to get the depth frames from the outputs defined above
depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
depthCalcQueue = device.getOutputQueue(name="depthCalcData", maxSize=4, blocking=False)
depthCalcConfigInQueue = device.getInputQueue("depthCalcConfig")

color = (255, 255, 255)

while True:
    inDepth = depthQueue.get() # blocking call, will wait until a new data has arrived
    inDepthAvg = depthCalcQueue.get() # blocking call, will wait until a new data has arrived
    
    depthFrame = inDepth.getFrame()
    depthFrameColor = cv2.normalize(depthFrame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8UC1)
    depthFrameColor = cv2.equalizeHist(depthFrameColor)
    depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

    depthCalcData = inDepthAvg.getDepthData()
    for depthData in depthCalcData:
        roi = depthData.config.roi
        xmin = int(roi.xmin * inDepth.getWidth())
        ymin = int(roi.ymin * inDepth.getHeight())
        xmax = int(roi.xmax * inDepth.getWidth())
        ymax = int(roi.ymax * inDepth.getHeight())

        fontType = cv2.FONT_HERSHEY_TRIPLEX
        cv2.rectangle(depthFrameColor, (xmin, ymin), (xmax, ymax), color, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX)
        cv2.putText(depthFrameColor, f"X: {int(depthData.depth_avg)} mm", (xmin + 10, ymin + 20), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"X: {int(depthData.depth_x)} mm", (xmin + 10, ymin + 35), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"Y: {int(depthData.depth_y)} mm", (xmin + 10, ymin + 50), fontType, 0.5, color)
        cv2.putText(depthFrameColor, f"Z: {int(depthData.depth_z)} mm", (xmin + 10, ymin + 65), fontType, 0.5, color)


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
        cfg = dai.DepthCalculatorConfig()
        cfg.addROI(config)
        depthCalcConfigInQueue.send(cfg)