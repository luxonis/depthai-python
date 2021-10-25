#!/usr/bin/env python3
import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

cam = pipeline.create(dai.node.ColorCamera)
cam.initialControl.setManualFocus(130)
# Not needed, you can display 1080P frames as well
cam.setIspScale(1,2)

# Script node
script = pipeline.create(dai.node.Script)
script.setScript("""
    ctrl = CameraControl()
    ctrl.setCaptureStill(True)
    # Initially send still event
    node.io['ctrl'].send(ctrl)

    normal = True
    while True:
        frame = node.io['frames'].get()
        if normal:
            ctrl.setAutoExposureCompensation(3)
            node.io['stream1'].send(frame)
            normal = False
        else:
            ctrl.setAutoExposureCompensation(-3)
            node.io['stream2'].send(frame)
            normal = True
        node.io['ctrl'].send(ctrl)
""")
cam.still.link(script.inputs['frames'])

# XLinkOut
xout1 = pipeline.create(dai.node.XLinkOut)
xout1.setStreamName('stream1')
script.outputs['stream1'].link(xout1.input)

xout2 = pipeline.create(dai.node.XLinkOut)
xout2.setStreamName('stream2')
script.outputs['stream2'].link(xout2.input)

script.outputs['ctrl'].link(cam.inputControl)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    qStream1 = device.getOutputQueue("stream1")
    qStream2 = device.getOutputQueue("stream2")
    while True:
        cv2.imshow('stream1', qStream1.get().getCvFrame())
        cv2.imshow('stream2', qStream2.get().getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            break