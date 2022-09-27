#!/usr/bin/env python3

import depthai as dai

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if res == True:
    print(f'Found device with name: {info.name}')
    bl = dai.DeviceBootloader(info)
    print(f'Version: {bl.getVersion()}')

    supportedMemTypes = [dai.DeviceBootloader.Memory.FLASH, dai.DeviceBootloader.Memory.EMMC]
    if bl.getType() == dai.DeviceBootloader.Type.USB:
        print("USB Bootloader - supports only Flash memory")
        supportedMemTypes = [dai.DeviceBootloader.Memory.FLASH];
    else:
        print(f"NETWORK Bootloader, is User Bootloader: {bl.isUserBootloader()}")

    try:
        for mem in supportedMemTypes:
            memInfo = bl.getMemoryInfo(mem)
            if memInfo.available:
                print(f"Memory '{mem}' size: {memInfo.size}, info: {memInfo.info}")
                appInfo = bl.readApplicationInfo(mem)
                if appInfo.hasApplication:
                    print(f"Application name: {appInfo.applicationName}, firmware version: {appInfo.firmwareVersion}")
            else:
                print(f"Memory '{mem.name}' not available...")
    except Exception as ex:
        print(f"Couldn't retrieve memory details: {ex}")
else:
    print('No devices found')
