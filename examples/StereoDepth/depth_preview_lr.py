#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Closer-in minimum depth, disparity range is doubled (from 95 to 190):
extended_disparity = True
# Better accuracy for longer distance, fractional disparity 32-levels:
subpixel = True
# Better handling for occlusions:
lr_check = True

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
left = pipeline.create(dai.node.ColorCamera)
right = pipeline.create(dai.node.ColorCamera)
depth = pipeline.create(dai.node.StereoDepth)
xout = pipeline.create(dai.node.XLinkOut)
xoutl = pipeline.create(dai.node.XLinkOut)
xoutr = pipeline.create(dai.node.XLinkOut)

xout.setStreamName("disparity")
xoutl.setStreamName("rectifiedLeft")
xoutr.setStreamName("rectifiedRight")

# Properties
left.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
left.setBoardSocket(dai.CameraBoardSocket.LEFT)
right.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)
right.setIspScale(2, 3)
left.setIspScale(2, 3)


# Create a node that will produce the depth map (using disparity output as it's easier to visualize depth this way)
depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
# Options: MEDIAN_OFF, KERNEL_3x3, KERNEL_5x5, KERNEL_7x7 (default)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
depth.setInputResolution(1280, 800)
depth.setLeftRightCheck(lr_check)
depth.setExtendedDisparity(extended_disparity)
depth.setSubpixel(subpixel)
depth.setInputResolution(1280, 800)

# Linking
left.isp.link(depth.left)
right.isp.link(depth.right)
depth.disparity.link(xout.input)
depth.rectifiedLeft.link(xoutl.input)
depth.rectifiedRight.link(xoutr.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    while not device.isClosed():
        queueNames = device.getQueueEvents()
        for q in queueNames:
            message = device.getOutputQueue(q).get()
            # Display arrived frames
            if type(message) == dai.ImgFrame:
                frame = message.getCvFrame()
                if 'disparity' in q:
                    maxDisp = depth.initialConfig.getMaxDisparity()
                    disp = (frame * (255.0 / maxDisp)).astype(np.uint8)
                    disp = cv2.applyColorMap(disp, cv2.COLORMAP_JET)
                    cv2.imshow(q, disp)
                else:
                    cv2.imshow(q, frame)
        if cv2.waitKey(1) == ord('q'):
            break