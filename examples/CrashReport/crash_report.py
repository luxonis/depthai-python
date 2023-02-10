#!/usr/bin/env python3

import cv2
import depthai as dai
import json

# Connect to device and start pipeline
with dai.Device() as device:

    crashDump = device.getCrashDump()
    if len(crashDump.crashReports) == 0:
        print("There was no crash dump found on your device!")
    else:
        json = crashDump.serializeToJson()
        print(json)
        destPath = "crashDump.json"
        with open(destPath, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=4)
