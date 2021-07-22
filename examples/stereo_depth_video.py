#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

withDepth = True

outputDepth = False
outputRectified = True
lrcheck = True
extended = False
subpixel = False

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.createMonoCamera()
monoRight = pipeline.createMonoCamera()
if withDepth:
    stereo = pipeline.createStereoDepth()

xoutLeft = pipeline.createXLinkOut()
xoutRight = pipeline.createXLinkOut()
xoutDisp = pipeline.createXLinkOut()
xoutDepth = pipeline.createXLinkOut()
xoutRectifL = pipeline.createXLinkOut()
xoutRectifR = pipeline.createXLinkOut()

# XLinkOut
xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
if withDepth:
    xoutDisp.setStreamName("disparity")
    xoutDepth.setStreamName("depth")
    xoutRectifL.setStreamName("rectified_left")
    xoutRectifR.setStreamName("rectified_right")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

if withDepth:
    # StereoDepth
    stereo.initialConfig.setConfidenceThreshold(230)
    stereo.setRectifyEdgeFillColor(0)  # black, to better see the cutout
    # stereo.setInputResolution(1280, 720)
    stereo.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_5x5)
    stereo.setLeftRightCheck(lrcheck)
    stereo.setExtendedDisparity(extended)
    stereo.setSubpixel(subpixel)

    # Linking
    monoLeft.out.link(stereo.left)
    monoRight.out.link(stereo.right)

    stereo.syncedLeft.link(xoutLeft.input)
    stereo.syncedRight.link(xoutRight.input)
    stereo.disparity.link(xoutDisp.input)

    if outputRectified:
        stereo.rectifiedLeft.link(xoutRectifL.input)
        stereo.rectifiedRight.link(xoutRectifR.input)

    if outputDepth:
        stereo.depth.link(xoutDepth.input)

else:
    # Link plugins CAM . XLINK
    monoLeft.out.link(xoutLeft.input)
    monoRight.out.link(xoutRight.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    leftQueue = device.getOutputQueue(name="left", maxSize=8, blocking=False)
    rightQueue = device.getOutputQueue(name="right", maxSize=8, blocking=False)
    if (withDepth):
        dispQueue = device.getOutputQueue(name="disparity", maxSize=8, blocking=False)
        depthQueue = device.getOutputQueue(name="depth", maxSize=8, blocking=False)
        rectifLeftQueue = device.getOutputQueue(name="rectified_left", maxSize=8, blocking=False)
        rectifRightQueue = device.getOutputQueue(name="rectified_right", maxSize=8, blocking=False)

    # Disparity range is used for normalization
    disparityMultiplier = 255 / stereo.getMaxDisparity()

    while True:
        left = leftQueue.get()
        cv2.imshow("left", left.getFrame())
        right = rightQueue.get()
        cv2.imshow("right", right.getFrame())

        if withDepth:
            disparity = dispQueue.get()
            disp = disparity.getCvFrame()
            disp = (disp*disparityMultiplier).astype(np.uint8) # Extended disparity range
            cv2.imshow("disparity", disp)
            disp = cv2.applyColorMap(disp, cv2.COLORMAP_JET)
            cv2.imshow("disparity_color", disp)

            if outputDepth:
                depth = depthQueue.get()
                cv2.imshow("depth", depth.getCvFrame().astype(np.uint16))

            if outputRectified:
                rectifL = rectifLeftQueue.get()
                cv2.imshow("rectified_left", rectifL.getFrame())

                rectifR = rectifRightQueue.get()
                cv2.imshow("rectified_right", rectifR.getFrame())

        if cv2.waitKey(1) == ord('q'):
            break
