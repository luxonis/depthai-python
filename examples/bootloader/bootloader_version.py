#!/usr/bin/env python3

import depthai as dai

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if res == True:
    print(f'Found device with name: {info.desc.name}')
    bl = dai.DeviceBootloader(info)
    print(f'Version: {bl.getVersion()}')

    (ok, err, info) = bl.readApplicationInfo()
    if ok:
        print(f'Application, flashed: {info.hasApplication}, firmware version: {info.firmwareVersion}, application name: {info.applicationName}')
    else:
        print(f'Error reading application info: {err}')

    for mem in (dai.DeviceBootloader.Memory.FLASH, dai.DeviceBootloader.Memory.EMMC):
        (ok, err, info) = bl.getMemoryInfo(mem)
        if ok:
            print(f'Memory size: {info.size}, info: {info.info}')
        else:
            print(f'Error retrieving memory information: {err}')
else:
    print('No devices found')
