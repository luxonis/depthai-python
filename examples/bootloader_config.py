#!/usr/bin/env python3

import depthai as dai
import sys

usage = False
read = True
clear = False
path = ''
if len(sys.argv) >= 2:
    op = sys.argv[1]
    if op == 'read':
        read = True
    elif op == 'flash':
        read = False
        if len(sys.argv) >= 3:
            path = sys.argv[2]
    elif op == 'clear':
        clear = True
        read = False
    else:
        usage = True
else:
    usage = True

if usage:
    print(f'Usage: {sys.argv[0]} [read/flash/clear] [flash: path/to/config/json]')
    exit(-1)

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if res:
    print(f'Found device with name: {info.desc.name}');
    with dai.DeviceBootloader(info) as bl:
        if read:
            print('Current flashed configuration')
            print(f'{bl.readConfigData()}')
        else:
            success = None
            error = None
            if clear:
                (success, error) = bl.flashConfigClear()
            else:
                if path == '':
                    (success, error) = bl.flashConfig(dai.DeviceBootloader.Config())
                else:
                    (success, error) = bl.flashConfigFile(path)
            if success:
                print('Successfully flashed bootloader configuration')
            else:
                print(f'Error flashing bootloader configuration: {error}')
else:
    print('No devices found')
