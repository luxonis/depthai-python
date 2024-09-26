#!/usr/bin/env python3

import cv2
import depthai as dai
from pathlib import Path

SHAPE = 720

p = dai.Pipeline()

camRgb = p.create(dai.node.ColorCamera)
nn = p.create(dai.node.NeuralNetwork)
script = p.create(dai.node.Script)
rgbXout = p.create(dai.node.XLinkOut)
cast = p.create(dai.node.Cast)
castXout = p.create(dai.node.XLinkOut)

camRgb.setVideoSize(SHAPE, SHAPE)
camRgb.setPreviewSize(SHAPE, SHAPE)
camRgb.setInterleaved(False)

nnBlobPath = (Path(__file__).parent / Path('../models/diff_openvino_2022.1_6shave.blob')).resolve().absolute()
nn.setBlobPath(nnBlobPath)

script.setScript("""
old = node.io['in'].get()
while True:
    frame = node.io['in'].get()
    node.io['img1'].send(old)
    node.io['img2'].send(frame)
    old = frame
""")

rgbXout.setStreamName("rgb")
castXout.setStreamName("cast")
cast.setOutputFrameType(dai.RawImgFrame.Type.GRAY8)

# Linking
camRgb.preview.link(script.inputs['in'])
script.outputs['img1'].link(nn.inputs['img1'])
script.outputs['img2'].link(nn.inputs['img2'])
camRgb.video.link(rgbXout.input)
nn.out.link(cast.input)
cast.output.link(castXout.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(p) as device:
    qCam = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    qCast = device.getOutputQueue(name="cast", maxSize=4, blocking=False)


    while True:
        colorFrame = qCam.get()
        assert isinstance(colorFrame, dai.ImgFrame)
        cv2.imshow("Color", colorFrame.getCvFrame())

        inCast = qCast.get()
        assert isinstance(inCast, dai.ImgFrame)
        cv2.imshow("Diff", inCast.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
