#!/usr/bin/env python3
import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setPreviewSize(500, 500)
camRgb.setInterleaved(False)
maxFrameSize = camRgb.getPreviewHeight() * camRgb.getPreviewHeight() * 3

# Warp preview frame
manip1 = pipeline.create(dai.node.ImageManip)
# Create a custom warp mesh
tl = dai.Point2f(20, 20)
tr = dai.Point2f(460, 20)
ml = dai.Point2f(100, 250)
mr = dai.Point2f(400, 250)
bl = dai.Point2f(20, 460)
br = dai.Point2f(460, 460)
manip1.setWarpMesh([tl,tr,ml,mr,bl,br], 2, 3)

manip1.setMaxOutputFrameSize(maxFrameSize)
camRgb.preview.link(manip1.inputImage)

xout1 = pipeline.create(dai.node.XLinkOut)
xout1.setStreamName('out1')
manip1.out.link(xout1.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue will be used to get the rgb frames from the output defined above
    q1 = device.getOutputQueue(name="out1", maxSize=4, blocking=False)

    while True:
        in1 = q1.get()
        if in1 is not None:
            cv2.imshow("Tile 1", in1.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
