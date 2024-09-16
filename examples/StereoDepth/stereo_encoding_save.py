#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from pathlib import Path
import json
import datetime

MAX_LUT_SIZE = 96 * 32 * 2
SAVE_UNENCODED_DEPTH = True

currentPath = Path(__file__).resolve().parent
currentTimeAndDate = datetime.datetime.now().strftime("%Y-%m-%d_%H.%M.%S")
outputDirectory = currentPath / ("output_" + currentTimeAndDate)
outputDirectory.mkdir(parents=True, exist_ok=False)

inW = 1280
inH = 800
pipeline = dai.Pipeline()

leftSocket = dai.CameraBoardSocket.LEFT
rightSocket = dai.CameraBoardSocket.RIGHT

colorLeft = pipeline.create(dai.node.ColorCamera)
colorLeft.setBoardSocket(leftSocket)
colorLeft.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
colorRight = pipeline.create(dai.node.ColorCamera)
colorRight.setBoardSocket(rightSocket)
colorRight.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1200_P)
colorLeft.setVideoSize(inW, inH)
colorRight.setVideoSize(inW, inH)
leftOut = colorLeft.video
rightOut = colorRight.video

xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
stereo = pipeline.create(dai.node.StereoDepth)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDepth.setStreamName("depth")

# Linking
leftOut.link(stereo.left)
rightOut.link(stereo.right)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
stereo.depth.link(xoutDepth.input)
stereo.setLeftRightCheck(False)
stereo.setSubpixel(True)
stereo.setSubpixelFractionalBits(5)
stereo.setConfidenceThreshold(230)

depthEncoder = pipeline.create(dai.node.DepthEncoder)
stereo.depth.link(depthEncoder.input)
depthEncoderOut = pipeline.create(dai.node.XLinkOut)
depthEncoderOut.setStreamName("depthColorized")
depthEncoder.output.link(depthEncoderOut.input)

depthVideoEncoder = pipeline.create(dai.node.VideoEncoder)
depthVideoEncoder.setDefaultProfilePreset(1280, 800, 30, dai.VideoEncoderProperties.Profile.H264_HIGH) # Change to MJPEG later
depthVideoEncoderOut = pipeline.create(dai.node.XLinkOut)
depthVideoEncoderOut.setStreamName("depthEncodedColorized")
depthEncoder.output.link(depthVideoEncoder.input)
depthVideoEncoder.bitstream.link(depthVideoEncoderOut.input)

# Get the scaling factor
device = dai.Device()
calibration = device.readCalibration()
fx = calibration.getCameraIntrinsics(leftSocket, inW, inH)[0][0]
baseline = max(np.abs(calibration.getCameraTranslationVector(leftSocket, rightSocket)))
print("fx: ", fx, " baseline: ", baseline)
dispMultiplier = baseline * fx * 32 * 10 # 32 is the number of subpixles and 10 is the conversion from cm to mm

# depthEncoder.setLut(lutR, lutG, lutB)
bufferColorSpace = 0.1
maxDepth = 3000
minDepth = 0
depthEncoder.setHueLut(minDepth, maxDepth, dispMultiplier, bufferColorSpace) # 0 - 3000mm - the unit has to match the unit of stereo node
depthEncoder.setOutputType(dai.ImgFrame.Type.NV12)

# Create a json with the depth config
recordingConfig = {
    "dispMultiplier": dispMultiplier,
    "colorMap": "HUE",
    "bufferColorSpace": bufferColorSpace,
    "minDepth": minDepth,
    "maxDepth": maxDepth,
    "depthUnit": "mm", # Default, change if you change the unit in the stereo node
}

# Save the config to json
with open(outputDirectory / "recordingConfig.json", 'w') as f:
    json.dump(recordingConfig, f, indent=4)

# Save the calibration to json
calibration.eepromToJsonFile(outputDirectory / "calibration.json")
# Connect to device and start pipeline
with device, open(outputDirectory / "encoded_depth.h265", 'w') as encodedFile:
    device.startPipeline(pipeline)
    qDepth = device.getOutputQueue("depth", 4, False)
    qEncoded = device.getOutputQueue("depthColorized", 4, False)
    qDepthEncoded = device.getOutputQueue("depthEncodedColorized", 4, False)
    qLeft = device.getOutputQueue("left", 4, False)
    qRight = device.getOutputQueue("right", 4, False)
    while True:
        inDepth = qDepth.get()

        data = np.array(inDepth.getData())
        count = np.count_nonzero(data % 32 == 0)
        frameDepth = inDepth.getCvFrame()
        disp = (frameDepth / 32).astype(np.uint8)
        cv2.imshow("disp", disp)
        encoded = qEncoded.get()
        print(len(encoded.getData()))
        encodedFrame = encoded.getCvFrame()
        cv2.imshow("encoded", encodedFrame)
        encodedDepth = qDepthEncoded.get()
        print(len(encodedDepth.getData()))
        encodedDepthFrame = encodedDepth.getData().tofile(encodedFile)
        disp2DepthMultiplier = inDepth.get().disp2DepthMultiplier
        if(abs(disp2DepthMultiplier- dispMultiplier) > 0.1):
            print("disp2DepthMultiplier: ", disp2DepthMultiplier, " dispMultiplier: ", dispMultiplier)
            print("The multipliers should be the same, but differ, the saved depth will not be valid!")
            raise RuntimeError("The multipliers should be the same, but differ, the saved depth will not be valid!")

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
