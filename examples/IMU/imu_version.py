#!/usr/bin/env python3

import depthai as dai

device = dai.Device()

imuType = device.getConnectedIMU()
imuFirmwareVersion = device.getIMUFirmwareVersion()
print(f"IMU type: {imuType}, firmware version: {imuFirmwareVersion}")
