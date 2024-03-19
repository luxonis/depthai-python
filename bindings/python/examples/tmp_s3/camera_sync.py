#!/usr/bin/env python3

import cv2
import depthai as dai

print("Press C to capture a set of frames, Q to quit")

# Create pipeline
pipeline = dai.Pipeline()

FPS = 15

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
monoVertical = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutVertical = pipeline.create(dai.node.XLinkOut)
xoutRgb = pipeline.create(dai.node.XLinkOut)
syncNode = pipeline.create(dai.node.Sync)

xoutLeft.setStreamName('left')
xoutRight.setStreamName('right')
xoutVertical.setStreamName('vertical')
xoutRgb.setStreamName('rgb')

# Properties
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setBoardSocket(dai.CameraBoardSocket.RGB)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_720_P)

monoLeft.setFps(FPS)
monoRight.setFps(FPS)
monoVertical.setFps(FPS)
camRgb.setFps(FPS)

monoLeft.out.link(syncNode.inputs["left"])
monoRight.out.link(syncNode.inputs["right"])
monoVertical.out.link(syncNode.inputs["vertical"])
camRgb.isp.link(syncNode.inputs["RGB"])

syncNode.outputs["left"].link(xoutLeft.input)
syncNode.outputs["right"].link(xoutRight.input)
syncNode.outputs["vertical"].link(xoutVertical.input)
syncNode.outputs["RGB"].link(xoutRgb.input)

tFrame = 1000 / FPS  # 0.033s, for 30fps
tSyncThresholdMs = tFrame / 2
print(f"Sync threshold {tSyncThresholdMs:.2f}")
syncNode.setSyncThresholdMs(tSyncThresholdMs)


# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues will be used to get the grayscale frames from the outputs defined above
    qLeft = device.getOutputQueue(name="left", maxSize=30, blocking=True)
    qRight = device.getOutputQueue(name="right", maxSize=30, blocking=True)
    qVertical = device.getOutputQueue(name="vertical", maxSize=30, blocking=True)
    qRgb = device.getOutputQueue(name="rgb", maxSize=30, blocking=True)

    cnt = 0
    while True:
        inL = qLeft.get()
        inR = qRight.get()
        inV = qVertical.get()
        inRgb = qRgb.get()

        tstampL = inL.getTimestamp().total_seconds()*1000
        tstampR = inR.getTimestamp().total_seconds()*1000
        tstampV = inV.getTimestamp().total_seconds()*1000
        tstampRgb = inRgb.getTimestamp().total_seconds()*1000

        print(f"synced! tstampL {tstampL:.2f}, tstampR {tstampR:.2f}, tstampV {tstampV:.2f}, tstampRgb {tstampRgb:.2f} ")

        frameL = inL.getCvFrame()
        frameR = inR.getCvFrame()
        frameV = inV.getCvFrame()
        frameRgb = inRgb.getCvFrame()

        cv2.imshow("left", frameL)
        cv2.imshow("right", frameR)
        cv2.imshow("vertical", frameV)
        cv2.imshow("frameRgb", frameRgb)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif 0 or key == ord('c'):
            print("Saving, iter", cnt)
            cv2.imwrite(str(cnt)+"_left_"    +str(tstampL)+".png", frameL)
            cv2.imwrite(str(cnt)+"_right_"   +str(tstampR)+".png", frameR)
            cv2.imwrite(str(cnt)+"_vertical_"+str(tstampV)+".png", frameV)
            cv2.imwrite(str(cnt)+"_rgb_"+str(tstampRgb)+".png", frameRgb)
            cnt += 1