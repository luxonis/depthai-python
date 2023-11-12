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

enableRectified = False

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
left = pipeline.create(dai.node.ColorCamera)
center = pipeline.create(dai.node.ColorCamera)
right = pipeline.create(dai.node.ColorCamera)
LC_depth = pipeline.create(dai.node.StereoDepth)
LR_depth = pipeline.create(dai.node.StereoDepth)
CR_depth = pipeline.create(dai.node.StereoDepth)

xout_LC = pipeline.create(dai.node.XLinkOut)
xout_LR = pipeline.create(dai.node.XLinkOut)
xout_CR = pipeline.create(dai.node.XLinkOut)

xout_LC.setStreamName("disparity_LC")
if enableRectified:
    xoutl_LC = pipeline.create(dai.node.XLinkOut)
    xoutr_LC = pipeline.create(dai.node.XLinkOut)
    xoutl_LC.setStreamName("rectifiedLeft_LC")
    xoutr_LC.setStreamName("rectifiedRight_LC")

xout_LR.setStreamName("disparity_LR")
if enableRectified:
    xoutl_LR = pipeline.create(dai.node.XLinkOut)
    xoutr_LR = pipeline.create(dai.node.XLinkOut)
    xoutl_LR.setStreamName("rectifiedLeft_LR")
    xoutr_LR.setStreamName("rectifiedRight_LR")

xout_CR.setStreamName("disparity_CR")
if enableRectified:
    xoutl_CR = pipeline.create(dai.node.XLinkOut)
    xoutr_CR = pipeline.create(dai.node.XLinkOut)
    xoutl_CR.setStreamName("rectifiedLeft_CR")
    xoutr_CR.setStreamName("rectifiedRight_CR")

# Properties
left.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
left.setCamera("left")
left.setIspScale(2, 3)

center.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
center.setBoardSocket(dai.CameraBoardSocket.CENTER)
center.setIspScale(2, 3)

right.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
right.setCamera("right")
right.setIspScale(2, 3)

LC_depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
LC_depth.initialConfig.setMedianFilter(dai.MedianFilter.MEDIAN_OFF)
LC_depth.setLeftRightCheck(lr_check)
LC_depth.setExtendedDisparity(extended_disparity)
LC_depth.setSubpixel(subpixel)

LR_depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
LR_depth.initialConfig.setMedianFilter(dai.MedianFilter.MEDIAN_OFF)
LR_depth.setLeftRightCheck(lr_check)
LR_depth.setExtendedDisparity(extended_disparity)
LR_depth.setSubpixel(subpixel)

CR_depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
CR_depth.initialConfig.setMedianFilter(dai.MedianFilter.MEDIAN_OFF)
CR_depth.setLeftRightCheck(lr_check)
CR_depth.setExtendedDisparity(extended_disparity)
CR_depth.setSubpixel(subpixel)

# Linking
# LC
left.isp.link(LC_depth.left)
center.isp.link(LC_depth.right)
LC_depth.disparity.link(xout_LC.input)
if enableRectified:
    LC_depth.rectifiedLeft.link(xoutl_LC.input)
    LC_depth.rectifiedRight.link(xoutr_LC.input)
# LR
left.isp.link(LR_depth.left)
right.isp.link(LR_depth.right)
LR_depth.disparity.link(xout_LR.input)
if enableRectified:
    LR_depth.rectifiedLeft.link(xoutl_LR.input)
    LR_depth.rectifiedRight.link(xoutr_LR.input)
# CR
center.isp.link(CR_depth.left)
right.isp.link(CR_depth.right)
CR_depth.disparity.link(xout_CR.input)
if enableRectified:
    CR_depth.rectifiedLeft.link(xoutl_CR.input)
    CR_depth.rectifiedRight.link(xoutr_CR.input)

maxDisp = LC_depth.initialConfig.getMaxDisparity()

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