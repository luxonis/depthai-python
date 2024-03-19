#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

meshCellSize = 12

def downSampleMesh(mapXL, mapYL):
    meshLeft = []

    w = 0
    h = 0

    for y in range(mapXL.shape[0] + 1):
        if y % meshCellSize == 0:
            h += 1
            w = 0

            rowLeft = []
            for x in range(mapXL.shape[1] + 1):
                if x % meshCellSize == 0:
                    w += 1
                    if y == mapXL.shape[0] and x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y - 1, x - 1])
                        rowLeft.append(mapXL[y - 1, x - 1])
                    elif y == mapXL.shape[0]:
                        rowLeft.append(mapYL[y - 1, x])
                        rowLeft.append(mapXL[y - 1, x])
                    elif x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y, x - 1])
                        rowLeft.append(mapXL[y, x - 1])
                    else:
                        rowLeft.append(mapYL[y, x])
                        rowLeft.append(mapXL[y, x])
            if (mapXL.shape[1] % meshCellSize) % 2 != 0:
                rowLeft.append(0)
                rowLeft.append(0)
                w += 1

            meshLeft.append(rowLeft)

    meshLeft = np.array(meshLeft)

    return meshLeft, w, h


device = dai.Device()

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutVideo = pipeline.create(dai.node.XLinkOut)

xoutVideo.setStreamName("video")

# Properties
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_B)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
camRgb.setVideoSize(1920, 1200)

xoutVideo.input.setBlocking(False)
xoutVideo.input.setQueueSize(1)

# Linking
camRgb.video.link(xoutVideo.input)

camRgb.setMeshSource(dai.ColorCameraProperties.WarpMeshSource.CALIBRATION)
camRgb.setCalibrationAlpha(1)

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)
    video = device.getOutputQueue(name="video", maxSize=1, blocking=False)

    while True:
        videoIn = video.get()

        # Get BGR frame from NV12 encoded video frame to show with opencv
        # Visualizing the frame on slower hosts might have overhead
        cv2.imshow("video", videoIn.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
