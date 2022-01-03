#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np
from projector_3d import PointCloudVisualizer


pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
spatialLocationCalculator = pipeline.create(dai.node.SpatialLocationCalculator)
#camRgb = pipeline.create(dai.node.ColorCamera)

xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutSpatialData = pipeline.create(dai.node.XLinkOut)
#xoutRgb = pipeline.create(dai.node.XLinkOut)
xinSpatialCalcConfig = pipeline.create(dai.node.XLinkIn)
xoutRight = pipeline.create(dai.node.XLinkOut)

xoutDepth.setStreamName("depth")
xoutSpatialData.setStreamName("spatialData")
xinSpatialCalcConfig.setStreamName("spatialCalcConfig")
#xoutRgb.setStreamName("rgb")
xoutRight.setStreamName("right")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

#camRgb.setPreviewSize(1280, 720)
#camRgb.setInterleaved(False)
#camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.RGB)

lrcheck = False
subpixel = False

stereo.initialConfig.setConfidenceThreshold(255)
stereo.setLeftRightCheck(lrcheck)
stereo.setSubpixel(subpixel)

config = dai.SpatialLocationCalculatorConfigData()
config.depthThresholds.lowerThreshold = 100
config.depthThresholds.upperThreshold = 10000
spatialLocationCalculator.setWaitForConfigInput(False)
spatialLocationCalculator.initialConfig.addROI(config)

monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
spatialLocationCalculator.passthroughDepth.link(xoutDepth.input)
stereo.depth.link(spatialLocationCalculator.inputDepth)
spatialLocationCalculator.out.link(xoutSpatialData.input)
xinSpatialCalcConfig.out.link(spatialLocationCalculator.inputConfig)
#camRgb.preview.link(xoutRgb.input)
monoRight.out.link(xoutRight.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    depthQueue = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
    spatialCalcQueue = device.getOutputQueue(name="spatialData", maxSize=4, blocking=False)
    #qRgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)
    # spatialCalcConfigInQueue = device.getInputQueue("spatialCalcConfig")
    calibData = device.readCalibration()
    right_intrinsic = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, 1280, 720))
    color = (255, 255, 255)
    pcl_converter = PointCloudVisualizer(right_intrinsic, 1280, 720)
    while True:
        inDepth = depthQueue.get()  # Blocking call, will wait until a new data has arrived
        depthFrame = inDepth.getFrame()
        depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
        depthFrameColor = cv2.equalizeHist(depthFrameColor)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)
        #inRgb = qRgb.get()
        #frame_rgb = inRgb.getCvFrame()
        inRight = qRight.tryGet()
        frame_right = inRight.getCvFrame()

        # Show the frame
        cv2.imshow("depth", depthFrameColor)
        #cv2.imshow("rgb", frame_rgb)
        cv2.imshow("right", frame_right)
        pcl_converter.rgbd_to_projection(depthFrameColor, frame_right, False)
        pcl_converter.visualize_pcd()

        # End program
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
