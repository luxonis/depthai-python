#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import numpy as np
import time
import argparse

# Connect Device
with dai.Device() as device:

    calibData = device.readCalibration()
    M_rgb, width, height = np.array(calibData.getDefaultIntrinsics(dai.CameraBoardSocket.RGB))
    print("RGB Camera Default intrinsics...")
    print(M_rgb)
    print(width)
    print(height)

    M_left = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, 1280, 720))
    print("LEFT Camera resized intrinsics...")
    print(M_left)

    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    M_right = np.array(calibData.getCameraIntrinsics(calibData.getStereoRightCameraId(), 1280, 720))

    H_left = np.matmul(np.matmul(M_right, R1), np.linalg.inv(M_left))
    print("LEFT Camera stereo rectification matrix...")
    print(H_left)

    lr_extrinsics = np.array(calibData.getCameraExtrinsics(dai.CameraBoardSocket.LEFT, dai.CameraBoardSocket.RIGHT))
    print("Transformation matrix of where left Camera is W.R.T right Camera's optical center")
    print(lr_extrinsics)
