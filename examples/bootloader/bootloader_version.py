#!/usr/bin/env python3

import depthai as dai

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if res == True:
    print(f'Found device with name: {info.name}')
    bl = dai.DeviceBootloader(info)
    print(f'Version: {bl.getVersion()}')

    for mem in (dai.DeviceBootloader.Memory.FLASH, dai.DeviceBootloader.Memory.EMMC):
        memInfo = bl.getMemoryInfo(mem)
        if not memInfo.available: continue # Memory is not available, skip
        print(f"Memory '{mem}' size: {memInfo.size}, info: {memInfo.info}")

        appInfo = bl.readApplicationInfo(mem)
        if appInfo.hasApplication:
            print(f"Application name: {appInfo.applicationName}, firmware version: {appInfo.firmwareVersion}")
else:
    print('No devices found')
