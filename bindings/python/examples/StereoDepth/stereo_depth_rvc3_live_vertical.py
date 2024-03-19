#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import argparse

#run examples/install_requirements.py -sdai

calibJsonFile = str((Path(__file__).parent / Path('./depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to calibration file in json", default=calibJsonFile)
args = parser.parse_args()

calibData = dai.CalibrationHandler(args.calibJsonFile)

# Create pipeline
pipeline = dai.Pipeline()
pipeline.setCalibrationData(calibData)

monoVertical = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
xoutVertical = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutRectifiedLeft = pipeline.create(dai.node.XLinkOut)
xoutRectifiedRight = pipeline.create(dai.node.XLinkOut)
xoutDisparity = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

xoutVertical.setStreamName("vertical")
xoutRight.setStreamName("right")
xoutRectifiedLeft.setStreamName("rectified_left")
xoutRectifiedRight.setStreamName("rectified_right")
xoutDisparity.setStreamName("disparity")


# Define sources and outputs
monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoRight.out.link(stereo.left)
monoVertical.out.link(stereo.right)
stereo.syncedLeft.link(xoutVertical.input)
stereo.syncedRight.link(xoutRight.input)
stereo.disparity.link(xoutDisparity.input)
stereo.rectifiedLeft.link(xoutRectifiedLeft.input)
stereo.rectifiedRight.link(xoutRectifiedRight.input)
stereo.setVerticalStereo(True)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qLeft = device.getOutputQueue("vertical", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("disparity", 4, False)
    qRectifiedLeft = device.getOutputQueue("rectified_left", 4, False)
    qRectifiedRight = device.getOutputQueue("rectified_right", 4, False)

    while True:

        inLeft = qLeft.get()
        frameLeft = inLeft.getCvFrame()
        cv2.imshow("vertical", frameLeft)

        inRight = qRight.get()
        frameRight = inRight.getCvFrame()
        cv2.imshow("right", frameRight)

        inRLeft = qRectifiedLeft.get()
        frameRLeft = inRLeft.getCvFrame()
        cv2.imshow("rectified_left", frameRLeft)

        inRRight = qRectifiedRight.get()
        frameRRight = inRRight.getCvFrame()
        cv2.imshow("rectified_right", frameRRight)

        inDepth = qDepth.get()
        frameDepth = inDepth.getCvFrame()
        # cv2.imshow("disparity", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disp", disp)



        if cv2.waitKey(1) == ord('q'):
            break