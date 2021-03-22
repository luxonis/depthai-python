#!/usr/bin/env python3

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) cameras
camLeft = pipeline.createMonoCamera()
camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

camRight = pipeline.createMonoCamera()
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
camRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Create outputs
xoutLeft = pipeline.createXLinkOut()
xoutLeft.setStreamName('left')
camLeft.out.link(xoutLeft.input)

xoutRight = pipeline.createXLinkOut()
xoutRight.setStreamName('right')
camRight.out.link(xoutRight.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale frames from the outputs defined above
    qLeft = device.getOutputQueue(name="left", maxSize=4, blocking=False)
    qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)

    frameLeft = None
    frameRight = None

    while True:
        # instead of get (blocking) used tryGet (nonblocking) which will return the available data or None otherwise
        inLeft = qLeft.tryGet()
        inRight = qRight.tryGet()

        if inLeft is not None:
            frameLeft = inLeft.getCvFrame()

        if inRight is not None:
            frameRight = inRight.getCvFrame()

        # show the frames if available
        if frameLeft is not None:
            cv2.imshow("left", frameLeft)
        if frameRight is not None:
            cv2.imshow("right", frameRight)

        if cv2.waitKey(1) == ord('q'):
            break
