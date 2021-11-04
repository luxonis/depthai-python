import math

import cv2
import depthai as dai
import numpy as np


def getDisparityScaleFactor(device, width):
    calib = device.readCalibration()
    eeprom = calib.getEepromData()
    rightCam = calib.getStereoRightCameraId()
    if rightCam != dai.CameraBoardSocket.AUTO:
        camInfo = eeprom.cameraData[rightCam]
        baseline = abs(camInfo.extrinsics.specTranslation.x * 10)  # cm -> mm
        fov = calib.getFov(calib.getStereoRightCameraId())
        focal = (camInfo.width / 2) / (2. * math.tan(math.radians(fov / 2)))
    else:
        print("Warning: calibration data missing, using OAK-D defaults")
        baseline = 75
        fov = 71.86
        focal = width / (2. * math.tan(math.radians(fov / 2)))
    return baseline * focal


def parseDepthPacket(inPacket, dispScaleFactor, stereoNode, colorMap=cv2.COLORMAP_JET):
    maxDisparity = stereoNode.initialConfig.getMaxDisparity()
    subpixelBits = stereoNode.initialConfig.get().algorithmControl.subpixelFractionalBits if stereoNode.initialConfig.get().algorithmControl.enableSubpixel else 0
    subpixelLevels = pow(2, subpixelBits)
    dispIntegerLevels = maxDisparity / subpixelLevels
    depthRaw = inPacket.getFrame()
    with np.errstate(divide='ignore'):
        dispFrame = dispScaleFactor / depthRaw

    dispFrame = (dispFrame * 255. / dispIntegerLevels).astype(np.uint8)
    map_custom = cv2.applyColorMap(np.arange(256, dtype=np.uint8), colorMap)
    map_custom[0] = [0, 0, 0]
    return cv2.applyColorMap(dispFrame, map_custom)
