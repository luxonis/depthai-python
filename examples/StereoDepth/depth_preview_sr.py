#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Closer-in minimum depth, disparity range is doubled (from 95 to 190):
extended_disparity = False
# Better accuracy for longer distance, fractional disparity 32-levels:
subpixel = True
# Better handling for occlusions:
lr_check = True

enableRectified = True

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
left = pipeline.create(dai.node.ColorCamera)
right = pipeline.create(dai.node.ColorCamera)

# Create stereo
stereo = pipeline.create(dai.node.StereoDepth)
xout_depth = pipeline.create(dai.node.XLinkOut)
xout_depth.setStreamName("disparity")

# Properties
left.setResolution(dai.ColorCameraProperties.SensorResolution.THE_800_P)
left.setCamera("left")

right.setResolution(dai.ColorCameraProperties.SensorResolution.THE_800_P)
right.setCamera("right")

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
stereo.setLeftRightCheck(lr_check)
stereo.setExtendedDisparity(extended_disparity)
stereo.setSubpixel(subpixel)


# Linking
left.isp.link(stereo.left)
right.isp.link(stereo.right)
if enableRectified:
    xout_rect_l = pipeline.create(dai.node.XLinkOut)
    xout_rect_r = pipeline.create(dai.node.XLinkOut)
    xout_rect_l.setStreamName("rectifiedLeft")
    xout_rect_r.setStreamName("rectifiedRight")
    stereo.rectifiedLeft.link(xout_rect_l.input)
    stereo.rectifiedRight.link(xout_rect_r.input)
stereo.disparity.link(xout_depth.input)

maxDisp = stereo.initialConfig.getMaxDisparity()

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
                    disp = (frame * (255.0 / maxDisp)).astype(np.uint8)
                    disp = cv2.applyColorMap(disp, cv2.COLORMAP_JET)
                    cv2.imshow(q, disp)
                else:
                    cv2.imshow(q, frame)
        if cv2.waitKey(1) == ord('q'):
            break