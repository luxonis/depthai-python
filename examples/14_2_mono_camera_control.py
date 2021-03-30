#!/usr/bin/env python3

"""
This example shows usage of mono camera in crop mode with the possibility to move the crop.
Use 'WASD' in order to do it.
"""


import cv2
import depthai as dai

stepSize = 0.02

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) camera
camRight = pipeline.createMonoCamera()
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
camRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
camLeft = pipeline.createMonoCamera()
camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)


topLeft = dai.Point2f(0.4, 0.4)
bottomRight = dai.Point2f(0.6, 0.6)

manipRight = pipeline.createImageManip()
manipRight.initialConfig.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
manipLeft = pipeline.createImageManip()
manipLeft.initialConfig.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
manipRight.setMaxOutputFrameSize(camRight.getResolutionHeight()*camRight.getResolutionWidth()*3)

# Control movement
controlIn = pipeline.createXLinkIn()
controlIn.setStreamName('control')
controlIn.out.link(manipRight.inputConfig)
controlIn.out.link(manipLeft.inputConfig)

# Linking with USB
camRight.out.link(manipRight.inputImage)
camLeft.out.link(manipLeft.inputImage)

# Create outputs
manipOutRight = pipeline.createXLinkOut()
manipOutRight.setStreamName("right")
manipRight.out.link(manipOutRight.input)

manipOutLeft = pipeline.createXLinkOut()
manipOutLeft.setStreamName("left")
manipLeft.out.link(manipOutLeft.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale frames
    qRight = device.getOutputQueue(manipOutRight.getStreamName(), maxSize=4, blocking=False)
    qLeft = device.getOutputQueue(manipOutLeft.getStreamName(), maxSize=4, blocking=False)
    controlQueue = device.getInputQueue(controlIn.getStreamName())

    def displayFrame(name, frame):
        cv2.imshow(name, frame)

    sendCamConfig = False

    while True:
        inRight = qRight.get()
        inLeft = qLeft.get()
        frameRight = inRight.getCvFrame()
        frameLeft = inLeft.getCvFrame()
        displayFrame("right", frameRight)
        displayFrame("left", frameLeft)

   
        
        # Update screen
        key = cv2.waitKey(1)
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


        if sendCamConfig:
            cfg = dai.ImageManipConfig()
            cfg.setCropRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y)
            controlQueue.send(cfg)
            sendCamConfig = False                