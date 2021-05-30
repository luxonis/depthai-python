#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse


# Connect Device
boardConfigFile = str((Path(__file__).parent / Path('models/BW1098OBC.json')).resolve().absolute())
calibBinaryFile = str((Path(__file__).parent / Path('models/depthai_v5.calib')).resolve().absolute())
calibBackupfile = str((Path(__file__).parent / Path('depthai_calib_backup.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('boardConfigFile', nargs='?', help="Path to board config file", default=boardConfigFile)
parser.add_argument('calibBinaryFile', nargs='?', help="Path to version 5 .calib file", default=calibBinaryFile)
args = parser.parse_args()

with dai.Device() as device:
    deviceCalib = device.readCalibration()
    deviceCalib.eepromToJsonFile(calibBackupfile)
    print("Calibration Data on the device is backed up at:")
    print(calibBackupfile)
    calibData = dai.CalibrationHandler(args.calibBinaryFile, args.boardConfigFile);
    status = device.flashCalibration(calibData)
    if status:
        print('Calibration write succesful')
    else:
        print('Calibration Flash Failed!!')