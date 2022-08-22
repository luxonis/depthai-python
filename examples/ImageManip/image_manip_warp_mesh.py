#!/usr/bin/env python3
# from turtle import shape
import cv2
import depthai as dai
import numpy as np

def getMesh(calibData, resolution):
    print(resolution)
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.LEFT))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, resolution[0], resolution[1]))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.RIGHT))
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    mapXL, mapYL = cv2.initUndistortRectifyMap(M1, d1, R1, M2, resolution, cv2.CV_32FC1)
    mapXR, mapYR = cv2.initUndistortRectifyMap(M2, d2, R2, M2, resolution, cv2.CV_32FC1)

    meshCellSize = 16
    meshLeft = []
    meshRight = []

    for y in range(mapXL.shape[0] + 1):
        if y % meshCellSize == 0:
            for x in range(mapXL.shape[1]):
                if x % meshCellSize == 0:
                    if y == mapXL.shape[0] and x == mapXL.shape[1]:
                        meshLeft.append((mapXL[y - 1, x - 1], mapYL[y - 1, x - 1]))
                        meshRight.append((mapXR[y - 1, x - 1], mapYR[y - 1, x - 1]))
                    elif y == mapXL.shape[0]:
                        meshLeft.append((mapXL[y - 1,  x], mapYL[y - 1, x]))
                        meshRight.append((mapXR[y - 1, x], mapYR[y - 1, x]))
                    elif x == mapXL.shape[1]:
                        meshLeft.append((mapXL[y, x - 1], mapYL[y, x - 1]))
                        meshRight.append((mapXR[y, x - 1], mapYR[y, x - 1]))
                    else:
                        meshLeft.append(( mapXL[y, x - 1], mapYL[y,x]))
                        meshRight.append((mapXR[y, x - 1], mapYR[y,x]))
            if (mapXL.shape[1] % meshCellSize) % 2 != 0:
                meshLeft.append((0, 0))
                meshRight.append((0, 0))

    return meshLeft, meshRight


device = dai.Device()
calibrationHandler = device.readCalibration()
# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
camRgb.setBoardSocket(dai.CameraBoardSocket.LEFT)

camRgb.setIspScale(1, 1);

maxFrameSize = camRgb.getIspWidth() * camRgb.getIspHeight() * 3 / 2

# Warp preview frame 1
manip1 = pipeline.create(dai.node.ImageManip)
# Create a custom warp mesh

meshWidth = camRgb.getIspWidth() // 16;
meshHeight = (camRgb.getIspHeight() // 16) + 1
mesh_left, mesh_right = getMesh(calibrationHandler, (camRgb.getIspWidth(), camRgb.getIspHeight()))
print('mesh shapes...')
print(meshWidth, meshHeight)
print(len(mesh_left))
print(meshWidth*meshHeight)

manip1.setWarpMesh(mesh_left, meshWidth, meshHeight)
manip1.setMaxOutputFrameSize(int(maxFrameSize))

camRgb.isp.link(manip1.inputImage)
xout1 = pipeline.create(dai.node.XLinkOut)
xout1.setStreamName('out1')
manip1.out.link(xout1.input)

device.startPipeline(pipeline)

# Connect to device and start pipeline
# with dai.Device(pipeline) as device:
# Output queue will be used to get the rgb frames from the output defined above
q1 = device.getOutputQueue(name="out1", maxSize=8, blocking=False)

while True:
    in1 = q1.get()
    if in1 is not None:
        imgFrame = in1.getCvFrame()
        imgFrame = cv2.resize(imgFrame, (0, 0), fx=0.7, fy=0.7)
        cv2.imshow("Warped preview 1", imgFrame)
    if cv2.waitKey(1) == ord('q'):
        break
