#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import argparse
from numba import njit, prange

import numpy as np

MAX_LUT_SIZE = 96 * 32 * 2
USE_CAMERAS = False
USE_DEPTH = True
NV12_OUTPUT = True
@njit(parallel=True)
def depthToNv12(inDepth, lutR, lutG, lutB, disp2DepthMultiplier:float):

    height = inDepth.shape[0]
    width = inDepth.shape[1]

    y_plane = np.zeros((height, width), dtype=np.uint8)  # For Y output
    uv_plane = np.zeros((height//2, width), dtype=np.uint8)  # For NV12 output

    lutY = np.zeros(MAX_LUT_SIZE, dtype=np.uint8)
    lutU = np.zeros(MAX_LUT_SIZE, dtype=np.uint8)
    lutV = np.zeros(MAX_LUT_SIZE, dtype=np.uint8)

    for i in range(MAX_LUT_SIZE):
        r = lutR[i]
        g = lutG[i]
        b = lutB[i]

        _y = int(0.299 * r + 0.587 * g + 0.114 * b)
        y = min(max(_y, 0), 255)
        lutY[i] = y

        _u = int(-0.14713 * r - 0.28886 * g + 0.436 * b) + 128
        _v = int(0.615 * r - 0.51499 * g - 0.10001 * b) + 128

        u = min(max(_u, 0), 255)
        v = min(max(_v, 0), 255)

        lutU[i] = u
        lutV[i] = v


    for i in range(height):
        for j in range(0, width, 2):  # Processing two pixels at a time
            # Process first pixel
            px1 = inDepth[i, j]
            if disp2DepthMultiplier == 0:
                disparity1 = px1
            else:
                disparity1 = 0 if px1 == 0 else int(disp2DepthMultiplier / px1 + 0.5)
            disparity1 = 0 if disparity1 >= MAX_LUT_SIZE else disparity1

            y1 = lutY[disparity1]
            y_plane[i, j] = y1  # Storing Y component

            px2 = inDepth[i, j + 1]
            if disp2DepthMultiplier == 0:
                disparity2 = px2
            else:
                disparity2 = 0 if px2 == 0 else int(disp2DepthMultiplier / px2 + 0.5)
            disparity2 = 0 if disparity2 >= MAX_LUT_SIZE else disparity2

            y2 = lutY[disparity2]
            y_plane[i, j + 1] = y2  # Storing Y component

            # Compute U and V for NV12 format
            if i % 2 == 0:
                _u1, _v1 = np.uint8(lutU[disparity1]), np.uint8(lutV[disparity1])
                _u2, _v2 = np.uint8(lutU[disparity2]), np.uint8(lutV[disparity2])

                u = (_u1 + _u2) // 2
                v = (_v1 + _v2) // 2

                uv_plane[i//2, j] = u
                uv_plane[i//2, j + 1] = v

    nv12_combined = np.vstack((y_plane, uv_plane))

    return nv12_combined

@njit(parallel=True)
def depthToRgb(inDepth, lutR, lutG, lutB, disp2DepthMultiplier):

    height, width = inDepth.shape

    # Initialize output arrays for R, G, B channels
    outEncodedP1 = np.zeros((height, width), dtype=np.uint8)  # For R channel
    outEncodedP2 = np.zeros((height, width), dtype=np.uint8)  # For G channel
    outEncodedP3 = np.zeros((height, width), dtype=np.uint8)  # For B channel

    for i in range(height):
        for j in range(width):
            px = inDepth[i, j]

            if disp2DepthMultiplier == 0:
                disparity = px
            else:
                # Compute disparity
                disparity = int(disp2DepthMultiplier / px + 0.5) if px != 0 else 0
            disparity = 0 if disparity >= MAX_LUT_SIZE else disparity

            # Lookup RGB values
            r = lutR[disparity]
            g = lutG[disparity]
            b = lutB[disparity]

            # Assign to output arrays
            outEncodedP1[i, j] = r
            outEncodedP2[i, j] = g
            outEncodedP3[i, j] = b

    rgb_combined = np.dstack((outEncodedP1, outEncodedP2, outEncodedP3))

    return rgb_combined

datasetDefault = str((Path(__file__).parent / Path('../models/dataset')).resolve().absolute())
if not Path(datasetDefault).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

left = datasetDefault + '/' + '0' + '/' + 'in_left' + '.png'
right = datasetDefault + '/' + '0' + '/' + 'in_right' + '.png'


inW = 1280
inH = 800

leftImg = cv2.imread(left, cv2.IMREAD_GRAYSCALE)
rightImg = cv2.imread(right, cv2.IMREAD_GRAYSCALE)
leftImg = cv2.resize(leftImg, (inW, inH), interpolation = cv2.INTER_AREA)
rightImg = cv2.resize(rightImg, (inW, inH), interpolation = cv2.INTER_AREA)

width = leftImg.shape[1]
height = leftImg.shape[0]


# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs

leftOut = None
rightOut = None
if USE_CAMERAS:
    colorLeft = pipeline.create(dai.node.ColorCamera)
    colorLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
    colorLeft.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
    colorRight = pipeline.create(dai.node.ColorCamera)
    colorRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
    colorRight.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
    colorLeft.setVideoSize(inW, inH)
    colorRight.setVideoSize(inW, inH)
    leftOut = colorLeft.video
    rightOut = colorRight.video
else:
    colorLeft = pipeline.create(dai.node.XLinkIn)
    colorRight = pipeline.create(dai.node.XLinkIn)
    leftOut = colorLeft.out
    rightOut = colorRight.out
    colorLeft.setStreamName("inLeft")
    colorRight.setStreamName("inRight")
    colorLeft.setMaxDataSize(width * height)
    colorRight.setMaxDataSize(width * height)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDepth.setStreamName("depth")


# Linking
leftOut.link(stereo.left)
rightOut.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.rectifiedRight.link(xoutRight.input)
stereo.setInputResolution(width,height) # set input resolution specifically

stereo.setRectification(False) #disable rectification, frames are pre-rectified

stereo.setLeftRightCheck(False)
stereo.setSubpixel(True)
stereo.setSubpixelFractionalBits(5)

stereo.setConfidenceThreshold(230)

depthEncoder = pipeline.create(dai.node.DepthEncoder)

if USE_DEPTH:
    stereo.depth.link(depthEncoder.input)
    stereo.depth.link(xoutDepth.input)
else:
    stereo.disparity.link(depthEncoder.input)
    stereo.disparity.link(xoutDepth.input)

depthEncoderOut = pipeline.create(dai.node.XLinkOut)
depthEncoderOut.setStreamName("depthEnc")

depthEncoder.output.link(depthEncoderOut.input)

lutR = np.random.randint(0, 256, 6144, dtype=np.uint8)
lutG = np.random.randint(0, 256, 6144, dtype=np.uint8)
lutB = np.random.randint(0, 256, 6144, dtype=np.uint8)

depthEncoder.setLut(lutR, lutG, lutB)
# depthEncoder.setHueLut(0, 96 * 32, 1, 0.45)

depthEncoder.setNumShaves(1)

outType = dai.ImgFrame.Type.NV12 if NV12_OUTPUT else dai.ImgFrame.Type.RGB888p

depthEncoder.setOutputType(outType)
# depthEncoder.setOutputType(dai.ImgFrame.Type.RGB888p)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    if not USE_CAMERAS:
        qInLeft = device.getInputQueue("inLeft")
        qInRight = device.getInputQueue("inRight")
    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    qDepth = device.getOutputQueue("depth", 4, False)

    qEncoded = device.getOutputQueue("depthEnc", 4, False)

    cnt = 0

    while True:
        if not USE_CAMERAS:
            data = cv2.resize(leftImg, (width, height), interpolation = cv2.INTER_AREA)

            data = data.reshape(height*width)
            img = dai.ImgFrame()
            img.setData(data)
            img.setInstanceNum(1)
            img.setType(dai.ImgFrame.Type.RAW8)
            img.setWidth(width)
            img.setHeight(height)
            # print("left send")
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

        data = np.array(inDepth.getData())

        count = np.count_nonzero(data % 32 == 0)
        # print("Non multiples of 32 in disparity: ", count, " out of ", len(data), " (", count/len(data)*100, "%)")

        frameDepth = inDepth.getCvFrame()
        # cv2.imshow("depth", frameDepth)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disp", disp)

        cnt+=1


        encoded = qEncoded.get()
        print(len(encoded.getData()))

        encodedFrame = encoded.getCvFrame()
        cv2.imshow("encoded", encodedFrame)


        disp2DepthMultiplier = inDepth.get().disp2DepthMultiplier

        print(disp2DepthMultiplier)

        if outType == dai.ImgFrame.Type.NV12:
            encoded_depth_raw = depthToNv12(frameDepth, lutR, lutG, lutB, disp2DepthMultiplier)
            # Convert NV12 (YUV420) to BGR for visualization
            encoded_depth_host = cv2.cvtColor(encoded_depth_raw, cv2.COLOR_YUV2BGR_NV12)

        else:
            encoded_depth_raw = depthToRgb(frameDepth, lutR, lutG, lutB, disp2DepthMultiplier)
            bgr_array = encoded_depth_raw[:, :, [2, 1, 0]]

            encoded_depth_host = bgr_array #todo

        cv2.imshow("host_encoded", encoded_depth_host)

        diff = cv2.absdiff(encoded_depth_host, encodedFrame)
        cv2.imshow("diff", diff)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
