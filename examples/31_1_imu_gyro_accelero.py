#!/usr/bin/env python3

import cv2
import depthai as dai
import time
import math

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
imu = pipeline.createIMU()
xlinkOut = pipeline.createXLinkOut()
xlinkOut.setStreamName("imu")

sensorConfig = dai.IMUSensorConfig()
sensorConfig.reportIntervalUs = 2500 # 400hz
sensorConfig.sensorId = dai.IMUSensorId.IMU_ACCELEROMETER
imu.enableIMUSensor(sensorConfig)
sensorConfig.sensorId = dai.IMUSensorId.IMU_GYROSCOPE_CALIBRATED
imu.enableIMUSensor(sensorConfig)
# above this threshold packets will be sent in batch of X, if the host is not blocked
imu.setBatchReportThreshold(5)
# maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
# if lower or equal to batchReportThreshold then the sending is always blocking on device
imu.setMaxBatchReports(5)

# Link plugins CAM -> XLINK
imu.out.link(xlinkOut.input)


# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    baseTs = time.monotonic()
    device.startPipeline()

    # Output queue for imu bulk packets
    imuQueue = device.getOutputQueue(name="imu", maxSize=4, blocking=False)
    baseTs = None
    while True:
        imuPacket = imuQueue.get()  # blocking call, will wait until a new data has arrived

        imuDatas = imuPacket.imuDatas
        for imuData in imuDatas:
            acceleroTs = imuData.acceleroMeter.timestamp.getTimestamp()
            gyroTs = imuData.gyroscope.timestamp.getTimestamp()
            if baseTs is None:
                baseTs = acceleroTs if acceleroTs < gyroTs else gyroTs
            acceleroTs = acceleroTs - baseTs
            gyroTs = gyroTs - baseTs

            ffs = "{: .06f}"
            accelLength = math.sqrt(imuData.acceleroMeter.x**2 + imuData.acceleroMeter.y**2 + imuData.acceleroMeter.z**2)

            print(f"Accelero timestamp: {acceleroTs} s")
            print(f"Accelero: x: {ffs.format(imuData.acceleroMeter.x)} y: {ffs.format(imuData.acceleroMeter.y)} z: {ffs.format(imuData.acceleroMeter.z)}, length {ffs.format(accelLength)}")
            print(f"Gyro timestamp: {gyroTs} s")
            print(f"Gyro: x: {ffs.format(imuData.gyroscope.x)} y: {ffs.format(imuData.gyroscope.y)} z: {ffs.format(imuData.gyroscope.z)} ")

        if cv2.waitKey(1) == ord('q'):
            break
