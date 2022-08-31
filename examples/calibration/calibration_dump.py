#!/usr/bin/env python3

import depthai as dai
import json

# Connect device
with dai.Device(dai.OpenVINO.VERSION_2021_4, dai.UsbSpeed.HIGH) as device:

    print(f'Is EEPROM available: {device.isEepromAvailable()}')

    # User calibration
    try:
        print(f'User calibration: {json.dumps(device.readCalibration2().eepromToJson(), indent=2)}')
    except Exception as ex:
        print(f'No user calibration: {ex}')

    # Factory calibration
    try:
        print(f'Factory calibration: {json.dumps(device.readFactoryCalibration().eepromToJson(), indent=2)}')
    except Exception as ex:
        print(f'No factory calibration: {ex}')

    print(f'User calibration raw: {json.dumps(device.readCalibrationRaw())}')
    print(f'Factory calibration raw: {json.dumps(device.readFactoryCalibrationRaw())}')
