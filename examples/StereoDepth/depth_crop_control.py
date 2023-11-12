#!/usr/bin/env python3

"""
This example shows usage of depth camera in crop mode with the possibility to move the crop.
Use 'WASD' in order to do it.
"""

import cv2
import depthai as dai
import numpy as np

# Step size ('W','A','S','D' controls)
stepSize = 0.02

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoRight = pipeline.create(dai.node.MonoCamera)
monoLeft = pipeline.create(dai.node.MonoCamera)
manip = pipeline.create(dai.node.ImageManip)
stereo = pipeline.create(dai.node.StereoDepth)

configIn = pipeline.create(dai.node.XLinkIn)
xout = pipeline.create(dai.node.XLinkOut)

configIn.setStreamName('config')
xout.setStreamName("depth")

# Crop range
topLeft = dai.Point2f(0.2, 0.2)
bottomRight = dai.Point2f(0.8, 0.8)

# Properties
monoRight.setCamera("right")
monoLeft.setCamera("left")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)

manip.initialConfig.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
manip.setMaxOutputFrameSize(monoRight.getResolutionHeight()*monoRight.getResolutionWidth()*3)
stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.setSubpixel(True)

# Linking
configIn.out.link(manip.inputConfig)
stereo.depth.link(manip.inputImage)
manip.out.link(xout.input)
monoRight.out.link(stereo.right)
monoLeft.out.link(stereo.left)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Queues
    q = device.getOutputQueue(xout.getStreamName(), maxSize=4, blocking=False)
    configQueue = device.getInputQueue(configIn.getStreamName())

    sendCamConfig = False

    while True:
        inDepth = q.get()
        depthFrame = inDepth.getFrame() # depthFrame values are in millimeters

        # Frame is transformed, the color map will be applied to highlight the depth info
        depth_downscaled = depthFrame[::4]
        if np.all(depth_downscaled == 0):
            min_depth = 0  # Set a default minimum depth value when all elements are zero
        else:
            min_depth = np.percentile(depth_downscaled[depth_downscaled != 0], 1)
        max_depth = np.percentile(depth_downscaled, 99)
        depthFrameColor = np.interp(depthFrame, (min_depth, max_depth), (0, 255)).astype(np.uint8)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

        # Frame is ready to be shown
        cv2.imshow("depth", depthFrameColor)

        # Update screen
        key = cv2.waitKey(10)
        if key == ord('q'):
            break
        elif key == ord('w'):
            if topLeft.y - stepSize >= 0:
                topLeft.y -= stepSize
                bottomRight.y -= stepSize
                sendCamConfig = True
        elif key == ord('a'):
            if topLeft.x - stepSize >= 0:
                topLeft.x -= stepSize
                bottomRight.x -= stepSize
                sendCamConfig = True
        elif key == ord('s'):
            if bottomRight.y + stepSize <= 1:
                topLeft.y += stepSize
                bottomRight.y += stepSize
                sendCamConfig = True
        elif key == ord('d'):
            if bottomRight.x + stepSize <= 1:
                topLeft.x += stepSize
                bottomRight.x += stepSize
                sendCamConfig = True

        # Send new config to camera
        if sendCamConfig:
            cfg = dai.ImageManipConfig()
            cfg.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
            configQueue.send(cfg)
            sendCamConfig = False
