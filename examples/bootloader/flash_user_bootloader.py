#!/usr/bin/env python3

import depthai as dai
import sys
import time

deviceInfos = dai.DeviceBootloader.getAllAvailableDevices()
if len(deviceInfos) == 0:
    print("No device found to flash. Exiting.")
    exit(-1)
else:
    for i, di in enumerate(deviceInfos):
        print(f'[{i}] {di.getMxId()} [{di.protocol.name}]', end='')
        if di.state == dai.XLinkDeviceState.X_LINK_BOOTLOADER:
            with dai.DeviceBootloader(di) as bl:
                print(f' current bootloader: {bl.getVersion()}', end='')
        print()
    selected = input(f'Which DepthAI device to flash User Bootloader for (Note: Only NETWORK supported) [0..{len(deviceInfos)-1}]: ')
    info = deviceInfos[int(selected)]

# Open DeviceBootloader and allow flashing bootloader
with dai.DeviceBootloader(info) as bl:
    print("User Bootloader version to flash:", bl.getVersion())

    # Create a progress callback lambda
    progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')

    print(f"Flashing User Bootloader...")
    startTime = time.monotonic()
    (res, message) = bl.flashUserBootloader(progress)
    if res:
        print("Flashing successful. Took", time.monotonic() - startTime, "seconds")
    else:
        print("Flashing failed:", message)
