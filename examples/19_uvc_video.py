#!/usr/bin/env python3

import depthai as dai
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setBoardSocket(dai.CameraBoardSocket.RGB)
cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
cam_rgb.setInterleaved(False)

# Create an UVC (USB Video Class) output node
uvc = pipeline.createUVC()
cam_rgb.video.link(uvc.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    print("\nDevice started, please keep this process running")
    print("and open an UVC viewer. Example on Linux:")
    print("    guvcview -d /dev/video0")
    print("\nTo close: Ctrl+C")

    while True:
        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            break
