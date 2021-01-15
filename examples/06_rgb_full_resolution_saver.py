#!/usr/bin/env python3

import time
from pathlib import Path

import cv2
import depthai as dai
import numpy as np

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)

# Create output
xout_rgb = pipeline.createXLinkOut()
xout_rgb.setStreamName("rgb")
cam_rgb.video.link(xout_rgb.input)

# Pipeline defined, now the device is assigned and pipeline is started
device = dai.Device(pipeline)
device.startPipeline()

# Output queue will be used to get the rgb frames from the output defined above
q_rgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)

# Make sure the destination path is present before starting to store the examples
Path('06_data').mkdir(parents=True, exist_ok=True)

while True:
    in_rgb = q_rgb.get()  # blocking call, will wait until a new data has arrived
    # data is originally represented as a flat 1D array, it needs to be converted into HxW form
    shape = (in_rgb.getHeight() * 3 // 2, in_rgb.getWidth())
    frame_rgb = cv2.cvtColor(in_rgb.getData().reshape(shape), cv2.COLOR_YUV2BGR_NV12)
    # frame is transformed and ready to be shown
    cv2.imshow("rgb", frame_rgb)
    # after showing the frame, it's being stored inside a target directory as a PNG image
    cv2.imwrite(f"06_data/{int(time.time() * 10000)}.png", frame_rgb)

    if cv2.waitKey(1) == ord('q'):
        break
