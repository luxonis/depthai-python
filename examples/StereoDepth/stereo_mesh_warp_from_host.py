#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
import argparse

meshCellSize = 14
def getMesh(calibData, resolution):
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.LEFT))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, resolution[0], resolution[1]))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.RIGHT))
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    mapXL, mapYL = cv2.initUndistortRectifyMap(M1, d1, R1, M2, (resolution), cv2.CV_32FC1)
    mapXR, mapYR = cv2.initUndistortRectifyMap(M2, d2, R2, M2, (resolution), cv2.CV_32FC1)

    meshLeft = []
    meshRight = []

    for y in range(mapXL.shape[0] + 1):
        if y % meshCellSize == 0:
            rowLeft = []
            rowRight = []
            for x in range(mapXL.shape[1] + 1):
                if x % meshCellSize == 0:
                    if y == mapXL.shape[0] and x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y - 1, x - 1])
                        rowLeft.append(mapXL[y - 1, x - 1])
                        rowRight.append(mapYR[y - 1, x - 1])
                        rowRight.append(mapXR[y - 1, x - 1])
                    elif y == mapXL.shape[0]:
                        rowLeft.append(mapYL[y - 1, x])
                        rowLeft.append(mapXL[y - 1, x])
                        rowRight.append(mapYR[y - 1, x])
                        rowRight.append(mapXR[y - 1, x])
                    elif x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y, x - 1])
                        rowLeft.append(mapXL[y, x - 1])
                        rowRight.append(mapYR[y, x - 1])
                        rowRight.append(mapXR[y, x - 1])
                    else:
                        rowLeft.append(mapYL[y, x])
                        rowLeft.append(mapXL[y, x])
                        rowRight.append(mapYR[y, x])
                        rowRight.append(mapXR[y, x])
            if (mapXL.shape[1] % meshCellSize) % 2 != 0:
                rowLeft.append(0)
                rowLeft.append(0)
                rowRight.append(0)
                rowRight.append(0)

            meshLeft.append(rowLeft)
            meshRight.append(rowRight)

    meshLeft = np.array(meshLeft)
    meshRight = np.array(meshRight)

    return meshLeft, meshRight

def getDisparityFrame(frame):
    maxDisp = stereo.initialConfig.getMaxDisparity()
    disp = (frame * (255.0 / maxDisp)).astype(np.uint8)
    disp = cv2.applyColorMap(disp, cv2.COLORMAP_JET)

    return disp

from pathlib import Path
import argparse
calibJsonFile = str((Path(__file__).parent / Path('./depthai_calib.json')).resolve().absolute())
calibData = dai.CalibrationHandler(calibJsonFile)

print("Creating Stereo Depth pipeline")
pipeline = dai.Pipeline()
pipeline.setCalibrationData(calibData)

camLeft = pipeline.create(dai.node.MonoCamera)
camRight = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDisparity = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutRectifLeft = pipeline.create(dai.node.XLinkOut)
xoutRectifRight = pipeline.create(dai.node.XLinkOut)

camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDisparity.setStreamName("disparity")
xoutDepth.setStreamName("depth")
xoutRectifLeft.setStreamName("rectifiedLeft")
xoutRectifRight.setStreamName("rectifiedRight")

camLeft.out.link(stereo.left)
camRight.out.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.disparity.link(xoutDisparity.input)
stereo.rectifiedLeft.link(xoutRectifLeft.input)
stereo.rectifiedRight.link(xoutRectifRight.input)

streams = ["left", "right"]
streams.extend(["rectifiedLeft", "rectifiedRight"])
streams.append("disparity")

resolution = (1280, 720)

device = dai.Device()
leftMesh, rightMesh = getMesh(calibData, resolution)
meshLeft = list(leftMesh.tobytes())
meshRight = list(rightMesh.tobytes())
stereo.loadMeshData(meshLeft, meshRight)
stereo.setMeshStep(meshCellSize,meshCellSize)


print("Creating DepthAI device")
with device:
    device.startPipeline(pipeline)

    # Create a receive queue for each stream
    qList = [device.getOutputQueue(stream, 8, blocking=False) for stream in streams]

    while True:
        for q in qList:
            name = q.getName()
            frame = q.get().getCvFrame()
            if name == "depth":
                frame = frame.astype(np.uint16)
            elif name == "disparity":
                frame = getDisparityFrame(frame)

            cv2.imshow(name, frame)
        if cv2.waitKey(1) == ord("q"):
            break
