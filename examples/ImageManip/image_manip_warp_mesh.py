#!/usr/bin/env python3
import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setPreviewSize(500, 500)
camRgb.setInterleaved(False)
maxFrameSize = camRgb.getPreviewWidth() * camRgb.getPreviewHeight() * 3

# Warp preview frame 1
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

# Warp preview frame 2
manip2 = pipeline.create(dai.node.ImageManip);
# Create a custom warp mesh
mesh2 = [
    (20, 20), (250, 100), (460, 20),
    (100, 250), (250, 250), (400, 250),
    (20, 480), (250, 400), (460,480)
]
manip2.setWarpMesh(mesh2, 3, 3);
manip2.setMaxOutputFrameSize(maxFrameSize);

camRgb.preview.link(manip2.inputImage);
xout2 = pipeline.create(dai.node.XLinkOut)
xout2.setStreamName('out2')
manip2.out.link(xout2.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue will be used to get the rgb frames from the output defined above
    q1 = device.getOutputQueue(name="out1", maxSize=8, blocking=False)
    q2 = device.getOutputQueue(name="out2", maxSize=8, blocking=False)

    while True:
        in1 = q1.get()
        if in1 is not None:
            cv2.imshow("Warped preview 1", in1.getCvFrame())
        in2 = q2.get()
        if in2 is not None:
            cv2.imshow("Warped preview 2", in2.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
