#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse
import numpy as np
import cv2

calibJsonFile = str((Path(__file__).parent / Path('../models/depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to calibration file in json", default=calibJsonFile)
args = parser.parse_args()

calibData = dai.CalibrationHandler(args.calibJsonFile)

# Create pipeline
pipeline = dai.Pipeline()
pipeline.setCalibrationData(calibData)

# Define sources and output
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutDepth.setStreamName("depth")

# MonoCamera
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setCamera("left")
# monoLeft.setFps(5.0)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setCamera("right")
# monoRight.setFps(5.0)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.depth.link(xoutDepth.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    while True:
        # blocking call, will wait until a new data has arrived
        inDepth = depthQueue.get()
        frame = inDepth.getFrame()

        # frame is ready to be shown
        cv2.imshow("depth", frame)

        if cv2.waitKey(1) == ord('q'):
            break
