#!/usr/bin/env python3

import cv2
import depthai as dai
import time
import math

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
imu = pipeline.createIMU()
xlinkOut = pipeline.createXLinkOut()

xlinkOut.setStreamName("imu")

# enable ROTATION_VECTOR and RAW_GYROSCOPE at 400 hz rate
imu.enableIMUSensor([dai.IMUSensor.ROTATION_VECTOR], 400)

# Link plugins IMU -> XLINK
imu.out.link(xlinkOut.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    baseTs = time.monotonic()

    # Output queue for imu bulk packets
    imuQueue = device.getOutputQueue(name="imu", maxSize=50, blocking=False)
    baseTs = None
    while True:
        imuData = imuQueue.get()  # blocking call, will wait until a new data has arrived

        imuPackets = imuData.packets
        for imuPacket in imuPackets:
            gyroTs = imuPacket.rotationVector.timestamp.get()
            if baseTs is None:
                baseTs = gyroTs
            gyroTs = gyroTs - baseTs

            print(f"Gyro timestamp: {gyroTs} s")
            print(f"Quaternion: i: {imuPacket.rotationVector.i} j: {imuPacket.rotationVector.j} k: {imuPacket.rotationVector.k} real: {imuPacket.rotationVector.real} ")
            print(f"Accuracy (rad): {imuPacket.rotationVector.accuracy}")


        if cv2.waitKey(1) == ord('q'):
            break
