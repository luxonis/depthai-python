#!/usr/bin/env python3

from pathlib import Path
import depthai as dai
import argparse

calibJsonFile = str((Path(__file__).parent / Path('../models/depthai_calib.json')).resolve().absolute())
calibBackUpFile = str((Path(__file__).parent / Path('depthai_calib_backup.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to calibration file in json", default=calibJsonFile)
args = parser.parse_args()

# Connect device
with dai.Device(dai.OpenVINO.VERSION_UNIVERSAL, dai.UsbSpeed.HIGH) as device:

    deviceCalib = device.readCalibration()
    deviceCalib.eepromToJsonFile(calibBackUpFile)
    print("Calibration Data on the device is backed up at:")
    print(calibBackUpFile)
    calibData = dai.CalibrationHandler(args.calibJsonFile)

    try:
        device.flashCalibration2(calibData)
        print('Successfully flashed calibration')
    except Exception as ex:
        print(f'Failed flashing calibration: {ex}')
