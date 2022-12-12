#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
cam = pipeline.create(dai.node.Camera)
xoutRgb = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("camera")

# Properties
cam.setPreviewSize(300, 300)
cam.setInterleaved(False)
cam.setColorOrder(dai.CameraProperties.ColorOrder.RGB)
cam.setBoardSocket(dai.CameraBoardSocket.LEFT)

# Linking
cam.isp.link(xoutRgb.input)

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
    qRgb = device.getOutputQueue(name="camera", maxSize=4, blocking=False)

    while True:
        inRgb = qRgb.get()  # blocking call, will wait until a new data has arrived

        # Retrieve 'bgr' (opencv format) frame
        cv2.imshow("camera", inRgb.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
