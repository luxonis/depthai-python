#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setPreviewSize(1000, 500)
camRgb.setInterleaved(False)
maxFrameSize = camRgb.getPreviewHeight() * camRgb.getPreviewWidth() * 3

# In this example we use 2 imageManips for splitting the original 1000x500
# preview frame into 2 500x500 frames
manip1 = pipeline.create(dai.node.ImageManip)
manip1.initialConfig.setCropRect(0, 0, 0.5, 1)
manip1.setMaxOutputFrameSize(maxFrameSize)
camRgb.preview.link(manip1.inputImage)

manip2 = pipeline.create(dai.node.ImageManip)
manip2.initialConfig.setCropRect(0.5, 0, 1, 1)
manip2.setMaxOutputFrameSize(maxFrameSize)
camRgb.preview.link(manip2.inputImage)

xout1 = pipeline.create(dai.node.XLinkOut)
xout1.setStreamName('out1')
manip1.out.link(xout1.input)

xout2 = pipeline.create(dai.node.XLinkOut)
xout2.setStreamName('out2')
manip2.out.link(xout2.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue will be used to get the rgb frames from the output defined above
    q1 = device.getOutputQueue(name="out1", maxSize=4, blocking=False)
    q2 = device.getOutputQueue(name="out2", maxSize=4, blocking=False)

    while True:
        if q1.has():
            cv2.imshow("Tile 1", q1.get().getCvFrame())

        if q2.has():
            cv2.imshow("Tile 2", q2.get().getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
