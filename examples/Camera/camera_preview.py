#!/usr/bin/env python3

import cv2
import depthai as dai

# Connect to device and start pipeline
with dai.Device() as device:
    print('Connected cameras:', device.getConnectedCameraFeatures())
    # Print out usb speed
    print('Usb speed:', device.getUsbSpeed().name)

    # Create pipeline
    pipeline = dai.Pipeline()
    cams = device.getConnectedCameraFeatures()
    streams = []
    for cam in cams:
        c = pipeline.create(dai.node.Camera)
        x = pipeline.create(dai.node.XLinkOut)
        c.preview.link(x.input)
        stream = str(cam.socket)
        x.setStreamName(stream)
        streams.append(stream)

    # Bootloader version
    if device.getBootloaderVersion() is not None:
        print('Bootloader version:', device.getBootloaderVersion())
    # Device name
    print('Device name:', device.getDeviceName())

    # Start pipeline
    device.startPipeline(pipeline)
    # Clear queue events
    device.getQueueEvents()
    while True:

        queueName = device.getQueueEvent(streams)
        message = device.getOutputQueue(queueName).get()
        # Display arrived frames
        if type(message) == dai.ImgFrame:
            cv2.imshow(queueName, message.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
