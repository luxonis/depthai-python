#!/usr/bin/env python3

import time
from pathlib import Path

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - mono (grayscale) camera
cam_right = pipeline.createMonoCamera()
cam_right.setBoardSocket(dai.CameraBoardSocket.RIGHT)
cam_right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Create output
xout_right = pipeline.createXLinkOut()
xout_right.setStreamName("right")
cam_right.out.link(xout_right.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()
        
    # Output queue will be used to get the grayscale frames from the output defined above
    q_right = device.getOutputQueue(name="right", maxSize=4, blocking=False)

    # Make sure the destination path is present before starting to store the examples
    Path('07_data').mkdir(parents=True, exist_ok=True)

    while True:
        in_right = q_right.get()  # blocking call, will wait until a new data has arrived
        # data is originally represented as a flat 1D array, it needs to be converted into HxW form
        frame_right = in_right.getCvFrame()
        # frame is transformed and ready to be shown
        cv2.imshow("right", frame_right)
        # after showing the frame, it's being stored inside a target directory as a PNG image
        cv2.imwrite(f"07_data/{int(time.time() * 10000)}.png", frame_right)

        if cv2.waitKey(1) == ord('q'):
            break
