#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import argparse

# Create pipeline
pipeline = dai.Pipeline()

monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutRectifiedLeft = pipeline.create(dai.node.XLinkOut)
xoutRectifiedRight = pipeline.create(dai.node.XLinkOut)
xoutDescriptors = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutRectifiedLeft.setStreamName("rectified_left")
xoutRectifiedRight.setStreamName("rectified_right")
xoutDescriptors.setStreamName("descriptors")
xoutDepth.setStreamName("depth")


# Define sources and outputs
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.depth.link(xoutDepth.input)
stereo.rectifiedLeft.link(xoutRectifiedLeft.input)
stereo.rectifiedRight.link(xoutRectifiedRight.input)
stereo.pixelDescriptors.link(xoutDescriptors.input)


# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("depth", 4, False)
    qRectifiedLeft = device.getOutputQueue("rectified_left", 4, False)
    qRectifiedRight = device.getOutputQueue("rectified_right", 4, False)
    qDescriptors = device.getOutputQueue("descriptors", 4, False)


    while True:

        inLeft = qLeft.get()
        frameLeft = inLeft.getCvFrame()
        cv2.imshow("left", frameLeft)

        inRight = qRight.get()
        frameRight = inRight.getCvFrame()
        cv2.imshow("right", frameRight)

        inRLeft = qRectifiedLeft.get()
        frameRLeft = inRLeft.getCvFrame()
        cv2.imshow("rectified_left", frameRLeft)

        inRRight = qRectifiedRight.get()
        frameRRight = inRRight.getCvFrame()
        cv2.imshow("rectified_right", frameRRight)

        inDescriptors = qDescriptors.get()

        inDepth = qDepth.get()
        frameDepth = inDepth.getCvFrame()
        # cv2.imshow("depth", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disp", disp)



        if cv2.waitKey(1) == ord('q'):
            break