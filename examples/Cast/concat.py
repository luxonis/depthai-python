import numpy as np
import cv2
import depthai as dai
from pathlib import Path

SHAPE = 300

p = dai.Pipeline()

camRgb = p.create(dai.node.ColorCamera)
left = p.create(dai.node.MonoCamera)
right = p.create(dai.node.MonoCamera)
manipLeft = p.create(dai.node.ImageManip)
manipRight = p.create(dai.node.ImageManip)
nn = p.create(dai.node.NeuralNetwork)
cast = p.create(dai.node.Cast)
castXout = p.create(dai.node.XLinkOut)

camRgb.setPreviewSize(SHAPE, SHAPE)
camRgb.setInterleaved(False)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)

left.setCamera("left")
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
manipLeft.initialConfig.setResize(SHAPE, SHAPE)
manipLeft.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

right.setCamera("right")
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
manipRight.initialConfig.setResize(SHAPE, SHAPE)
manipRight.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

nnBlobPath = (Path(__file__).parent / Path('../models/concat_openvino_2021.4_6shave.blob')).resolve().absolute()
nn.setBlobPath(nnBlobPath)
nn.setNumInferenceThreads(2)

castXout.setStreamName("cast")
cast.setOutputFrameType(dai.ImgFrame.Type.BGR888p)

# Linking
left.out.link(manipLeft.inputImage)
right.out.link(manipRight.inputImage)
manipLeft.out.link(nn.inputs['img1'])
camRgb.preview.link(nn.inputs['img2'])
manipRight.out.link(nn.inputs['img3'])
nn.out.link(cast.input)
cast.output.link(castXout.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(p) as device:
    qCast = device.getOutputQueue(name="cast", maxSize=4, blocking=False)

    while True:
        inCast = qCast.get()
        assert isinstance(inCast, dai.ImgFrame)
        cv2.imshow("Concated frames", inCast.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
