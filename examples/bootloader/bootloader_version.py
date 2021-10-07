#!/usr/bin/env python3

import depthai as dai

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if res == True:
    print(f'Found device with name: {info.desc.name}')
    bl = dai.DeviceBootloader(info)
    print(f'Version: {bl.getVersion()}')
else:
    print('No devices found')
