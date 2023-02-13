#!/usr/bin/env python3

import cv2
import depthai as dai
from json import dump

# Connect to device and start pipeline
with dai.Device() as device:

    if device.hasCrashDump():
        crashDump = device.getCrashDump()

        json = crashDump.serializeToJson()
        print(json)
        
        destPath = "crashDump.json"
        with open(destPath, 'w', encoding='utf-8') as f:
            dump(json, f, ensure_ascii=False, indent=4)
    else:
        print("There was no crash dump found on your device!")

