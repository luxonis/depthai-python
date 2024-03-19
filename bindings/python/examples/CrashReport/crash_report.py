#!/usr/bin/env python3

import cv2
import depthai as dai
from json import dump
from os.path import exists

# Connect to device and start pipeline
with dai.Device() as device:

    if device.hasCrashDump():
        crashDump = device.getCrashDump()
        commitHash = crashDump.depthaiCommitHash
        deviceId = crashDump.deviceId

        json = crashDump.serializeToJson()
        
        i = -1
        while True:
            i += 1
            destPath = "crashDump_" + str(i) + "_" + deviceId + "_" + commitHash + ".json"
            if exists(destPath):
                continue

            with open(destPath, 'w', encoding='utf-8') as f:
                dump(json, f, ensure_ascii=False, indent=4)

            print("Crash dump found on your device!")
            print(f"Saved to {destPath}")
            print("Please report to developers!")
            break
    else:
        print("There was no crash dump found on your device!")

