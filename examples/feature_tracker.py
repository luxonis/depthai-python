#!/usr/bin/env python3

import cv2
import depthai as dai
from collections import deque

trackedIDs = set()
trackedFeaturesPath = dict()

lineColor = (200, 0, 200)
pointColor = (0, 0, 255)
circleRadius = 2
maxTrackedFeaturesPathLength = 30
trackedFeaturesPathLength = 10

def onTrackBar(val):
    global trackedFeaturesPathLength
    trackedFeaturesPathLength = val
    pass

def trackFeaturePath(features):
    global trackedIDs
    global trackedFeaturesPath

    newTrackedIDs = set()
    for currentFeature in features:
        currentID = currentFeature.id
        newTrackedIDs.add(currentID)

        if currentID not in trackedFeaturesPath:
            trackedFeaturesPath[currentID] = deque()

        path = trackedFeaturesPath[currentID]

        path.append(currentFeature.position)
        while(len(path) > max(1, trackedFeaturesPathLength)):
            path.popleft()

        trackedFeaturesPath[currentID] = path

    featuresToRemove = set()
    for oldId in trackedIDs:
        if oldId not in newTrackedIDs:
            featuresToRemove.add(oldId)

    for id in featuresToRemove:
        trackedFeaturesPath.pop(id)

    trackedIDs = newTrackedIDs

def drawFeatures(img):
    global trackedFeaturesPath

    # For every feature,
    for featurePath in trackedFeaturesPath.values():
        path = featurePath

        # Draw the feature movement path.
        for j in range(len(path) - 1):
            src = (int(path[j].x), int(path[j].y))
            dst = (int(path[j + 1].x), int(path[j + 1].y))
            cv2.line(img, src, dst, lineColor, 1, cv2.LINE_AA, 0)
        j = len(path) - 1
        cv2.circle(img, (int(path[j].x), int(path[j].y)), circleRadius, pointColor, -1, cv2.LINE_AA, 0)



# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
featureTrackerLeft = pipeline.createFeatureTracker()

xoutPassthroughFrameLeft = pipeline.createXLinkOut()
xoutTrackedFeaturesLeft = pipeline.createXLinkOut()

xoutPassthroughFrameLeft.setStreamName("passthroughFrameLeft")
xoutTrackedFeaturesLeft.setStreamName("trackedFeaturesLeft")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Linking
monoLeft.out.link(featureTrackerLeft.inputImage)

featureTrackerLeft.passthroughInputImage.link(xoutPassthroughFrameLeft.input)
featureTrackerLeft.outputFeatures.link(xoutTrackedFeaturesLeft.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues used to receive the results
    passthroughImageLeftQueue = device.getOutputQueue("passthroughFrameLeft", 8, False)
    outputFeaturesLeftQueue = device.getOutputQueue("trackedFeaturesLeft", 8, False)

    leftWindowName = "left"
    cv2.namedWindow(leftWindowName)
    cv2.createTrackbar("Feature tracking duration (frames)", leftWindowName, trackedFeaturesPathLength, maxTrackedFeaturesPathLength, onTrackBar)

    while True:
        inPassthroughFrameLeft = passthroughImageLeftQueue.get()

        passthroughFrameLeft = inPassthroughFrameLeft.getFrame()
        frame = cv2.cvtColor(passthroughFrameLeft, cv2.COLOR_GRAY2BGR)

        trackedFeaturesLeft = outputFeaturesLeftQueue.get().trackedFeatures
        trackFeaturePath(trackedFeaturesLeft)
        drawFeatures(frame)

        # Show the frame
        cv2.imshow(leftWindowName, frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break