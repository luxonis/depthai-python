#!/usr/bin/env python3

import cv2
import depthai as dai
import time
import math

device = dai.Device()

imuVersion = device.getConnectedIMU()
imuFirmwareVersion = device.getIMUFirmwareVersion()
embeddedIMUFirmwareVersion = device.getEmbeddedIMUFirmwareVersion()
print(f"IMU type: {imuVersion}, firmware version: {imuFirmwareVersion}, embedded firmware version: {embeddedIMUFirmwareVersion}")

print("Warning! Flashing IMU firmware can potentially soft brick your device and should be done with caution.")
print("Do not unplug your device while the IMU firmware is flashing.")
print("Type 'y' and press enter to proceed, otherwise exits: ")
if input() != 'y':
    print("Prompt declined, exiting...")
    exit(-1)

# Pipeline is defined, now we can connect to the device
with device:

    started = device.startIMUFirmwareUpdate()
    if not started:
        print("Couldn't start IMU firmware update")
        exit(1)

    while True:
        fwUpdateFinished, percentage = device.getIMUFirmwareUpdateStatus()
        print(f"IMU FW update status: {percentage:.1f}%")
        if fwUpdateFinished:
            if percentage == 100:
                print("Firmware update successful!")
            else: 
                print("Firmware update failed!")
            break
        time.sleep(1)
