#!/usr/bin/env python3

import depthai as dai
import numpy as np
import sys
from pathlib import Path

# Connect Device
with dai.Device() as device:
    calibFile = str((Path(__file__).parent / Path(f"calib_{device.getMxId()}.json")).resolve().absolute())
    if len(sys.argv) > 1:
        calibFile = sys.argv[1]

    calibData = device.readCalibration()
    calibData.eepromToJsonFile(calibFile)

    M_rgb, width, height = np.array(calibData.getDefaultIntrinsics(dai.CameraBoardSocket.RGB))
    print("RGB Camera Default intrinsics...")
    print(M_rgb)
    print(width)
    print(height)

    M_rgb, width, height = np.array(calibData.getDefaultIntrinsics(dai.CameraBoardSocket.RGB))

    M_left = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, 1280, 720))
    print("LEFT Camera resized intrinsics...")
    print(M_left)

    print(f"RGB FOV {calibData.getFov(dai.CameraBoardSocket.RGB)}, Mono FOV {calibData.getFov(dai.CameraBoardSocket.LEFT)}")

    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    M_right = np.array(calibData.getCameraIntrinsics(calibData.getStereoRightCameraId(), 1280, 720))

    H_left = np.matmul(np.matmul(M_right, R1), np.linalg.inv(M_left))
    print("LEFT Camera stereo rectification matrix...")
    print(H_left)

    H_right = np.matmul(np.matmul(M_right, R1), np.linalg.inv(M_right))
    print("RIGHT Camera stereo rectification matrix...")
    print(H_right)

    lr_extrinsics = np.array(calibData.getCameraExtrinsics(dai.CameraBoardSocket.LEFT, dai.CameraBoardSocket.RIGHT))
    print("Transformation matrix of where left Camera is W.R.T right Camera's optical center")
    print(lr_extrinsics)

    l_rgb_extrinsics = np.array(calibData.getCameraExtrinsics(dai.CameraBoardSocket.LEFT, dai.CameraBoardSocket.RGB))
    print("Transformation matrix of where left Camera is W.R.T RGB Camera's optical center")
    print(l_rgb_extrinsics)

