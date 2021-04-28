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
sensorConfig.reportIntervalUs = 2500
sensorConfig.sensorId = dai.IMUSensorId.IMU_ACCELEROMETER
imu.enableIMUSensor(sensorConfig)
sensorConfig.sensorId = dai.IMUSensorId.IMU_GYROSCOPE_CALIBRATED
imu.enableIMUSensor(sensorConfig)
# above this threshold packets will be sent in batch of X, if the host is not blocked
imu.setBatchReportThreshold(1)
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

    def timeDeltaToMilliS(delta) -> float:
        return delta.total_seconds()*1000

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
            acceleroTs = timeDeltaToMilliS(acceleroTs - baseTs)
            gyroTs = timeDeltaToMilliS(gyroTs - baseTs)

            imup = "{: .06f}"
            tsp = "{: .03f}"
            accelLength = math.sqrt(imuData.acceleroMeter.x**2 + imuData.acceleroMeter.y**2 + imuData.acceleroMeter.z**2)

            print(f"Accelero timestamp: {tsp.format(acceleroTs)} ms")
            print(f"Accelero: x: {imup.format(imuData.acceleroMeter.x)} y: {imup.format(imuData.acceleroMeter.y)} z: {imup.format(imuData.acceleroMeter.z)}, length {imup.format(accelLength)}")
            print(f"Gyro timestamp: {tsp.format(gyroTs)} ms")
            print(f"Gyro: x: {imup.format(imuData.gyroscope.x)} y: {imup.format(imuData.gyroscope.y)} z: {imup.format(imuData.gyroscope.z)} ")

        if cv2.waitKey(1) == ord('q'):
            break
