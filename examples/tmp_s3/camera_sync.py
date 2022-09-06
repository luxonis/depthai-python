#!/usr/bin/env python3

import cv2
import depthai as dai

print("Press C to capture a set of frames, Q to quit")

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
monoVertical = pipeline.create(dai.node.MonoCamera)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutVertical = pipeline.create(dai.node.XLinkOut)

xoutLeft.setStreamName('left')
xoutRight.setStreamName('right')
xoutVertical.setStreamName('vertical')

# Properties
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoRight.out.link(xoutRight.input)
monoLeft.out.link(xoutLeft.input)
monoVertical.out.link(xoutVertical.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queues will be used to get the grayscale frames from the outputs defined above
    qLeft = device.getOutputQueue(name="left", maxSize=30, blocking=True)
    qRight = device.getOutputQueue(name="right", maxSize=30, blocking=True)
    qVertical = device.getOutputQueue(name="vertical", maxSize=30, blocking=True)

    cnt = 0
    prevL, prevR, prevV = 0, 0, 0
    tFrame = 1000 / monoLeft.getFps()  # 0.033s, for 30fps
    tSyncThreshold = tFrame / 2
    debug = True
    while True:
        inL = qLeft.get()
        inR = qRight.get()
        inV = qVertical.get()

        while True:
            tstampL = inL.getTimestamp().total_seconds()*1000
            tstampR = inR.getTimestamp().total_seconds()*1000
            tstampV = inV.getTimestamp().total_seconds()*1000

            diffLR = tstampL - tstampR
            if debug:
                print(f"diffLR {diffLR}")
            if abs(diffLR) > tSyncThreshold:
                if diffLR < 0:
                    inL = qLeft.get()
                elif diffLR > 0:
                    inR = qRight.get()
                continue

            diffLV = tstampL - tstampV
            if debug:
                print(f"diffLV {diffLV}")
            if abs(diffLV) > tSyncThreshold:
                if diffLV < 0:
                    inL = qLeft.get()
                elif diffLV > 0:
                    inV = qVertical.get()
                continue

            diffRV = tstampR - tstampV
            if debug:
                print(f"diffRV {diffRV}")
            if abs(diffRV) > tSyncThreshold:
                if diffRV < 0:
                    inR = qRight.get()
                elif diffRV > 0:
                    inV = qVertical.get()
                continue
            
            if debug:
                print(f"synced! tstampL {tstampL}, tstampR {tstampR}, tstampV {tstampV} ")
            break
        

        frameL = inL.getCvFrame()
        frameR = inR.getCvFrame()
        frameV = inV.getCvFrame()

        cv2.imshow("left", frameL)
        cv2.imshow("right", frameR)
        cv2.imshow("vertical", frameV)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif 0 or key == ord('c'):
            print("Saving, iter", cnt)
            cv2.imwrite(str(cnt)+"_left_"    +str(tstampL)+".png", frameL)
            cv2.imwrite(str(cnt)+"_right_"   +str(tstampR)+".png", frameR)
            cv2.imwrite(str(cnt)+"_vertical_"+str(tstampV)+".png", frameV)
            cnt += 1