#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path

#run examples/install_requirements.py -sdai

datasetDefault = str((Path(__file__).parent / Path('../models/dataset')).resolve().absolute())
if not Path(datasetDefault).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

left = datasetDefault + '/' + '0' + '/' + 'in_left' + '.png'
right = datasetDefault + '/' + '0' + '/' + 'in_right' + '.png'

leftImg = cv2.imread(left, cv2.IMREAD_GRAYSCALE)
rightImg = cv2.imread(right, cv2.IMREAD_GRAYSCALE)

cv2.imshow("leftImg", leftImg)
cv2.imshow("rightImg", rightImg)
cv2.waitKey(1) 

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.XLinkIn)
monoRight = pipeline.create(dai.node.XLinkIn)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

monoLeft.setStreamName("inLeft")
monoRight.setStreamName("inRight")
xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDepth.setStreamName("depth")

monoLeft.setMaxDataSize(1280 * 720)
monoRight.setMaxDataSize(1280 * 720)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.depth.link(xoutDepth.input)
stereo.setInputResolution(1280,720)

stereo.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_LEFT)
# stereo.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_RIGHT)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qInLeft = device.getInputQueue("inLeft")
    qInRight = device.getInputQueue("inRight")
    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("depth", 4, False)

    cnt = 0

    width = 1280
    height = 720

    while True:

        data = cv2.resize(leftImg, (width, height), interpolation = cv2.INTER_AREA)
        data = data.reshape(height*width)
        img = dai.ImgFrame()
        img.setData(data)
        img.setInstanceNum(1)
        img.setType(dai.ImgFrame.Type.RAW8)
        img.setWidth(width)
        img.setHeight(height)
        qInLeft.send(img)

        data = cv2.resize(rightImg, (width, height), interpolation = cv2.INTER_AREA)
        data = data.reshape(height*width)
        img = dai.ImgFrame()
        img.setData(data)
        img.setInstanceNum(2)
        img.setType(dai.ImgFrame.Type.RAW8)
        img.setWidth(width)
        img.setHeight(height)
        qInRight.send(img)

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

        cnt+=1


        if cv2.waitKey(1) == ord('q'):
            break