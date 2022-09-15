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

monoLeft = pipeline.create(dai.node.MonoCamera)
monoVertical = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
xoutRectifiedVertical = pipeline.create(dai.node.XLinkOut)
xoutRectifiedRight = pipeline.create(dai.node.XLinkOut)
xoutRectifiedLeft = pipeline.create(dai.node.XLinkOut)
xoutDisparityVertical = pipeline.create(dai.node.XLinkOut)
xoutDisparityHorizontal = pipeline.create(dai.node.XLinkOut)
stereoVertical = pipeline.create(dai.node.StereoDepth)
stereoHorizontal = pipeline.create(dai.node.StereoDepth)
syncNode = pipeline.create(dai.node.Sync)

xoutRectifiedVertical.setStreamName("rectified_vertical")
xoutRectifiedRight.setStreamName("rectified_right")
xoutRectifiedLeft.setStreamName("rectified_left")
xoutDisparityVertical.setStreamName("disparity_vertical")
xoutDisparityHorizontal.setStreamName("disparity_horizontal")


# Define sources and outputs
monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoRight.out.link(syncNode.input1)
monoLeft.out.link(syncNode.input2)
monoVertical.out.link(syncNode.input3)

syncNode.output1.link(stereoVertical.left)
syncNode.output3.link(stereoVertical.right)
stereoVertical.disparity.link(xoutDisparityVertical.input)
stereoVertical.rectifiedLeft.link(xoutRectifiedVertical.input)
stereoVertical.rectifiedRight.link(xoutRectifiedRight.input)
stereoVertical.setVerticalStereo(True)

syncNode.output2.link(stereoHorizontal.left)
syncNode.output1.link(stereoHorizontal.right)
stereoHorizontal.disparity.link(xoutDisparityHorizontal.input)
stereoHorizontal.rectifiedLeft.link(xoutRectifiedLeft.input)
# stereoHorizontal.rectifiedRight.link(xoutRectifiedRight.input)
stereoHorizontal.setVerticalStereo(False)

stereoHorizontal.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_RIGHT)
stereoVertical.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_LEFT)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qDisparityHorizontal = device.getOutputQueue("disparity_horizontal", 4, False)
    qDisparityVertical = device.getOutputQueue("disparity_vertical", 4, False)
    qRectifiedVertical = device.getOutputQueue("rectified_vertical", 4, False)
    qRectifiedRight = device.getOutputQueue("rectified_right", 4, False)
    qRectifiedLeft = device.getOutputQueue("rectified_left", 4, False)

    while True:

        inRectifiedVertical = qRectifiedVertical.get()
        frameRVertical = inRectifiedVertical.getCvFrame()
        cv2.imshow("rectified_vertical", frameRVertical)

        inRectifiedRight = qRectifiedRight.get()
        frameRRight = inRectifiedRight.getCvFrame()
        cv2.imshow("rectified_right", frameRRight)

        inDisparityVertical = qDisparityVertical.get()
        frameDepth = inDisparityVertical.getCvFrame()
        # cv2.imshow("disparity", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disparity_vertical", disp)

        inRectifiedLeft = qRectifiedLeft.get()
        frameRLeft = inRectifiedLeft.getCvFrame()
        cv2.imshow("rectified_left", frameRLeft)

        inDisparityHorizontal = qDisparityHorizontal.get()
        frameDepth = inDisparityHorizontal.getCvFrame()
        # cv2.imshow("disparity", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disparity_horizontal", disp)


        if cv2.waitKey(1) == ord('q'):
            break