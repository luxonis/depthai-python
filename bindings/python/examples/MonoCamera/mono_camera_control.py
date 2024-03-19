#!/usr/bin/env python3

"""
This example shows usage of mono camera in crop mode with the possibility to move the crop.
Uses 'WASD' controls to move the crop window, 'T' to trigger autofocus, 'IOKL,.' for manual exposure/focus:
  Control:      key[dec/inc]  min..max
  exposure time:     I   O      1..33000 [us]
  sensitivity iso:   K   L    100..1600
To go back to auto controls:
  'E' - autoexposure
"""

import cv2
import depthai as dai

# Step size ('W','A','S','D' controls)
stepSize = 0.02
# Manual exposure/focus set step
expStep = 500  # us
isoStep = 50

def clamp(num, v0, v1):
    return max(v0, min(num, v1))

sendCamConfig = False

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoRight = pipeline.create(dai.node.MonoCamera)
monoLeft = pipeline.create(dai.node.MonoCamera)
manipRight = pipeline.create(dai.node.ImageManip)
manipLeft = pipeline.create(dai.node.ImageManip)

controlIn = pipeline.create(dai.node.XLinkIn)
configIn = pipeline.create(dai.node.XLinkIn)
manipOutRight = pipeline.create(dai.node.XLinkOut)
manipOutLeft = pipeline.create(dai.node.XLinkOut)

controlIn.setStreamName('control')
configIn.setStreamName('config')
manipOutRight.setStreamName("right")
manipOutLeft.setStreamName("left")

# Crop range
topLeft = dai.Point2f(0.2, 0.2)
bottomRight = dai.Point2f(0.8, 0.8)

# Properties
monoRight.setCamera("right")
monoLeft.setCamera("left")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
manipRight.initialConfig.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
manipLeft.initialConfig.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
manipRight.setMaxOutputFrameSize(monoRight.getResolutionHeight()*monoRight.getResolutionWidth()*3)

# Linking
monoRight.out.link(manipRight.inputImage)
monoLeft.out.link(manipLeft.inputImage)
controlIn.out.link(monoRight.inputControl)
controlIn.out.link(monoLeft.inputControl)
configIn.out.link(manipRight.inputConfig)
configIn.out.link(manipLeft.inputConfig)
manipRight.out.link(manipOutRight.input)
manipLeft.out.link(manipOutLeft.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues will be used to get the grayscale frames
    qRight = device.getOutputQueue(manipOutRight.getStreamName(), maxSize=4, blocking=False)
    qLeft = device.getOutputQueue(manipOutLeft.getStreamName(), maxSize=4, blocking=False)
    configQueue = device.getInputQueue(configIn.getStreamName())
    controlQueue = device.getInputQueue(controlIn.getStreamName())

    # Defaults and limits for manual focus/exposure controls
    expTime = 20000
    expMin = 1
    expMax = 33000

    sensIso = 800
    sensMin = 100
    sensMax = 1600

    while True:
        inRight = qRight.get()
        inLeft = qLeft.get()
        cv2.imshow("right", inRight.getCvFrame())
        cv2.imshow("left", inLeft.getCvFrame())

        # Update screen (1ms pooling rate)
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('e'):
            print("Autoexposure enable")
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureEnable()
            controlQueue.send(ctrl)
        elif key in [ord('i'), ord('o'), ord('k'), ord('l')]:
            if key == ord('i'): expTime -= expStep
            if key == ord('o'): expTime += expStep
            if key == ord('k'): sensIso -= isoStep
            if key == ord('l'): sensIso += isoStep
            expTime = clamp(expTime, expMin, expMax)
            sensIso = clamp(sensIso, sensMin, sensMax)
            print("Setting manual exposure, time:", expTime, "iso:", sensIso)
            ctrl = dai.CameraControl()
            ctrl.setManualExposure(expTime, sensIso)
            controlQueue.send(ctrl)
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
