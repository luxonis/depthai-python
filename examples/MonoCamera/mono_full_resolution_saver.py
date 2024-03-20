#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import time

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
monoRight = pipeline.create(dai.node.MonoCamera)
xoutRight = pipeline.create(dai.node.XLinkOut)

xoutRight.setStreamName("right")

# Properties
monoRight.setCamera("right")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoRight.out.link(xoutRight.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the grayscale frames from the output defined above
    qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)

    dirName = "mono_data"
    Path(dirName).mkdir(parents=True, exist_ok=True)

    while True:
        inRight = qRight.get()  # Blocking call, will wait until a new data has arrived
        # Data is originally represented as a flat 1D array, it needs to be converted into HxW form
        # Frame is transformed and ready to be shown
        cv2.imshow("right", inRight.getCvFrame())

        # After showing the frame, it's being stored inside a target directory as a PNG image
        cv2.imwrite(f"{dirName}/{int(time.time() * 1000)}.png", inRight.getFrame())

        if cv2.waitKey(1) == ord('q'):
            break
