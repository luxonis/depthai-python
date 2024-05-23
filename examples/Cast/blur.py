import depthai as dai
import cv2
from pathlib import Path

SHAPE = 300

p = dai.Pipeline()

camRgb = p.create(dai.node.ColorCamera)
nn = p.create(dai.node.NeuralNetwork)
rgbOut = p.create(dai.node.XLinkOut)
cast = p.create(dai.node.Cast)
castXout = p.create(dai.node.XLinkOut)

camRgb.setPreviewSize(SHAPE, SHAPE)
camRgb.setInterleaved(False)

nnBlobPath = str((Path(__file__).parent / Path('../models/blur_simplified_openvino_2021.4_6shave.blob')).resolve().absolute())

nn.setBlobPath(nnBlobPath)

rgbOut.setStreamName("rgb")

castXout.setStreamName("cast")

cast.setOutputFrameType(dai.RawImgFrame.Type.BGR888p)

# Linking
camRgb.preview.link(nn.input)
camRgb.preview.link(rgbOut.input)
nn.out.link(cast.input)
cast.output.link(castXout.input)

with dai.Device(p) as device:
    qCam = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    qCast = device.getOutputQueue(name="cast", maxSize=4, blocking=False)


    while True:
        inCast = qCast.get()
        assert isinstance(inCast, dai.ImgFrame)
        inRgb = qCam.get()
        assert isinstance(inRgb, dai.ImgFrame)
        cv2.imshow("Blur", inCast.getCvFrame())
        cv2.imshow("Original", inRgb.getCvFrame())


        if cv2.waitKey(1) == ord('q'):
            break
