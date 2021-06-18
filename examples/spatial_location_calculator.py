#!/usr/bin/env python3

import cv2
import depthai as dai

class trackbar:
    def __init__(self, trackbarName, windowName, minValue, maxValue, defaultValue, handler):
        cv2.createTrackbar(trackbarName, windowName, minValue, maxValue, handler)
        cv2.setTrackbarPos(trackbarName, windowName, defaultValue)

class depthHandler:
    depthStream = "depth"
    _send_new_config = False
    currentConfig = dai.StereoDepthConfig()

    def on_trackbar_change_sigma(self, value):
        self._sigma = value
        self._send_new_config = True

    def on_trackbar_change_confidence(self, value):
        self._confidence = value
        self._send_new_config = True

    def handleKeypress(self, key, stereoDepthConfigInQueue):
        if key == ord('m'):
            self._send_new_config = True
            medianSettings = [dai.MedianFilter.MEDIAN_OFF, dai.MedianFilter.KERNEL_3x3, dai.MedianFilter.KERNEL_5x5, dai.MedianFilter.KERNEL_7x7]
            currentMedian = self.currentConfig.getMedianFilter()
            # circle through median settins
            nextMedian = medianSettings[(medianSettings.index(currentMedian)+1) % len(medianSettings)]
            self.currentConfig.setMedianFilter(nextMedian)
            print(f"Changing median to {nextMedian.name} from {currentMedian.name}")
        self.sendConfig(stereoDepthConfigInQueue)

    def __init__(self, _confidence, _sigma):
        print("Control median filter using the 'm' key.")
        print("Use slider to adjust disparity confidence.")
        print("Use slider to adjust bilateral filter intensity.")
        self._confidence = _confidence
        self._sigma = _sigma
        cv2.namedWindow(self.depthStream)
        self.lambdaTrackbar = trackbar('Disparity confidence', self.depthStream, 0, 255, _confidence, self.on_trackbar_change_confidence)
        self.sigmaTrackbar  = trackbar('Bilateral sigma',  self.depthStream, 0, 250, _sigma, self.on_trackbar_change_sigma)

    def imshow(self, frame):
        cv2.imshow(self.depthStream, frame)

    def sendConfig(self, stereoDepthConfigInQueue):
        if self._send_new_config:
            self._send_new_config = False
            self.currentConfig.setConfidenceThreshold(self._confidence)
            self.currentConfig.setBilateralFilterSigma(self._sigma)
            stereoDepthConfigInQueue.send(self.currentConfig)

stepSize = 0.05

newConfig = False

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
stereo = pipeline.createStereoDepth()
spatialLocationCalculator = pipeline.createSpatialLocationCalculator()

xoutDepth = pipeline.createXLinkOut()
xoutSpatialData = pipeline.createXLinkOut()
xinSpatialCalcConfig = pipeline.createXLinkIn()
xinStereoDepthConfig = pipeline.createXLinkIn()

xoutDepth.setStreamName("depth")
xoutSpatialData.setStreamName("spatialData")
xinSpatialCalcConfig.setStreamName("spatialCalcConfig")
xinStereoDepthConfig.setStreamName("stereoDepthConfig")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

lrcheck = True
subpixel = False

stereo.initialConfig.setConfidenceThreshold(230)
stereo.setLeftRightCheck(lrcheck)
stereo.setSubpixel(subpixel)

# Config
topLeft = dai.Point2f(0.4, 0.4)
bottomRight = dai.Point2f(0.6, 0.6)

config = dai.SpatialLocationCalculatorConfigData()
config.depthThresholds.lowerThreshold = 100
config.depthThresholds.upperThreshold = 10000
config.roi = dai.Rect(topLeft, bottomRight)

spatialLocationCalculator.setWaitForConfigInput(False)
spatialLocationCalculator.initialConfig.addROI(config)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

spatialLocationCalculator.passthroughDepth.link(xoutDepth.input)
stereo.depth.link(spatialLocationCalculator.inputDepth)
xinStereoDepthConfig.out.link(stereo.inputConfig)

spatialLocationCalculator.out.link(xoutSpatialData.input)
xinSpatialCalcConfig.out.link(spatialLocationCalculator.inputConfig)

depthHandler = depthHandler(_confidence=stereo.initialConfig.getConfidenceThreshold(), _sigma=stereo.initialConfig.getBilateralFilterSigma())


# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the depth frames from the outputs defined above
    depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
    spatialCalcQueue = device.getOutputQueue(name="spatialData", maxSize=4, blocking=False)
    spatialCalcConfigInQueue = device.getInputQueue("spatialCalcConfig")
    stereoDepthConfigInQueue = device.getInputQueue("stereoDepthConfig")

    color = (255, 255, 255)

    print("Use WASD keys to move ROI!")

    while True:
        inDepth = depthQueue.get() # Blocking call, will wait until a new data has arrived

        depthFrame = inDepth.getFrame()
        depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
        depthFrameColor = cv2.equalizeHist(depthFrameColor)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

        spatialData = spatialCalcQueue.get().getSpatialLocations()
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
        depthHandler.imshow(depthFrameColor)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('w'):
            if topLeft.y - stepSize >= 0:
                topLeft.y -= stepSize
                bottomRight.y -= stepSize
                newConfig = True
        elif key == ord('a'):
            if topLeft.x - stepSize >= 0:
                topLeft.x -= stepSize
                bottomRight.x -= stepSize
                newConfig = True
        elif key == ord('s'):
            if bottomRight.y + stepSize <= 1:
                topLeft.y += stepSize
                bottomRight.y += stepSize
                newConfig = True
        elif key == ord('d'):
            if bottomRight.x + stepSize <= 1:
                topLeft.x += stepSize
                bottomRight.x += stepSize
                newConfig = True

        if newConfig:
            config.roi = dai.Rect(topLeft, bottomRight)
            cfg = dai.SpatialLocationCalculatorConfig()
            cfg.addROI(config)
            spatialCalcConfigInQueue.send(cfg)
            newConfig = False

        depthHandler.handleKeypress(key, stereoDepthConfigInQueue)
