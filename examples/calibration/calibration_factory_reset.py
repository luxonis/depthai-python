#!/usr/bin/env python3

import depthai as dai

# Connect device
with dai.Device(dai.OpenVINO.VERSION_2021_4, dai.UsbSpeed.HIGH) as device:

    try:
        device.factoryResetCalibration()
        print(f'Factory reset calibration OK')
    except Exception as ex:
        print(f'Factory reset calibration FAIL: {ex}')
