#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import argparse

#run examples/install_requirements.py -sdai

calibJsonFile = str((Path(__file__).parent / Path('../models/depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to calibration file in json", default=calibJsonFile)
args = parser.parse_args()

calibData = dai.CalibrationHandler(args.calibJsonFile)

# Create pipeline
pipeline = dai.Pipeline()
pipeline.setCalibrationData(calibData)

monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDepth.setStreamName("depth")


# Define sources and outputs
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.depth.link(xoutDepth.input)
stereo.setInputResolution(1280,720)



# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("depth", 4, False)



    while True:

        inLeft = qLeft.get()
        frameLeft = inLeft.getCvFrame()
        cv2.imshow("left", frameLeft)

        inRight = qRight.get()
        frameRight = inRight.getCvFrame()
        cv2.imshow("right", frameRight)

        inDepth = qDepth.get()
        frameDepth = inDepth.getCvFrame()
        # cv2.imshow("depth", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disp", disp)



        if cv2.waitKey(1) == ord('q'):
            break