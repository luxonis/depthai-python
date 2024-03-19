#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import argparse
import yaml
import time

calibJsonFile = str((Path(__file__).parent / Path('./depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('-si', action="store_true", help="Static input.")
parser.add_argument('calibJsonFile', nargs='?', help="Path to calibration file in json", default=calibJsonFile)
parser.add_argument('-left', type=str, default="left.png", help="left static input image")
parser.add_argument('-right', type=str, default="right.png", help="right static input image")
parser.add_argument('-bottom', type=str, default="bottom.png", help="bottom static input image")
parser.add_argument('-debug', action="store_true", default=False, help="Debug code.")
parser.add_argument('-rect', '--rectified', action="store_true", default=False, help="Generate and display rectified streams.")
parser.add_argument('-fps', type=int, default=15, help="Set camera FPS.")

args = parser.parse_args()

staticInput = args.si

enableRectified = args.rectified
cameraFPS = args.fps
blockingOutputs = False

if staticInput:
    left = args.left
    right = args.right
    bottom = args.bottom

    leftImg = cv2.imread(left, cv2.IMREAD_GRAYSCALE)
    rightImg = cv2.imread(right, cv2.IMREAD_GRAYSCALE)
    bottomImg = cv2.imread(bottom, cv2.IMREAD_GRAYSCALE)

    width = leftImg.shape[1]
    height = leftImg.shape[0]

    if args.debug:
        cv2.imshow("leftImg", leftImg)
        cv2.imshow("rightImg", rightImg)
        cv2.imshow("bottomImg", bottomImg)
        cv2.waitKey(1) 

class Undistorter:
    def __init__(self, K_in, P_rectified, d, R_in):
        P_rectified = np.array(P_rectified)
        assert len(d) == 5, "Currently using only k1, k2, k3, p1, p2"
        self.K_in = K_in
        self.P_rectified = P_rectified
        self.R_in = R_in
        self.p_inv = np.linalg.inv(P_rectified[:3, :3])
        self.k1 = d[0]
        self.k2 = d[1]
        self.p1 = d[2]
        self.p2 = d[3]
        self.k3 = d[4]
        self.k4 = 0.
        self.k5 = 0.
        self.k6 = 0.
        self.s1 = 0.
        self.s2 = 0.
        self.s3 = 0.
        self.s4 = 0.
        self.tx = 0.
        self.ty = 0.

    def undistort(self, img_shape):
        x_in = np.tile(np.arange(img_shape[0]), img_shape[1])
        y_in = np.repeat(range(img_shape[1]), img_shape[0])
        p_in = np.vstack((x_in, y_in, np.ones_like(x_in)))  # (x, y, 1)

        p = self.R_in @ self.p_inv @ p_in
        p /= p[2]
        x = p[0]
        y = p[1]

        # distort
        x2 = x**2
        y2 = y**2
        _2xy = 2 * x * y
        r2 = x2 + y2
        kr = 1 + ((self.k3 * r2 + self.k2) * r2 + self.k1) * r2
        p[0] = x * kr + self.p1 * _2xy + self.p2 * (r2 + 2 * x2)
        p[1] = y * kr + self.p2 * _2xy + self.p1 * (r2 + 2 * y2)

        pd = (self.K_in @ p).astype(np.float32)
        map_x = (pd[0] / pd[2]).reshape(img_shape[::-1])
        map_y = (pd[1] / pd[2]).reshape(img_shape[::-1])

        return map_x, map_y

meshCellSize = 16

def downSampleMesh(mapXL, mapYL, mapXR, mapYR):
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



calibData = dai.CalibrationHandler(args.calibJsonFile)

# Create pipeline
pipeline = dai.Pipeline()
pipeline.setCalibrationData(calibData)

if staticInput:
    monoLeft = pipeline.create(dai.node.XLinkIn)
    monoRight = pipeline.create(dai.node.XLinkIn)
    monoVertical = pipeline.create(dai.node.XLinkIn)
    monoLeft.setStreamName("inLeft")
    monoRight.setStreamName("inRight")
    monoVertical.setStreamName("inVertical")
else:
    monoLeft = pipeline.create(dai.node.MonoCamera)
    monoVertical = pipeline.create(dai.node.MonoCamera)
    monoRight = pipeline.create(dai.node.MonoCamera)
    monoLeft.setFps(cameraFPS)
    monoVertical.setFps(cameraFPS)
    monoRight.setFps(cameraFPS)

if enableRectified:
    xoutRectifiedVertical = pipeline.create(dai.node.XLinkOut)
    xoutRectifiedRight = pipeline.create(dai.node.XLinkOut)
    xoutRectifiedLeft = pipeline.create(dai.node.XLinkOut)
xoutDisparityVertical = pipeline.create(dai.node.XLinkOut)
xoutDisparityHorizontal = pipeline.create(dai.node.XLinkOut)
stereoVertical = pipeline.create(dai.node.StereoDepth)
stereoHorizontal = pipeline.create(dai.node.StereoDepth)
syncNode = pipeline.create(dai.node.Sync)

if enableRectified:
    xoutRectifiedVertical.setStreamName("rectified_vertical")
    xoutRectifiedRight.setStreamName("rectified_right")
    xoutRectifiedLeft.setStreamName("rectified_left")
xoutDisparityVertical.setStreamName("disparity_vertical")
xoutDisparityHorizontal.setStreamName("disparity_horizontal")

# Define sources and outputs
if not staticInput:
    monoVertical.setBoardSocket(dai.CameraBoardSocket.VERTICAL)
    monoVertical.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
    monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
    monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
    monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
    monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoRight.out.link(syncNode.inputs["right"])
monoLeft.out.link(syncNode.inputs["left"])
monoVertical.out.link(syncNode.inputs["vertical"])

monoRight.setFps(20)
monoLeft.setFps(20)
monoVertical.setFps(20)

syncNode.outputs["left"].link(stereoVertical.left) # left input is bottom camera
syncNode.outputs["vertical"].link(stereoVertical.right) # right input is right camera
stereoVertical.disparity.link(xoutDisparityVertical.input)
if enableRectified:
    stereoVertical.rectifiedLeft.link(xoutRectifiedVertical.input)
    stereoVertical.rectifiedRight.link(xoutRectifiedRight.input)
stereoVertical.setVerticalStereo(True)

syncNode.outputs["left"].link(stereoHorizontal.left)
syncNode.outputs["right"].link(stereoHorizontal.right)
stereoHorizontal.disparity.link(xoutDisparityHorizontal.input)
if enableRectified:
    stereoHorizontal.rectifiedLeft.link(xoutRectifiedLeft.input)
# stereoHorizontal.rectifiedRight.link(xoutRectifiedRight.input)
stereoHorizontal.setVerticalStereo(False)

# stereoHorizontal.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_RIGHT)
# stereoVertical.initialConfig.setDepthAlign(dai.StereoDepthConfig.AlgorithmControl.DepthAlign.RECTIFIED_RIGHT)

if 1:
    # leftMesh, rightMesh = getMesh(calibData, resolution)
    with open('calib.yaml', 'r') as file:
        calibData = yaml.safe_load(file)

    calib = calibData["calibration"]
    rectif = calibData["rectification"]
    name = "left"
    r_code = "lbr"
    img_shape = (1280, 720)
    K = np.array(calib[name]["K"])
    P = np.array(rectif[r_code][name]["P"])
    d = np.array(calib[name]["d"])
    R = np.array(calib["right"]["R"])

    undis = Undistorter(K, P, d, R)
    mapXL, mapYL = undis.undistort(img_shape)

    name = "right"
    r_code = "lbr"
    img_shape = (1280, 720)
    K = np.array(calib[name]["K"])
    P = np.array(rectif[r_code][name]["P"])
    d = np.array(calib[name]["d"])
    R = np.array(calib["right"]["R"])

    undis = Undistorter(K, P, d, R)
    mapXR, mapYR = undis.undistort(img_shape)

    leftMesh, rightMesh = downSampleMesh(mapXL, mapYL, mapXR, mapYR)

    name = "bottom"
    r_code = "lbr"
    img_shape = (1280, 720)
    K = np.array(calib[name]["K"])
    P = np.array(rectif[r_code][name]["P"])
    d = np.array(calib[name]["d"])
    R = np.array(calib["right"]["R"])

    undis = Undistorter(K, P, d, R)
    mapXV, mapYV = undis.undistort(img_shape)

    def rotate_mesh_90_cw(map_x, map_y):
        direction = -1
        map_x_rot = np.rot90(map_x, direction)
        map_y_rot = np.rot90(map_y, direction)
        return map_x_rot, map_y_rot

    mapXV_rot, mapYV_rot = rotate_mesh_90_cw(mapXV, mapYV)
    mapXR_rot, mapYR_rot = rotate_mesh_90_cw(mapXR, mapYR)

    rightMeshRot, verticalMeshRot = downSampleMesh(mapXR_rot, mapYR_rot, mapXV_rot, mapYV_rot)

    meshLeft = list(leftMesh.tobytes())
    meshRight = list(rightMesh.tobytes())
    stereoHorizontal.loadMeshData(meshLeft, meshRight)
    stereoHorizontal.setMeshStep(meshCellSize,meshCellSize)

    # for vertical stereo left input is right camera
    meshLeftVertical = list(verticalMeshRot.tobytes())
    meshRightVertical = list(rightMeshRot.tobytes())
    stereoVertical.loadMeshData(meshLeftVertical, meshRightVertical)

    stereoVertical.setMeshStep(meshCellSize,meshCellSize)

    if args.debug and staticInput:
        img_l_rectified = cv2.remap(leftImg, mapXL, mapYL, cv2.INTER_LINEAR)
        cv2.imshow("img_l_rectified", img_l_rectified)

        img_r_rectified = cv2.remap(rightImg, mapXR, mapYR, cv2.INTER_LINEAR)
        cv2.imshow("img_r_rectified", img_r_rectified)

        img_r_vertical = cv2.remap(rightImg, mapXR_rot, mapYR_rot, cv2.INTER_LINEAR)
        cv2.imshow("img_r_vertical", img_r_vertical)

        img_b_vertical = cv2.remap(bottomImg, mapXV_rot, mapYV_rot, cv2.INTER_LINEAR)
        cv2.imshow("img_b_vertical", img_b_vertical)


    # stereoVertical.setOutputSize(720,1024)

stereoHorizontal.left.setQueueSize(1)
stereoHorizontal.left.setBlocking(False)
stereoHorizontal.right.setQueueSize(1)
stereoHorizontal.right.setBlocking(False)
stereoVertical.left.setQueueSize(1)
stereoVertical.left.setBlocking(False)
stereoVertical.right.setQueueSize(1)
stereoVertical.right.setBlocking(False)

stereoHorizontal.setNumFramesPool(10)
stereoVertical.setNumFramesPool(10)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qDisparityHorizontal = device.getOutputQueue("disparity_horizontal", 4, blockingOutputs)
    qDisparityVertical = device.getOutputQueue("disparity_vertical", 4, blockingOutputs)
    if enableRectified:
        qRectifiedVertical = device.getOutputQueue("rectified_vertical", 4, blockingOutputs)
        qRectifiedRight = device.getOutputQueue("rectified_right", 4, blockingOutputs)
        qRectifiedLeft = device.getOutputQueue("rectified_left", 4, blockingOutputs)

    if staticInput:
        qInLeft = device.getInputQueue("inLeft")
        qInRight = device.getInputQueue("inRight")
        qInVertical = device.getInputQueue("inVertical")

    while True:
        if staticInput:
            ts = dai.Clock.now()

            data = cv2.resize(leftImg, (width, height), interpolation = cv2.INTER_AREA)
            data = data.reshape(height*width)
            img = dai.ImgFrame()
            img.setData(data)
            img.setInstanceNum(1)
            img.setType(dai.ImgFrame.Type.RAW8)
            img.setWidth(width)
            img.setHeight(height)
            img.setTimestamp(ts)
            # print("left send")
            qInLeft.send(img)

            data = cv2.resize(rightImg, (width, height), interpolation = cv2.INTER_AREA)
            data = data.reshape(height*width)
            img = dai.ImgFrame()
            img.setData(data)
            img.setInstanceNum(2)
            img.setType(dai.ImgFrame.Type.RAW8)
            img.setWidth(width)
            img.setHeight(height)
            img.setTimestamp(ts)
            qInRight.send(img)
            # print("right send")

            data = cv2.resize(bottomImg, (width, height), interpolation = cv2.INTER_AREA)
            data = data.reshape(height*width)
            img = dai.ImgFrame()
            img.setData(data)
            img.setInstanceNum(3)
            img.setType(dai.ImgFrame.Type.RAW8)
            img.setWidth(width)
            img.setHeight(height)
            img.setTimestamp(ts)
            qInVertical.send(img)
            # print("vertical send")

        if enableRectified:
            inRectifiedVertical = qRectifiedVertical.get()
            frameRVertical = inRectifiedVertical.getCvFrame()
            cv2.imshow("rectified_vertical", frameRVertical)

            inRectifiedRight = qRectifiedRight.get()
            frameRRight = inRectifiedRight.getCvFrame()
            cv2.imshow("rectified_right", frameRRight)

            inRectifiedLeft = qRectifiedLeft.get()
            frameRLeft = inRectifiedLeft.getCvFrame()
            cv2.imshow("rectified_left", frameRLeft)

        inDisparityVertical = qDisparityVertical.get()
        frameDepth = inDisparityVertical.getCvFrame()
        # cv2.imshow("disparity", frameDepth)
        tsV = inDisparityVertical.getTimestampDevice()
        # print(tsV)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disparity_vertical", disp)

        inDisparityHorizontal = qDisparityHorizontal.get()
        frameDepth = inDisparityHorizontal.getCvFrame()
        # cv2.imshow("disparity", frameDepth)
        tsH = inDisparityHorizontal.getTimestampDevice()
        # print(tsH)
        disparityTsDiff = tsV-tsH
        if disparityTsDiff:
            print(disparityTsDiff)

        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disparity_horizontal", disp)


        if cv2.waitKey(1) == ord('q'):
            break