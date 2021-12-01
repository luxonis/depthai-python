#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

class StereoConfigHandler:

    class Trackbar:
        def __init__(self, trackbarName, windowName, minValue, maxValue, defaultValue, handler):
            self.min = minValue
            self.max = maxValue
            self.windowName = windowName
            self.trackbarName = trackbarName
            cv2.createTrackbar(trackbarName, windowName, minValue, maxValue, handler)
            cv2.setTrackbarPos(trackbarName, windowName, defaultValue)

        def set(self, value):
            if value < self.min:
                value = self.min
                print(f'{self.trackbarName} min value is {self.min}')
            if value > self.max:
                value = self.max
                print(f'{self.trackbarName} max value is {self.max}')
            cv2.setTrackbarPos(self.trackbarName, self.windowName, value)

    newConfig = False
    config = None
    trSigma = list()
    trConfidence = list()
    trLrCheck = list()
    trFractionalBits = list()
    trLineqAlpha = list()
    trLineqBeta = list()
    trLineqThreshold = list()
    trCostAggregationP1 = list()
    trCostAggregationP2 = list()
    trTemporalAlpha = list()
    trTemporalDelta = list()
    trThresholdMinRange = list()
    trThresholdMaxRange = list()
    trSpeckleRange = list()

    def trackbarSigma(value):
        StereoConfigHandler.config.postProcessing.bilateralSigmaValue = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trSigma:
            tr.set(value)

    def trackbarConfidence(value):
        StereoConfigHandler.config.costMatching.confidenceThreshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trConfidence:
            tr.set(value)

    def trackbarLrCheckThreshold(value):
        StereoConfigHandler.config.algorithmControl.leftRightCheckThreshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLrCheck:
            tr.set(value)

    def trackbarFractionalBits(value):
        StereoConfigHandler.config.algorithmControl.subpixelFractionalBits = value
        for tr in StereoConfigHandler.trFractionalBits:
            tr.set(value)

    def trackbarLineqAlpha(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.alpha = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqAlpha:
            tr.set(value)

    def trackbarLineqBeta(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.beta = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqBeta:
            tr.set(value)

    def trackbarLineqThreshold(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.threshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqThreshold:
            tr.set(value)

    def trackbarCostAggregationP1(value):
        StereoConfigHandler.config.costAggregation.horizontalPenaltyCostP1 = value
        StereoConfigHandler.config.costAggregation.verticalPenaltyCostP1 = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trCostAggregationP1:
            tr.set(value)

    def trackbarCostAggregationP2(value):
        StereoConfigHandler.config.costAggregation.horizontalPenaltyCostP2 = value
        StereoConfigHandler.config.costAggregation.verticalPenaltyCostP2 = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trCostAggregationP2:
            tr.set(value)

    def trackbarTemporalFilterAlpha(value):
        StereoConfigHandler.config.postProcessing.temporalFilter.alpha = value / 100.
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trTemporalAlpha:
            tr.set(value)

    def trackbarTemporalFilterDelta(value):
        StereoConfigHandler.config.postProcessing.temporalFilter.delta = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trTemporalDelta:
            tr.set(value)

    def trackbarThresholdMinRange(value):
        StereoConfigHandler.config.postProcessing.thresholdFilter.minRange = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trThresholdMinRange:
            tr.set(value)

    def trackbarThresholdMaxRange(value):
        StereoConfigHandler.config.postProcessing.thresholdFilter.maxRange = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trThresholdMaxRange:
            tr.set(value)

    def trackbarSpeckleRange(value):
        StereoConfigHandler.config.postProcessing.speckleFilter.speckleRange = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trSpeckleRange:
            tr.set(value)

    def handleKeypress(key, stereoDepthConfigInQueue):
        if key == ord('m'):
            StereoConfigHandler.newConfig = True
            medianSettings = [dai.MedianFilter.MEDIAN_OFF, dai.MedianFilter.KERNEL_3x3, dai.MedianFilter.KERNEL_5x5, dai.MedianFilter.KERNEL_7x7]
            currentMedian = StereoConfigHandler.config.postProcessing.median
            nextMedian = medianSettings[(medianSettings.index(currentMedian)+1) % len(medianSettings)]
            print(f"Changing median to {nextMedian.name} from {currentMedian.name}")
            StereoConfigHandler.config.postProcessing.median = nextMedian
        if key == ord('w'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.postProcessing.spatialFilter.enable = not StereoConfigHandler.config.postProcessing.spatialFilter.enable
            state = "on" if StereoConfigHandler.config.postProcessing.spatialFilter.enable else "off"
            print(f"Spatial filter {state}")
        if key == ord('t'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.postProcessing.temporalFilter.enable = not StereoConfigHandler.config.postProcessing.temporalFilter.enable
            state = "on" if StereoConfigHandler.config.postProcessing.temporalFilter.enable else "off"
            print(f"Temporal filter {state}")
        if key == ord('s'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.postProcessing.speckleFilter.enable = not StereoConfigHandler.config.postProcessing.speckleFilter.enable
            state = "on" if StereoConfigHandler.config.postProcessing.speckleFilter.enable else "off"
            print(f"Speckle filter {state}")
        if key == ord('r'):
            StereoConfigHandler.newConfig = True
            temporalSettings = [dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.PERSISTENCY_OFF,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_8_OUT_OF_8,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_2_IN_LAST_3,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_2_IN_LAST_4,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_2_OUT_OF_8,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_1_IN_LAST_2,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_1_IN_LAST_5,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.VALID_1_IN_LAST_8,
            dai.RawStereoDepthConfig.PostProcessing.TemporalFilter.PersistencyMode.PERSISTENCY_INDEFINITELY,
            ]
            currentTemporal = StereoConfigHandler.config.postProcessing.temporalFilter.persistencyMode
            nextTemporal = temporalSettings[(temporalSettings.index(currentTemporal)+1) % len(temporalSettings)]
            print(f"Changing temporal persistency to {nextTemporal.name} from {currentTemporal.name}")
            StereoConfigHandler.config.postProcessing.temporalFilter.persistencyMode = nextTemporal
        elif key == ord('c'):
            StereoConfigHandler.newConfig = True
            censusSettings = [dai.RawStereoDepthConfig.CensusTransform.KernelSize.AUTO, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_5x5, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_7x7, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_7x9]
            currentCensus = StereoConfigHandler.config.censusTransform.kernelSize
            nextCensus = censusSettings[(censusSettings.index(currentCensus)+1) % len(censusSettings)]
            print(f"Changing census transform to {nextCensus.name} from {currentCensus.name}")
            StereoConfigHandler.config.censusTransform.kernelSize = nextCensus
        elif key == ord('d'):
            StereoConfigHandler.newConfig = True
            dispRangeSettings = [dai.RawStereoDepthConfig.CostMatching.DisparityWidth.DISPARITY_64, dai.RawStereoDepthConfig.CostMatching.DisparityWidth.DISPARITY_96]
            currentDispRange = StereoConfigHandler.config.costMatching.disparityWidth
            nextDispRange = dispRangeSettings[(dispRangeSettings.index(currentDispRange)+1) % len(dispRangeSettings)]
            print(f"Changing disparity range to {nextDispRange.name} from {currentDispRange.name}")
            StereoConfigHandler.config.costMatching.disparityWidth = nextDispRange
        elif key == ord('f'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.costMatching.enableCompanding = not StereoConfigHandler.config.costMatching.enableCompanding
            state = "on" if StereoConfigHandler.config.costMatching.enableCompanding else "off"
            print(f"Companding {state}")
        elif key == ord('v'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.censusTransform.enableMeanMode = not StereoConfigHandler.config.censusTransform.enableMeanMode
            state = "on" if StereoConfigHandler.config.censusTransform.enableMeanMode else "off"
            print(f"Census transform mean mode {state}")
        elif key == ord('1'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.algorithmControl.enableLeftRightCheck = not StereoConfigHandler.config.algorithmControl.enableLeftRightCheck
            state = "on" if StereoConfigHandler.config.algorithmControl.enableLeftRightCheck else "off"
            print(f"LR-check {state}")
        elif key == ord('2'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.algorithmControl.enableSubpixel = not StereoConfigHandler.config.algorithmControl.enableSubpixel
            state = "on" if StereoConfigHandler.config.algorithmControl.enableSubpixel else "off"
            print(f"Subpixel {state}")
        elif key == ord('3'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.algorithmControl.enableExtended = not StereoConfigHandler.config.algorithmControl.enableExtended
            state = "on" if StereoConfigHandler.config.algorithmControl.enableExtended else "off"
            print(f"Extended {state}")

        StereoConfigHandler.sendConfig(stereoDepthConfigInQueue)

    def sendConfig(stereoDepthConfigInQueue):
        if StereoConfigHandler.newConfig:
            StereoConfigHandler.newConfig = False
            configMessage = dai.StereoDepthConfig()
            configMessage.set(StereoConfigHandler.config)
            stereoDepthConfigInQueue.send(configMessage)

    def registerWindow(stream):
        cv2.namedWindow(stream)
        StereoConfigHandler.trConfidence.append(StereoConfigHandler.Trackbar('Disparity confidence', stream, 0, 255, StereoConfigHandler.config.costMatching.confidenceThreshold, StereoConfigHandler.trackbarConfidence))
        StereoConfigHandler.trSigma.append(StereoConfigHandler.Trackbar('Bilateral sigma', stream, 0, 100, StereoConfigHandler.config.postProcessing.bilateralSigmaValue, StereoConfigHandler.trackbarSigma))
        StereoConfigHandler.trLrCheck.append(StereoConfigHandler.Trackbar('LR-check threshold', stream, 0, 16, StereoConfigHandler.config.algorithmControl.leftRightCheckThreshold, StereoConfigHandler.trackbarLrCheckThreshold))
        StereoConfigHandler.trFractionalBits.append(StereoConfigHandler.Trackbar('Subpixel fractional bits', stream, 3, 5, StereoConfigHandler.config.algorithmControl.subpixelFractionalBits, StereoConfigHandler.trackbarFractionalBits))
        StereoConfigHandler.trLineqAlpha.append(StereoConfigHandler.Trackbar('Linear equation alpha', stream, 0, 15, StereoConfigHandler.config.costMatching.linearEquationParameters.alpha, StereoConfigHandler.trackbarLineqAlpha))
        StereoConfigHandler.trLineqBeta.append(StereoConfigHandler.Trackbar('Linear equation beta', stream, 0, 15, StereoConfigHandler.config.costMatching.linearEquationParameters.beta, StereoConfigHandler.trackbarLineqBeta))
        StereoConfigHandler.trLineqThreshold.append(StereoConfigHandler.Trackbar('Linear equation threshold', stream, 0, 255, StereoConfigHandler.config.costMatching.linearEquationParameters.threshold, StereoConfigHandler.trackbarLineqThreshold))
        StereoConfigHandler.trCostAggregationP1.append(StereoConfigHandler.Trackbar('Cost aggregation P1', stream, 0, 500, StereoConfigHandler.config.costAggregation.horizontalPenaltyCostP1, StereoConfigHandler.trackbarCostAggregationP1))
        StereoConfigHandler.trCostAggregationP2.append(StereoConfigHandler.Trackbar('Cost aggregation P2', stream, 0, 500, StereoConfigHandler.config.costAggregation.horizontalPenaltyCostP2, StereoConfigHandler.trackbarCostAggregationP2))
        StereoConfigHandler.trTemporalAlpha.append(StereoConfigHandler.Trackbar('Temporal filter alpha', stream, 0, 100, int(StereoConfigHandler.config.postProcessing.temporalFilter.alpha*100), StereoConfigHandler.trackbarTemporalFilterAlpha))
        StereoConfigHandler.trTemporalDelta.append(StereoConfigHandler.Trackbar('Temporal filter delta', stream, 1, 200, StereoConfigHandler.config.postProcessing.temporalFilter.delta, StereoConfigHandler.trackbarTemporalFilterDelta))
        StereoConfigHandler.trThresholdMinRange.append(StereoConfigHandler.Trackbar('Threshold filter min range', stream, 0, 15000, StereoConfigHandler.config.postProcessing.thresholdFilter.minRange, StereoConfigHandler.trackbarThresholdMinRange))
        StereoConfigHandler.trThresholdMaxRange.append(StereoConfigHandler.Trackbar('Threshold filter max range', stream, 0, 15000, StereoConfigHandler.config.postProcessing.thresholdFilter.maxRange, StereoConfigHandler.trackbarThresholdMaxRange))
        StereoConfigHandler.trSpeckleRange.append(StereoConfigHandler.Trackbar('Speckle filter range', stream, 0, 100, StereoConfigHandler.config.postProcessing.speckleFilter.speckleRange, StereoConfigHandler.trackbarSpeckleRange))

    def __init__(self, config):
        print("Control median filter using the 'm' key.")
        print("Control census transform kernel size using the 'c' key.")
        print("Control disparity search range using the 'd' key.")
        print("Control disparity companding using the 'f' key.")
        print("Control census transform mean mode using the 'v' key.")
        print("Control left-right check mode using the '1' key.")
        print("Control subpixel mode using the '2' key.")
        print("Control extended mode using the '3' key.")

        StereoConfigHandler.config = config

# Closer-in minimum depth, disparity range is doubled (from 95 to 190):
extended_disparity = False
# Better accuracy for longer distance, fractional disparity 32-levels:
subpixel = False
# Better handling for occlusions:
lr_check = False

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
depth = pipeline.create(dai.node.StereoDepth)
xout = pipeline.create(dai.node.XLinkOut)

xout.setStreamName("disparity")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create a node that will produce the depth map (using disparity output as it's easier to visualize depth this way)
depth.initialConfig.setConfidenceThreshold(245)
# Options: MEDIAN_OFF, KERNEL_3x3, KERNEL_5x5, KERNEL_7x7 (default)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_5x5)
depth.setLeftRightCheck(True)
depth.setExtendedDisparity(extended_disparity)
depth.setSubpixel(subpixel)
depth.setRuntimeModeSwitch(True)

# Linking
monoLeft.out.link(depth.left)
monoRight.out.link(depth.right)
depth.depth.link(xout.input)

StereoConfigHandler(depth.initialConfig.get())
StereoConfigHandler.registerWindow('disparity')

xinStereoDepthConfig = pipeline.create(dai.node.XLinkIn)
xinStereoDepthConfig.setStreamName("stereoDepthConfig")
xinStereoDepthConfig.out.link(depth.inputConfig)



# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the disparity frames from the outputs defined above
    q = device.getOutputQueue(name="disparity", maxSize=4, blocking=False)
    stereoDepthConfigInQueue = device.getInputQueue("stereoDepthConfig")

    while True:
        inDepth = q.get()  # blocking call, will wait until a new data has arrived
        frame = inDepth.getFrame()
        # Normalization for better visualization
        depthFrame = frame

        depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
        depthFrameColor = cv2.equalizeHist(depthFrameColor)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

        cv2.imshow("disparity", depthFrameColor)


        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        StereoConfigHandler.handleKeypress(key, stereoDepthConfigInQueue)
