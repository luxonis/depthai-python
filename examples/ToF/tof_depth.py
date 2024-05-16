#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

pipeline = dai.Pipeline()

camToF = pipeline.create(dai.node.Camera)

# We assume the ToF camera sensor is on port CAM_A
camToF.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camToF.setFps(60) # The actual FPS is /2

tof = pipeline.create(dai.node.ToF)

# Link the ToF sensor to the ToF node
camToF.raw.link(tof.input)

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("depth")
tof.depth.link(xout.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    print('Connected cameras:', device.getConnectedCameraFeatures())
    q = device.getOutputQueue(name="depth")

    while True:
        imgFrame = q.get()  # blocking call, will wait until a new data has arrived
        depthMap = imgFrame.getFrame()

        # Colorize the depth frame to jet colormap
        depthDownscaled = depthMap[::4]
        nonZeroDepth = depthDownscaled[depthDownscaled != 0] # Remove invalid depth values
        if len(nonZeroDepth) == 0:
            minDepth, maxDepth = 0, 0
        else:
            minDepth = np.percentile(nonZeroDepth, 3)
            maxDepth = np.percentile(nonZeroDepth, 97)
        depth_colorized = np.interp(depthMap, (minDepth, maxDepth), (0, 255)).astype(np.uint8)
        depth_colorized = cv2.applyColorMap(depth_colorized, cv2.COLORMAP_JET)

        cv2.imshow("Colorized depth", depth_colorized)

        if cv2.waitKey(1) == ord('q'):
            break
