#!/usr/bin/env python3

import depthai as dai

devices = dai.Device.getAllConnectedDevices()

for device in devices:
    dai.XLinkConnection.bootBootloader(device)
