#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
spatialLocationCalculator = pipeline.createSpatialLocationCalculator()

xoutDepth = pipeline.createXLinkOut()


xoutDepth.setStreamName("features")


# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_800_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_800_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)


# Config, unused
topLeft = dai.Point2f(0.4, 0.4)
bottomRight = dai.Point2f(0.6, 0.6)

config = dai.SpatialLocationCalculatorConfigData()
config.roi = dai.Rect(topLeft, bottomRight)

spatialLocationCalculator.setWaitForConfigInput(False)
spatialLocationCalculator.initialConfig.addROI(config)

# Linking

spatialLocationCalculator.passthroughDepth.link(xoutDepth.input)
monoLeft.out.link(spatialLocationCalculator.inputDepth)


# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the depth frames from the outputs defined above
    featureQueue = device.getOutputQueue(name="features", maxSize=4, blocking=False)

    while True:
        inFeatures = featureQueue.get() # Blocking call, will wait until a new data has arrived

        featureFrame = inFeatures.getFrame()

        # Show the frame
        cv2.imshow("features", featureFrame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break