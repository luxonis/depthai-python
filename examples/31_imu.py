#!/usr/bin/env python3

import cv2
import depthai as dai
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
imu = pipeline.createIMU()
xlinkOut = pipeline.createXLinkOut()
xlinkOut.setStreamName("imu")

# Link plugins CAM -> XLINK
imu.out.link(xlinkOut.input)


# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    baseTs = time.monotonic()
    device.startPipeline()

    # Output queue for imu bulk packets
    imuQueue = device.getOutputQueue(name="imu", maxSize=4, blocking=False)

    while True:
        imuPacket = imuQueue.get()  # blocking call, will wait until a new data has arrived

        imuDatas = imuPacket.imuDatas
        for imuData in imuDatas:
            dur = imuData.ts.getTimestamp()
            # TODO substract base time
            print(f"Timestamp: {dur}")

            print(f"Accelero: {imuData.accelerometer.x} {imuData.accelerometer.y} {imuData.accelerometer.z} ")
            print(f"Gyro: {imuData.gyro.x} {imuData.gyro.y} {imuData.gyro.z} ")


        if cv2.waitKey(1) == ord('q'):
            break
