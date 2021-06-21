#!/usr/bin/env python3

import depthai as dai
import sys
import time

blType = dai.DeviceBootloader.Type.USB

if len(sys.argv) > 1:
    if sys.argv[1] == 'usb':
        blType = dai.DeviceBootloader.Type.USB
    elif sys.argv[1] == 'eth':
        blType = dai.DeviceBootloader.Type.ETHERNET
    else:
        print("Specify either 'usb' or 'eth' bootloader type")
        quit()

(res, info) = dai.DeviceBootloader.getFirstAvailableDevice()

bl = dai.DeviceBootloader(info)
progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')

print("=== Flashing", blType.name, "bootloader...")
startTime = time.monotonic()
(res, message) = bl.flashBootloader(blType, progress)
if res:
    print("Flashing successful. Took", time.monotonic() - startTime, "seconds")
else:
    print("Flashing failed:", message)
