#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse


# Connect Device
calibJsonFile = str((Path(__file__).parent / Path('models/depthai_calib.json')).resolve().absolute())
calibBackupfile = str((Path(__file__).parent / Path('depthai_calib_backup.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to V6 calibration file in json", default=calibJsonFile)
args = parser.parse_args()

with dai.Device() as device:
    deviceCalib = device.readCalibration()
    deviceCalib.eepromToJsonFile(calibBackupfile)
    print("Calibration Data on the device is backed up at:")
    print(calibBackupfile)
    calibData = dai.CalibrationHandler(args.calibJsonFile);
    status = device.flashCalibration(calibData)
    if status:
        print('Calibration write succesfull')
    else:
        print('Calibration Flash Failed!!')