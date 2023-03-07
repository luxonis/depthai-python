#!/usr/bin/env python3

import depthai as dai

device = dai.Device()

imuVersion = device.getConnectedIMU()
imuFirmwareVersion = device.getIMUFirmwareVersion()
print(f"IMU version: {imuVersion}, firmware version: {imuFirmwareVersion}")
