#!/usr/bin/env python3

"""
Due to an issue with our calibration, you might receive the following error when running this script on early release OAK Thermal devices:
```bash
[ImageAlign(4)] [error] Failed to get calibration data: Extrinsic connection between the requested cameraId's doesn't exist. Please recalibrate or modify your calibration data
```
If this happens, please download the calibration data + script from https://drive.google.com/drive/folders/1Q_MZMqWMKDC1eOqVHGPeDO-NJgFmnY5U,
place them into the same folder, connect the camera to the computer and run the script. This will update the
calibration and add required extrinsics between the camera sensors.
"""

import cv2
import depthai as dai
import numpy as np
import time
from datetime import timedelta

FPS = 25.0

RGB_SOCKET = dai.CameraBoardSocket.CAM_A
COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_1080_P

class FPSCounter:
    def __init__(self):
        self.frameTimes = []

    def tick(self):
        now = time.time()
        self.frameTimes.append(now)
        self.frameTimes = self.frameTimes[-100:]

    def getFps(self):
        if len(self.frameTimes) <= 1:
            return 0
        # Calculate the FPS
        return (len(self.frameTimes) - 1) / (self.frameTimes[-1] - self.frameTimes[0])

device = dai.Device()

thermalWidth, thermalHeight = -1, -1
thermalFound = False
for features in device.getConnectedCameraFeatures():
    if dai.CameraSensorType.THERMAL in features.supportedTypes:
        thermalFound = True
        thermalSocket = features.socket
        thermalWidth, thermalHeight = features.width, features.height
        break
if not thermalFound:
    raise RuntimeError("No thermal camera found!")


ISP_SCALE = 3

calibrationHandler = device.readCalibration()
rgbDistortion = calibrationHandler.getDistortionCoefficients(RGB_SOCKET)
distortionModel = calibrationHandler.getDistortionModel(RGB_SOCKET)
if distortionModel != dai.CameraModel.Perspective:
    raise RuntimeError("Unsupported distortion model for RGB camera. This example supports only Perspective model.")

pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
thermalCam = pipeline.create(dai.node.Camera)
thermalCam.setBoardSocket(thermalSocket)
thermalCam.setFps(FPS)

sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)
align = pipeline.create(dai.node.ImageAlign)
cfgIn = pipeline.create(dai.node.XLinkIn)


camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(COLOR_RESOLUTION)
camRgb.setFps(FPS)
camRgb.setIspScale(1,ISP_SCALE)

out.setStreamName("out")

sync.setSyncThreshold(timedelta(seconds=0.5 / FPS))

cfgIn.setStreamName("config")

cfg = align.initialConfig.get()
staticDepthPlane = cfg.staticDepthPlane

# Linking
align.outputAligned.link(sync.inputs["aligned"])
camRgb.isp.link(sync.inputs["rgb"])
camRgb.isp.link(align.inputAlignTo)
thermalCam.raw.link(align.input)
sync.out.link(out.input)
cfgIn.out.link(align.inputConfig)


rgbWeight = 0.4
thermalWeight = 0.6


def updateBlendWeights(percentRgb):
    """
    Update the rgb and depth weights used to blend depth/rgb image
    @param[in] percent_rgb The rgb weight expressed as a percentage (0..100)
    """
    global thermalWeight
    global rgbWeight
    rgbWeight = float(percentRgb) / 100.0
    thermalWeight = 1.0 - rgbWeight

def updateDepthPlane(depth):
    global staticDepthPlane
    staticDepthPlane = depth

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)
    queue = device.getOutputQueue("out", 8, False)
    cfgQ = device.getInputQueue("config")

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    windowName = "rgb-thermal"

    # Set the window to be resizable and the initial size
    cv2.namedWindow(windowName, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(windowName, 1280, 720)
    cv2.createTrackbar(
        "RGB Weight %",
        windowName,
        int(rgbWeight * 100),
        100,
        updateBlendWeights,
    )
    cv2.createTrackbar(
        "Static Depth Plane [mm]",
        windowName,
        0,
        2000,
        updateDepthPlane,
    )
    fpsCounter = FPSCounter()
    while True:
        messageGroup = queue.get()
        assert isinstance(messageGroup, dai.MessageGroup)
        frameRgb = messageGroup["rgb"]
        assert isinstance(frameRgb, dai.ImgFrame)
        thermalAligned = messageGroup["aligned"]
        assert isinstance(thermalAligned, dai.ImgFrame)
        frameRgbCv = frameRgb.getCvFrame()
        fpsCounter.tick()

        rgbIntrinsics = calibrationHandler.getCameraIntrinsics(RGB_SOCKET, int(frameRgbCv.shape[1]), int(frameRgbCv.shape[0]))

        cvFrameUndistorted = cv2.undistort(
            frameRgbCv,
            np.array(rgbIntrinsics),
            np.array(rgbDistortion),
        )

        # Colorize the aligned depth
        thermalFrame = thermalAligned.getCvFrame().astype(np.float32)
        # Create a mask for nan values
        mask = np.isnan(thermalFrame)
        # Replace nan values with a mean for visualization
        thermalFrame[mask] = np.nanmean(thermalFrame)
        thermalFrame = cv2.normalize(thermalFrame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
        colormappedFrame = cv2.applyColorMap(thermalFrame, cv2.COLORMAP_MAGMA)
        # Apply the mask back with black pixels (0)
        colormappedFrame[mask] = 0

        blended = cv2.addWeighted(cvFrameUndistorted, rgbWeight, colormappedFrame, thermalWeight, 0)

        cv2.putText(
            blended,
            f"FPS: {fpsCounter.getFps():.2f}",
            (10, 30),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (255, 255, 255),
            2,
        )

        cv2.imshow(windowName, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break

        cfg.staticDepthPlane = staticDepthPlane
        cfgQ.send(cfg)
