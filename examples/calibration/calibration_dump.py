#!/usr/bin/env python3

import depthai as dai

# Connect device
with dai.Device(dai.OpenVINO.VERSION_2021_4, dai.UsbSpeed.HIGH) as device:

    print(f'Is EEPROM available: {device.isEepromAvailable()}')

    # User calibration
    try:
        print(f'User calibration: {device.readCalibration2().eepromToJson()}')
    except Exception as ex:
        print('No user calibration: {ex}')

    # Factory calibration
    try:
        print(f'Factory calibration: {device.readFactoryCalibration().eepromToJson()}')
    except Exception as ex:
        print('No factory calibration: {ex}')
