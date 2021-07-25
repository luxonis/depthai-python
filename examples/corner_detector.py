#!/usr/bin/env python3

import cv2
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
featureTrackerLeft = pipeline.createFeatureTracker()
featureTrackerRight = pipeline.createFeatureTracker()

xoutPassthroughFrameLeft = pipeline.createXLinkOut()
xoutTrackedFeaturesLeft = pipeline.createXLinkOut()
xoutPassthroughFrameRight = pipeline.createXLinkOut()
xoutTrackedFeaturesRight = pipeline.createXLinkOut()
xinTrackedFeaturesConfig = pipeline.createXLinkIn()


xoutPassthroughFrameLeft.setStreamName("passthroughFrameLeft")
xoutTrackedFeaturesLeft.setStreamName("trackedFeaturesLeft")
xoutPassthroughFrameRight.setStreamName("passthroughFrameRight")
xoutTrackedFeaturesRight.setStreamName("trackedFeaturesRight")
xinTrackedFeaturesConfig.setStreamName("trackedFeaturesConfig")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Linking
monoLeft.out.link(featureTrackerLeft.inputImage)
featureTrackerLeft.passthroughInputImage.link(xoutPassthroughFrameLeft.input)
featureTrackerLeft.outputFeatures.link(xoutTrackedFeaturesLeft.input)
featureTrackerLeft.initialConfig.setMotionEstimator(False)
xinTrackedFeaturesConfig.out.link(featureTrackerLeft.inputConfig)

monoRight.out.link(featureTrackerRight.inputImage)
featureTrackerRight.passthroughInputImage.link(xoutPassthroughFrameRight.input)
featureTrackerRight.outputFeatures.link(xoutTrackedFeaturesRight.input)
featureTrackerRight.initialConfig.setMotionEstimator(False)
xinTrackedFeaturesConfig.out.link(featureTrackerRight.inputConfig)

featureTrackerConfig = featureTrackerRight.initialConfig.get()

print("Press 's' to switch between Harris and Shi-Thomasi corner detector!")

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues used to receive the results
    passthroughImageLeftQueue = device.getOutputQueue("passthroughFrameLeft", 8, False)
    outputFeaturesLeftQueue = device.getOutputQueue("trackedFeaturesLeft", 8, False)
    passthroughImageRightQueue = device.getOutputQueue("passthroughFrameRight", 8, False)
    outputFeaturesRightQueue = device.getOutputQueue("trackedFeaturesRight", 8, False)

    inputFeatureTrackerConfigQueue = device.getInputQueue("trackedFeaturesConfig")


    leftWindowName = "left"
    rightWindowName = "right"

    pointColor = (0, 0, 255)
    circleRadius = 2

    while True:
        inPassthroughFrameLeft = passthroughImageLeftQueue.get()
        passthroughFrameLeft = inPassthroughFrameLeft.getFrame()
        leftFrame = cv2.cvtColor(passthroughFrameLeft, cv2.COLOR_GRAY2BGR)

        inPassthroughFrameRight = passthroughImageRightQueue.get()
        passthroughFrameRight = inPassthroughFrameRight.getFrame()
        rightFrame = cv2.cvtColor(passthroughFrameRight, cv2.COLOR_GRAY2BGR)

        trackedFeaturesLeft = outputFeaturesLeftQueue.get().trackedFeatures
        for feature in trackedFeaturesLeft:
            cv2.circle(leftFrame, (int(feature.position.x), int(feature.position.y)), circleRadius, pointColor, -1, cv2.LINE_AA, 0)

        trackedFeaturesRight = outputFeaturesRightQueue.get().trackedFeatures
        for feature in trackedFeaturesRight:
            cv2.circle(rightFrame, (int(feature.position.x), int(feature.position.y)), circleRadius, pointColor, -1, cv2.LINE_AA, 0)

        # Show the frame
        cv2.imshow(leftWindowName, leftFrame)
        cv2.imshow(rightWindowName, rightFrame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('s'):
            if featureTrackerConfig.cornerDetector.algorithmType == dai.FeatureTrackerConfigData.CornerDetector.AlgorithmType.HARRIS:
                featureTrackerConfig.cornerDetector.algorithmType = dai.FeatureTrackerConfigData.CornerDetector.AlgorithmType.SHI_THOMASI
                print("Switchig to Shi-Thomasi")
            else:
                featureTrackerConfig.cornerDetector.algorithmType = dai.FeatureTrackerConfigData.CornerDetector.AlgorithmType.HARRIS
                print("Switchig to Harris")

            cfg = dai.FeatureTrackerConfig()
            cfg.set(featureTrackerConfig)
            inputFeatureTrackerConfigQueue.send(cfg)


            pass