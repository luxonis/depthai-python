#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
manipLeft = pipeline.create(dai.node.ImageManip)
# monoRight = pipeline.create(dai.node.MonoCamera)
# monoVertical = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
# xoutRight = pipeline.create(dai.node.XLinkOut)
# xoutVertical = pipeline.create(dai.node.XLinkOut)

xoutLeft.setStreamName('left')
manipLeft.setResize(640, 480)
manipLeft.setKeepAspectRatio(False)
# xoutRight.setStreamName('right')
# xoutVertical.setStreamName('vertical')

# Properties
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
# monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
# monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
# monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
# monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoLeft.out.link(manipLeft.inputImage)
manipLeft.out.link(xoutLeft.input)
# monoRight.out.link(xoutRight.input)
# monoVertical.out.link(xoutVertical.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues will be used to get the grayscale frames from the outputs defined above
    qLeft = device.getOutputQueue(name="left", maxSize=4, blocking=False)
    # qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)
    # qVertical = device.getOutputQueue(name="vertical", maxSize=4, blocking=False)

    while True:
        # Instead of get (blocking), we use tryGet (non-blocking) which will return the available data or None otherwise
        inLeft = qLeft.get()
        # inRight = qRight.get()
        # inVertical = qVertical.get()

        cv2.imshow("left", inLeft.getCvFrame())
        # cv2.imshow("right", inRight.getCvFrame())
        # cv2.imshow("vertical", inVertical.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
