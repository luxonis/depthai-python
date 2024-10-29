#!/usr/bin/env python3

import depthai as dai
import time

cfg = dai.Device.Config()
cfg.nonExclusiveMode = True

with dai.Device(cfg) as device:
    while not device.isClosed():
        print('CPU usage:',device.getLeonCssCpuUsage().average)
        time.sleep(1)