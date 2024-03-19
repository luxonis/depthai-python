#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
tofCam = pipeline.create(dai.node.ColorCamera)
tofProcess = pipeline.create(dai.node.ToF)

xoutPasstrough = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutVideo = pipeline.create(dai.node.XLinkOut)

xoutPasstrough.setStreamName("passtroughTof")
xoutDepth.setStreamName("depth")
xoutVideo.setStreamName("video")

# Properties
tofCam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_240X180)
tofCam.setBoardSocket(dai.CameraBoardSocket.CAM_F)

# Linking
tofCam.raw.link(tofProcess.inputRaw)
tofProcess.passthroughInputRaw.link(xoutPasstrough.input)
tofCam.isp.link(xoutVideo.input)

xoutPasstrough.input.setBlocking(False)
xoutPasstrough.input.setQueueSize(1)

tofProcess.depth.link(xoutDepth.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    print('Connected cameras:', device.getConnectedCameraFeatures())
    # Print out usb speed
    print('Usb speed:', device.getUsbSpeed().name)
    # Bootloader version
    if device.getBootloaderVersion() is not None:
        print('Bootloader version:', device.getBootloaderVersion())
    # Device name
    print('Device name:', device.getDeviceName())

    # Output queue will be used to get the rgb frames from the output defined above
    qPassthrough = device.getOutputQueue(name="passtroughTof", maxSize=4, blocking=False)
    video = device.getOutputQueue(name="video", maxSize=4, blocking=False)
    qDepth = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    while True:
        videoIn = video.get()
        inPassthrough = qPassthrough.get()  # blocking call, will wait until a new data has arrived
        inDepth = qDepth.get()  # blocking call, will wait until a new data has arrived

        # Preview
        cv2.imshow("ToF raw", (inPassthrough.getFrame()).clip(0, 255).astype(np.uint8))
        name = "ToF Depth"
        if inDepth.getSequenceNum() % 2 :
            name = "ToF Amplitude / Confidence"
        cv2.imshow(name, (inDepth.getFrame()).clip(0, 255).astype(np.uint8))

        if cv2.waitKey(1) == ord('q'):
            break
