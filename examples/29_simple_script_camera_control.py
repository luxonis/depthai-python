#!/usr/bin/env python3
import cv2
import depthai as dai
import numpy as np
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam = pipeline.create(dai.node.ColorCamera)

# Script node
script = pipeline.create(dai.node.Script)
script.setScriptData("""
import time
ctrl = CameraControl()
ctrl.setCaptureStill(True)
while True:
    time.sleep(1)
    node.io['out'].send(ctrl)
""")

# XLinkOut
xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('still')

# Connections
script.outputs['out'].link(cam.inputControl)
cam.still.link(xout.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()
    while True:
        img = device.getOutputQueue("still").get()
        cv2.imshow('still', img.getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            exit(0)
