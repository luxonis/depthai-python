#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse

calibJsonFile = str((Path(__file__).parent / Path('../models/depthai_calib.json')).resolve().absolute())
calibBackUpFile = str((Path(__file__).parent / Path('depthai_calib_backup.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to V6 calibration file in json", default=calibJsonFile)
args = parser.parse_args()

# Connect device
with dai.Device() as device:

    deviceCalib = device.readCalibration()
    deviceCalib.eepromToJsonFile(calibBackUpFile)
    print("Calibration Data on the device is backed up at:")
    print(calibBackUpFile)
    calibData = dai.CalibrationHandler(args.calibJsonFile)

    status = device.flashCalibration(calibData)
    if status:
        print('Calibration Flash Successful')
    else:
        print('Calibration Flash Failed!!!')
