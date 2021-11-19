#!/usr/bin/env python3

import cv2
import depthai as dai
import time
import math

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
imu = pipeline.create(dai.node.IMU)
xlinkOut = pipeline.create(dai.node.XLinkOut)

xlinkOut.setStreamName("imu")

# enable ROTATION_VECTOR at 400 hz rate
imu.enableIMUSensor(dai.IMUSensor.ROTATION_VECTOR, 400)
# above this threshold packets will be sent in batch of X, if the host is not blocked and USB bandwidth is available
imu.setBatchReportThreshold(1)
# maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
# if lower or equal to batchReportThreshold then the sending is always blocking on device
# useful to reduce device's CPU load  and number of lost packets, if CPU load is high on device side due to multiple nodes
imu.setMaxBatchReports(10)

# Link plugins IMU -> XLINK
imu.out.link(xlinkOut.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:

    def timeDeltaToMilliS(delta) -> float:
        return delta.total_seconds()*1000

    # Output queue for imu bulk packets
    imuQueue = device.getOutputQueue(name="imu", maxSize=50, blocking=False)
    baseTs = None
    while True:
        imuData = imuQueue.get()  # blocking call, will wait until a new data has arrived

        imuPackets = imuData.packets
        for imuPacket in imuPackets:
            rVvalues = imuPacket.rotationVector

            rvTs = rVvalues.timestamp.get()
            if baseTs is None:
                baseTs = rvTs
            rvTs = rvTs - baseTs

            imuF = "{:.06f}"
            tsF  = "{:.03f}"

            print(f"Rotation vector timestamp: {tsF.format(timeDeltaToMilliS(rvTs))} ms")
            print(f"Quaternion: i: {imuF.format(rVvalues.i)} j: {imuF.format(rVvalues.j)} "
                f"k: {imuF.format(rVvalues.k)} real: {imuF.format(rVvalues.real)}")
            print(f"Accuracy (rad): {imuF.format(rVvalues.rotationVectorAccuracy)}")


        if cv2.waitKey(1) == ord('q'):
            break
