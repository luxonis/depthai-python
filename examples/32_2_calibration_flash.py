#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse


# Connect Device
calibJsonFile = str((Path(__file__).parent / Path('models/depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to V6 calibration file in json", default=calibJsonFile)
args = parser.parse_args()

print("The following code is going to replace the calibration data stored on your device. Proceed with Caution.")
input("Press Enter to continue... or Press ctrl + c to abort")

with dai.Device() as device:
    calibData = dai.CalibrationHandler(args.calibJsonFile);
    status = device.flashCalibration(calibData)
    if status:
        print('Calibration write succesfull')
    else:
        print('Calibration Flash Failed!!')