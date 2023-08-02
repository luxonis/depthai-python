#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutRgb = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("rgb")

# Properties
camRgb.setPreviewSize(300, 300)
camRgb.setInterleaved(False)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.RGB)

# Linking
camRgb.preview.link(xoutRgb.input)

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
    qRgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)

    cv2.namedWindow("rgb", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("rgb", (640, 480))
    cv2.namedWindow("img3_16", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("img3_16", (640, 480))
    cv2.namedWindow("color", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("color", (640, 480))
    while True:
        inRgb = qRgb.get()  # blocking call, will wait until a new data has arrived

        # Retrieve 'bgr' (opencv format) frame
        im = inRgb.getFrame()

        im2 = im.reshape(192, 256, 2)

        rgb = cv2.cvtColor(im2, cv2.COLOR_YUV2BGR_YUYV)

        cv2.imshow("rgb", rgb)

        img3 = im.view(np.uint16).reshape(192,256)
        img3_16 = img3 * 256 # Now we get the good values in 16 bit format
        cv2.imshow("img3_16", img3_16)

        img3 = img3.astype(np.uint8)
        color = cv2.applyColorMap(img3, cv2.COLORMAP_HOT)

        cv2.imshow("color", color)


        if cv2.waitKey(1) == ord('q'):
            break
