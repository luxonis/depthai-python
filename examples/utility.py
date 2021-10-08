import math

import cv2
import depthai as dai
import numpy as np


def calculateDispScaleFactor(device):
    calib = device.readCalibration()
    eeprom = calib.getEepromData()
    leftCam = calib.getStereoLeftCameraId()
    if leftCam != dai.CameraBoardSocket.AUTO:
        camInfo = eeprom.cameraData[leftCam]
        baseline = abs(camInfo.extrinsics.specTranslation.x * 10)  # cm -> mm
        fov = calib.getFov(calib.getStereoLeftCameraId())
        focal = (camInfo.width / 2) / (2. * math.tan(math.radians(fov / 2)))
    else:
        print("Warning: calibration data missing, using OAK-D defaults")
        baseline = 75
        fov = 71.86
        focal = 440
    return baseline * focal


def parseDepthPacket(inPacket, dispScaleFactor, maxDisparity, colorMap=cv2.COLORMAP_JET):
    depthRaw = inPacket.getFrame()
    with np.errstate(divide='ignore'):
        dispFrame = dispScaleFactor / depthRaw
    dispFrame = (dispFrame * 255. / maxDisparity).astype(np.uint8)
    map_custom = cv2.applyColorMap(np.arange(256, dtype=np.uint8), colorMap)
    map_custom[0] = [0, 0, 0]
    return cv2.applyColorMap(dispFrame, map_custom)