#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-res", "--resolution", type=str, default="720",
    help="Sets the resolution on mono cameras. Options: 800 | 720 | 400")
parser.add_argument("-md", "--mesh_dir", type=str, default=None,
    help="Output directory for mesh files. If not specified mesh files won't be saved")
parser.add_argument("-lm", "--load_mesh", default=False, action="store_true",
    help="Read camera intrinsics, generate mesh files and load them into the stereo node.")
args = parser.parse_args()

meshDirectory = args.mesh_dir  # Output dir for mesh files
generateMesh = args.load_mesh  # Load mesh files
RES_MAP = {
    '800': {'w': 1280, 'h': 800, 'res': dai.MonoCameraProperties.SensorResolution.THE_800_P },
    '720': {'w': 1280, 'h': 720, 'res': dai.MonoCameraProperties.SensorResolution.THE_720_P },
    '400': {'w': 640, 'h': 400, 'res': dai.MonoCameraProperties.SensorResolution.THE_400_P }
}
if args.resolution not in RES_MAP:
    exit("Unsupported resolution!")

resolution = RES_MAP[args.resolution]

def getMesh(calibData):
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.CAM_B, resolution['w'], resolution['h']))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.CAM_B))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.CAM_C, resolution['w'], resolution['h']))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.CAM_C))
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    mapXL, mapYL = cv2.initUndistortRectifyMap(M1, d1, R1, M2, (resolution['w'], resolution['h']), cv2.CV_32FC1)
    mapXR, mapYR = cv2.initUndistortRectifyMap(M2, d2, R2, M2, (resolution['w'], resolution['h']), cv2.CV_32FC1)

    meshCellSize = 16
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

def saveMeshFiles(meshLeft, meshRight, outputPath):
    print("Saving mesh to:", outputPath)
    meshLeft.tofile(outputPath + "/left_mesh.calib")
    meshRight.tofile(outputPath + "/right_mesh.calib")


def create_pipeline(device: dai.Device) -> dai.Pipeline:
    calibData = device.readCalibration()
    print("Creating Stereo Depth pipeline")
    pipeline = dai.Pipeline()

    camLeft = pipeline.create(dai.node.MonoCamera)
    camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)

    camRight = pipeline.create(dai.node.MonoCamera)
    camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

    xoutRight = pipeline.create(dai.node.XLinkOut)
    xoutRight.setStreamName("right")
    camRight.out.link(xoutRight.input)

    for monoCam in (camLeft, camRight):  # Common config
        monoCam.setResolution(resolution['res'])
        # monoCam.setFps(20.0)

    stereo = pipeline.create(dai.node.StereoDepth)
    camLeft.out.link(stereo.left)
    camRight.out.link(stereo.right)
    stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
    stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout
    stereo.setLeftRightCheck(True)
    stereo.setExtendedDisparity(True)

    


    xoutDisparity = pipeline.create(dai.node.XLinkOut)
    xoutDisparity.setStreamName("disparity")
    stereo.disparity.link(xoutDisparity.input)

    xoutRectifRight = pipeline.create(dai.node.XLinkOut)
    xoutRectifRight.setStreamName("rectifiedRight")
    stereo.rectifiedRight.link(xoutRectifRight.input)

    # Create custom meshes from calibration data. Here you could also
    # load your own mesh files, or generate them in any other way.
    leftMesh, rightMesh = getMesh(calibData)
    if generateMesh:
        meshLeft = list(leftMesh.tobytes())
        meshRight = list(rightMesh.tobytes())
        # Load mesh data to the StereoDepth node
        stereo.loadMeshData(meshLeft, meshRight)

    if meshDirectory is not None:
        saveMeshFiles(leftMesh, rightMesh, meshDirectory)
    return pipeline

with dai.Device() as device:
    device.startPipeline(create_pipeline(device))

    # Create a receive queue for each stream
    qList = [device.getOutputQueue(stream, 8, blocking=False) for stream in ['right', 'rectifiedRight', 'disparity']]

    while True:
        for q in qList:
            name = q.getName()
            frame = q.get().getCvFrame()
            cv2.imshow(name, frame)
        if cv2.waitKey(1) == ord("q"):
            break
