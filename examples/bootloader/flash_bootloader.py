#!/usr/bin/env python3

import depthai as dai
import sys
import time

blType = dai.DeviceBootloader.Type.AUTO
if len(sys.argv) > 1:
    if sys.argv[1] == 'usb':
        blType = dai.DeviceBootloader.Type.USB
    elif sys.argv[1] == 'network':
        blType = dai.DeviceBootloader.Type.NETWORK
    else:
        print("Specify either 'usb' or 'network' bootloader type")
        exit()

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
    selected = input(f'Which OAK device to flash factory bootloader for [0..{len(deviceInfos)-1}]: ')
    info = deviceInfos[int(selected)]

hasBootloader = (info.state == dai.XLinkDeviceState.X_LINK_BOOTLOADER)
if hasBootloader:
    print("Warning! Flashing factory bootloader can potentially soft brick your device and should be done with caution.")
    print("Do not unplug your device while the bootloader is flashing.")
    print("Type 'y' and press enter to proceed, otherwise exits: ")
    if input() != 'y':
        print("Prompt declined, exiting...")
        exit(-1)

# Open DeviceBootloader and allow flashing bootloader
print(f"Booting latest bootloader first, will take a tad longer...")
with dai.DeviceBootloader(info, allowFlashingBootloader=True) as bl:
    print("Bootloader version to flash:", bl.getVersion())
    currentBlType = bl.getType()

    if blType == dai.DeviceBootloader.Type.AUTO:
        blType = currentBlType

    # Check if bootloader type is the same, if already booted by bootloader (not in USB recovery mode)
    if currentBlType != blType and hasBootloader:
        print(f"Are you sure you want to flash '{blType.name}' bootloader over current '{currentBlType.name}' bootloader?")
        print(f"Type 'y' and press enter to proceed, otherwise exits: ")
        if input() != 'y':
            print("Prompt declined, exiting...")
            exit(-1)

    try:
        # Clears out user bootloader
        configJson = bl.readConfigData()
        configJson["userBlSize"] = 0
        bl.flashConfigData(configJson)
    except:
        print('No config found, skipping erasing user bootloader')

    # Create a progress callback lambda
    progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')

    print(f"Flashing {blType.name} bootloader...")
    startTime = time.monotonic()
    (res, message) = bl.flashBootloader(dai.DeviceBootloader.Memory.FLASH, blType, progress)
    if res:
        print("Flashing successful. Took", time.monotonic() - startTime, "seconds")
    else:
        print("Flashing failed:", message)
