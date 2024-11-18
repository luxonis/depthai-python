#!/usr/bin/env python3

import cv2
import depthai as dai
from datetime import timedelta
import numpy as np

# This is an interactive example that shows how two frame sources without depth information.
FPS = 30.0

RGB_SOCKET = dai.CameraBoardSocket.CAM_A
LEFT_SOCKET = dai.CameraBoardSocket.CAM_B
ALIGN_SOCKET = LEFT_SOCKET

COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_1080_P
LEFT_RIGHT_RESOLUTION = dai.MonoCameraProperties.SensorResolution.THE_720_P

ISP_SCALE = 3

device = dai.Device()

calibrationHandler = device.readCalibration()
rgbIntrinsics = calibrationHandler.getCameraIntrinsics(RGB_SOCKET, int(1920 / ISP_SCALE), int(1080 / ISP_SCALE))
rgbDistortion = calibrationHandler.getDistortionCoefficients(RGB_SOCKET)
distortionModel = calibrationHandler.getDistortionModel(RGB_SOCKET)
if distortionModel != dai.CameraModel.Perspective:
    raise RuntimeError("Unsupported distortion model for RGB camera. This example supports only Perspective model.")

pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)
align = pipeline.create(dai.node.ImageAlign)
cfgIn = pipeline.create(dai.node.XLinkIn)

left.setResolution(LEFT_RIGHT_RESOLUTION)
left.setBoardSocket(LEFT_SOCKET)
left.setFps(FPS)

camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(COLOR_RESOLUTION)
camRgb.setFps(FPS)
camRgb.setIspScale(1, ISP_SCALE)

out.setStreamName("out")

sync.setSyncThreshold(timedelta(seconds=0.5 / FPS))

cfgIn.setStreamName("config")

cfg = align.initialConfig.get()
staticDepthPlane = cfg.staticDepthPlane

# Linking
align.outputAligned.link(sync.inputs["aligned"])
camRgb.isp.link(sync.inputs["rgb"])
camRgb.isp.link(align.inputAlignTo)
left.out.link(align.input)
sync.out.link(out.input)
cfgIn.out.link(align.inputConfig)


rgbWeight = 0.4
leftWeight = 0.6


def updateBlendWeights(percentRgb):
    """
    Update the rgb and left weights used to blend rgb/left image

    @param[in] percent_rgb The rgb weight expressed as a percentage (0..100)
    """
    global leftWeight
    global rgbWeight
    rgbWeight = float(percentRgb) / 100.0
    leftWeight = 1.0 - rgbWeight

def updateDepthPlane(depth):
    global staticDepthPlane
    staticDepthPlane = depth

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)
    queue = device.getOutputQueue("out", 8, False)
    cfgQ = device.getInputQueue("config")

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    windowName = "rgb-left"

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
    while True:
        messageGroup = queue.get()
        assert isinstance(messageGroup, dai.MessageGroup)
        frameRgb = messageGroup["rgb"]
        assert isinstance(frameRgb, dai.ImgFrame)
        leftAligned = messageGroup["aligned"]
        assert isinstance(leftAligned, dai.ImgFrame)

        frameRgbCv = frameRgb.getCvFrame()
        # Colorize the aligned depth
        leftCv = leftAligned.getCvFrame()

        cvFrameUndistorted = cv2.undistort(
            frameRgbCv,
            np.array(rgbIntrinsics),
            np.array(rgbDistortion),
        )

        if len(leftCv.shape) == 2:
            leftCv = cv2.cvtColor(leftCv, cv2.COLOR_GRAY2BGR)
        if leftCv.shape != cvFrameUndistorted.shape:
            leftCv = cv2.resize(leftCv, (cvFrameUndistorted.shape[1], cvFrameUndistorted.shape[0]))

        blended = cv2.addWeighted(cvFrameUndistorted, rgbWeight, leftCv, leftWeight, 0)
        cv2.imshow(windowName, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break

        cfg.staticDepthPlane = staticDepthPlane
        cfgQ.send(cfg)
