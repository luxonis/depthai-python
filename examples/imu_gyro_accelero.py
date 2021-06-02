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
sensorConfig.sensorId = dai.IMUSensorId.RAW_ACCELEROMETER
imu.enableIMUSensor(sensorConfig)
sensorConfig.sensorId = dai.IMUSensorId.RAW_GYROSCOPE
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
            acceleroTs = imuData.rawAcceleroMeter.timestamp.getTimestamp()
            gyroTs = imuData.rawGyroscope.timestamp.getTimestamp()
            if baseTs is None:
                baseTs = acceleroTs if acceleroTs < gyroTs else gyroTs
            acceleroTs = timeDeltaToMilliS(acceleroTs - baseTs)
            gyroTs = timeDeltaToMilliS(gyroTs - baseTs)

            imup = "{: .06f}"
            tsp = "{: .03f}"
            accelLength = math.sqrt(imuData.rawAcceleroMeter.x**2 + imuData.rawAcceleroMeter.y**2 + imuData.rawAcceleroMeter.z**2)

            print(f"Accelerometer timestamp: {tsp.format(acceleroTs)} ms")
            print(f"Accelerometer [m/s^2]: x: {imup.format(imuData.rawAcceleroMeter.x)} y: {imup.format(imuData.rawAcceleroMeter.y)} z: {imup.format(imuData.rawAcceleroMeter.z)}, length {imup.format(accelLength)}")
            print(f"Gyroscope timestamp: {tsp.format(gyroTs)} ms")
            print(f"Gyroscope [rad/s]: x: {imup.format(imuData.rawGyroscope.x)} y: {imup.format(imuData.rawGyroscope.y)} z: {imup.format(imuData.rawGyroscope.z)} ")

        if cv2.waitKey(1) == ord('q'):
            break
