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
sensorConfig.sensorId = dai.IMUSensorId.IMU_ROTATION_VECTOR
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
            gyroTs = imuData.rotationVector.timestamp.getTimestamp()
            if baseTs is None:
                baseTs = gyroTs
            gyroTs = gyroTs - baseTs

            print(f"Gyro timestamp: {gyroTs} s")
            print(f"Quaternion: i: {imuData.rotationVector.i} j: {imuData.rotationVector.j} k: {imuData.rotationVector.k} real: {imuData.rotationVector.real} ")
            print(f"Accuracy (rad): {imuData.rotationVector.accuracy}")


        if cv2.waitKey(1) == ord('q'):
            break
