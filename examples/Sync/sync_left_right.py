#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np


# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
syncNode = pipeline.create(dai.node.Sync)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")


# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

monoLeft.out.link(syncNode.inputs["left"])
monoRight.out.link(syncNode.inputs["right"])
syncNode.outputs["left"].link(xoutLeft.input)
syncNode.outputs["right"].link(xoutRight.input)

with dai.Device(pipeline) as device:
    qLeft = device.getOutputQueue(name="left", maxSize=4, blocking=False)
    qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)
    while True:
        inLeft = qLeft.get()
        inRight = qRight.get()

        cv2.imshow("left", inLeft.getCvFrame())
        cv2.imshow("right", inRight.getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            break