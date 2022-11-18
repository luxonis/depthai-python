#!/usr/bin/env python3

import cv2
import depthai as dai

def printSystemInformation(info: dai.SystemInformationS3):
    m = 1024 * 1024 # MiB
    print(f"Ddr used / total - {info.ddrMemoryUsage.used / m:.2f} / {info.ddrMemoryUsage.total / m:.2f} MiB")
    t = info.chipTemperature
    print(f"Chip temperature - average: {t.average:.2f}, css: {t.css:.2f}, mss: {t.mss:.2f}, nce: {t.nce:.2f}, soc: {t.soc:.2f}")
    print(f"Cpu usage average: {info.cpuAvgUsage.average * 100:.2f}%")
    print("Cpu usage per core: ")
    for i, cpuUsage in enumerate(info.cpuUsages):
        print(f"Core {i}: {cpuUsage.average * 100:.2f}%")
    print("----------------------------------------")

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
sysLog = pipeline.create(dai.node.SystemLogger)
linkOut = pipeline.create(dai.node.XLinkOut)

linkOut.setStreamName("sysinfo")

# Properties
sysLog.setRate(1)  # 1 Hz

# Linking
sysLog.out.link(linkOut.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the system info
    qSysInfo = device.getOutputQueue(name="sysinfo", maxSize=4, blocking=False)

    while True:
        sysInfo : dai.SystemInformationS3 = qSysInfo.get()  # Blocking call, will wait until a new data has arrived
        printSystemInformation(sysInfo)
