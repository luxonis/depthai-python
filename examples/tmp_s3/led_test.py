#!/usr/bin/env python3

import depthai as dai
import time

# Create pipeline
pipeline = dai.Pipeline()

# Connect to device and start pipeline
with dai.Device() as device:
    print('Connected IR Drivers: ', device.getIrDrivers())

    curBrightness = 0
    while True:
        device.setIrFloodLightBrightness(curBrightness)
        device.setIrLaserDotProjectorBrightness(1000 - curBrightness)
        curBrightness = curBrightness + 10

        if curBrightness > 1000:
            curBrightness = 0
        time.sleep(0.003)
