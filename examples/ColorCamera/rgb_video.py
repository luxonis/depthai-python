#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutIsp = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("raw")
xoutIsp.setStreamName("depth")

# Properties
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_240X180)

# Linking
camRgb.raw.link(xoutRgb.input)
camRgb.isp.link(xoutIsp.input)

camRgb.initialControl.setManualExposure(100,100)

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
    qRgb = device.getOutputQueue(name="raw", maxSize=4, blocking=False)
    qIsp = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    while True:
        inRgb = qRgb.get()  # blocking call, will wait until a new data has arrived
        inIsp = qIsp.get()  # blocking call, will wait until a new data has arrived
        #print(inRgb.getType())
        #print(inRgb.getData())
        # Retrieve 'bgr' (opencv format) frame
        cv2.imshow("raw", inRgb.getFrame()*16)
        cv2.imshow("depth", inIsp.getFrame())

        if cv2.waitKey(1) == ord('q'):
            break