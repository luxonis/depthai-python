#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse


# Connect Device
boardConfigFile = str((Path(__file__).parent / Path('models/BW1098OBC.json')).resolve().absolute())
calibBinaryFile = str((Path(__file__).parent / Path('models/depthai_v5.calib')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('boardConfigFile', nargs='?', help="Path to board config file", default=boardConfigFile)
parser.add_argument('calibBinaryFile', nargs='?', help="Path to version 5 .calib file", default=calibBinaryFile)
args = parser.parse_args()

print("The following code is going to replace the calibration data stored on your device. Proceed with Caution.")
input("Press Enter to continue... or Press ctrl + c to abort")

with dai.Device() as device:
    calibData = dai.CalibrationHandler(args.calibBinaryFile, args.boardConfigFile);
    status = device.flashCalibration(calibData)
    if status:
        print('Calibration write succesfull')
    else:
        print('Calibration Flash Failed!!')