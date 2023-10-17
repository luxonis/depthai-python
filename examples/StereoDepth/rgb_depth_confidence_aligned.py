#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai

# Weights to use when blending rgb/depth/confidence image
rgbWeight = 0.3
depthWeight = 0.3
confWeight = 0.3
# Normalized weights to use when blending rgb/depth/confidence image (should equal 1.0)
rgbWeightNorm = 0.3
depthWeightNorm = 0.3
confWeightNorm = 0.3


def updateRgbBlendWeights(percent):
    """
    Update the rgb weight used to blend rgb/depth/confidence image

    @param[in] percent The rgb weight expressed as a percentage (0..100)
    """
    global rgbWeight
    rgbWeight = float(percent)/100.0

def updateDepthBlendWeights(percent):
    """
    Update the depth weight used to blend rgb/depth/confidence image

    @param[in] percent The depth weight expressed as a percentage (0..100)
    """
    global depthWeight
    depthWeight = float(percent)/100.0

def updateConfBlendWeights(percent):
    """
    Update the confidence weight used to blend rgb/depth/confidence image

    @param[in] percent The confidence weight expressed as a percentage (0..100)
    """
    global confWeight
    confWeight = float(percent)/100.0

# Optional. If set (True), the ColorCamera is downscaled from 1080p to 720p.
# Otherwise (False), the aligned depth is automatically upscaled to 1080p
downscaleColor = True
fps = 30
# The disparity is computed at this resolution, then upscaled to RGB resolution
monoResolution = dai.MonoCameraProperties.SensorResolution.THE_720_P

# Create pipeline
pipeline = dai.Pipeline()
device = dai.Device()
queueNames = []

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
right = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)

rgbOut = pipeline.create(dai.node.XLinkOut)
disparityOut = pipeline.create(dai.node.XLinkOut)

rgbOut.setStreamName("rgb")
queueNames.append("rgb")
disparityOut.setStreamName("disp")
queueNames.append("disp")

#Properties
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setFps(fps)
if downscaleColor: camRgb.setIspScale(2, 3)
# For now, RGB needs fixed focus to properly align with depth.
# This value was used during calibration
try:
    calibData = device.readCalibration2()
    lensPosition = calibData.getLensPosition(dai.CameraBoardSocket.CAM_A)
    if lensPosition:
        camRgb.initialControl.setManualFocus(lensPosition)
except:
    raise

left.setResolution(monoResolution)
left.setCamera("left")
left.setFps(fps)
right.setResolution(monoResolution)
right.setCamera("right")
right.setFps(fps)

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
# LR-check is required for depth alignment
stereo.setLeftRightCheck(True)
if 0: stereo.setSubpixel(True)  # TODO enable for test
stereo.setDepthAlign(dai.CameraBoardSocket.CAM_A)

xoutConfMap = pipeline.create(dai.node.XLinkOut)
xoutConfMap.setStreamName('confidence_map')

# Linking
camRgb.isp.link(rgbOut.input)
left.out.link(stereo.left)
right.out.link(stereo.right)
stereo.disparity.link(disparityOut.input)
stereo.confidenceMap.link(xoutConfMap.input)

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)

    frameRgb = None
    frameDisp = None
    frameC = None

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    rgbWindowName = "rgb"
    depthWindowName = "depth"
    confWindowName = "conf"
    blendedWindowName = "rgb-depth-conf"
    cv2.namedWindow(rgbWindowName)
    cv2.namedWindow(depthWindowName)
    cv2.namedWindow(confWindowName)
    cv2.namedWindow(blendedWindowName)
    cv2.createTrackbar('RGB Weight %', blendedWindowName, int(rgbWeight*100), 100, updateRgbBlendWeights)
    cv2.createTrackbar('Depth Weight %', blendedWindowName, int(depthWeight*100), 100, updateDepthBlendWeights)
    cv2.createTrackbar('Confidence Weight %', blendedWindowName, int(confWeight*100), 100, updateConfBlendWeights)
    while True:
        latestPacket = {}
        latestPacket["rgb"] = None
        latestPacket["disp"] = None
        latestPacket["confidence_map"] = None

        queueEvents = device.getQueueEvents(("rgb", "disp", "confidence_map"))
        for queueName in queueEvents:
            packets = device.getOutputQueue(queueName).tryGetAll()
            if len(packets) > 0:
                latestPacket[queueName] = packets[-1]

        if latestPacket["rgb"] is not None:
            frameRgb = latestPacket["rgb"].getCvFrame()
            cv2.imshow(rgbWindowName, frameRgb)

        if latestPacket["confidence_map"] is not None:
            frameC = latestPacket["confidence_map"].getCvFrame()
            cv2.imshow(confWindowName, frameC)

        if latestPacket["disp"] is not None:
            frameDisp = latestPacket["disp"].getFrame()
            maxDisparity = stereo.initialConfig.getMaxDisparity()
            # Optional, extend range 0..95 -> 0..255, for a better visualisation
            if 1: frameDisp = (frameDisp * 255. / maxDisparity).astype(np.uint8)
            # Optional, apply false colorization
            if 1: frameDisp = cv2.applyColorMap(frameDisp, cv2.COLORMAP_HOT)
            frameDisp = np.ascontiguousarray(frameDisp)
            cv2.imshow(depthWindowName, frameDisp)

        # Blend when all three frames received
        if frameRgb is not None and frameDisp is not None and frameC is not None:
            # Need to have all three frames in BGR format before blending
            if len(frameDisp.shape) < 3:
                frameDisp = cv2.cvtColor(frameDisp, cv2.COLOR_GRAY2BGR)
            if len(frameC.shape) < 3:
                frameC = cv2.cvtColor(frameC, cv2.COLOR_GRAY2BGR)
            sumWeight = rgbWeight + depthWeight + confWeight
            # Normalize the weights so their sum to be <= 1.0
            if sumWeight <= 1.0:
                rgbWeightNorm = rgbWeight
                depthWeightNorm = depthWeight
                confWeightNorm = confWeight
            else :
                rgbWeightNorm = rgbWeight / sumWeight
                depthWeightNorm = depthWeight / sumWeight
                confWeightNorm = confWeight / sumWeight
            blended1 = cv2.addWeighted(frameRgb, rgbWeightNorm, frameDisp, depthWeightNorm, 0)
            blended2 = cv2.addWeighted(blended1, rgbWeightNorm + depthWeightNorm, frameC, confWeightNorm, 0)
            cv2.imshow(blendedWindowName, blended2)
            frameRgb = None
            frameDisp = None
            frameC = None

        if cv2.waitKey(1) == ord('q'):
            break
