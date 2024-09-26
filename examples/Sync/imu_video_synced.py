import depthai as dai
import numpy as np
import cv2
from datetime import timedelta

device = dai.Device()

imuType = device.getConnectedIMU()
imuFirmwareVersion = device.getIMUFirmwareVersion()
print(f"IMU type: {imuType}, firmware version: {imuFirmwareVersion}")

if imuType != "BNO086":
    print("Rotation vector output is supported only by BNO086!")
    exit(0)

pipeline = dai.Pipeline()

color = pipeline.create(dai.node.ColorCamera)
imu = pipeline.create(dai.node.IMU)
sync = pipeline.create(dai.node.Sync)
xoutImu = pipeline.create(dai.node.XLinkOut)
xoutImu.setStreamName("imu")

xoutGrp = pipeline.create(dai.node.XLinkOut)
xoutGrp.setStreamName("xout")

color.setCamera("color")

imu.enableIMUSensor(dai.IMUSensor.ROTATION_VECTOR, 120)
imu.setBatchReportThreshold(1)
imu.setMaxBatchReports(10)

sync.setSyncThreshold(timedelta(milliseconds=10))
sync.setSyncAttempts(-1)

color.video.link(sync.inputs["video"])
imu.out.link(sync.inputs["imu"])

sync.out.link(xoutGrp.input)


with device:
    device.startPipeline(pipeline)
    groupQueue = device.getOutputQueue("xout", 3, True)
    while True:
        groupMessage = groupQueue.get()
        imuMessage = groupMessage["imu"]
        colorMessage = groupMessage["video"]
        print()
        print("Device timestamp imu: " + str(imuMessage.getTimestampDevice()))
        print("Device timestamp video:" + str(colorMessage.getTimestampDevice()))
        latestRotationVector = imuMessage.packets[-1].rotationVector
        imuF = "{:.4f}"
        print(f"Quaternion: i: {imuF.format(latestRotationVector.i)} j: {imuF.format(latestRotationVector.j)} "
        f"k: {imuF.format(latestRotationVector.k)} real: {imuF.format(latestRotationVector.real)}")
        print()
        cv2.imshow("video", colorMessage.getCvFrame())
        if cv2.waitKey(1) == ord("q"):
            break
