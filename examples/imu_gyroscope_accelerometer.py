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

# enable RAW_ACCELEROMETER and RAW_GYROSCOPE at 400 hz rate
imu.enableIMUSensor([dai.IMUSensor.RAW_ACCELEROMETER, dai.IMUSensor.RAW_GYROSCOPE], 400)

# Link plugins IMU -> XLINK
imu.out.link(xlinkOut.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    baseTs = time.monotonic()

    def timeDeltaToMilliS(delta) -> float:
        return delta.total_seconds()*1000

    # Output queue for imu bulk packets
    imuQueue = device.getOutputQueue(name="imu", maxSize=50, blocking=False)
    baseTs = None
    while True:
        imuData = imuQueue.get()  # blocking call, will wait until a new data has arrived

        imuPackets = imuData.packets
        for imuPacket in imuPackets:
            acceleroTs = imuPacket.rawAcceleroMeter.timestamp.get()
            gyroTs = imuPacket.rawGyroscope.timestamp.get()
            if baseTs is None:
                baseTs = acceleroTs if acceleroTs < gyroTs else gyroTs
            acceleroTs = timeDeltaToMilliS(acceleroTs - baseTs)
            gyroTs = timeDeltaToMilliS(gyroTs - baseTs)

            imup = "{: .06f}"
            tsp = "{: .03f}"
            accelLength = math.sqrt(imuPacket.rawAcceleroMeter.x**2 + imuPacket.rawAcceleroMeter.y**2 + imuPacket.rawAcceleroMeter.z**2)

            print(f"Accelerometer timestamp: {tsp.format(acceleroTs)} ms")
            print(f"Accelerometer [m/s^2]: x: {imup.format(imuPacket.rawAcceleroMeter.x)} y: {imup.format(imuPacket.rawAcceleroMeter.y)} z: {imup.format(imuPacket.rawAcceleroMeter.z)}, length {imup.format(accelLength)}")
            print(f"Gyroscope timestamp: {tsp.format(gyroTs)} ms")
            print(f"Gyroscope [rad/s]: x: {imup.format(imuPacket.rawGyroscope.x)} y: {imup.format(imuPacket.rawGyroscope.y)} z: {imup.format(imuPacket.rawGyroscope.z)} ")

        if cv2.waitKey(1) == ord('q'):
            break
