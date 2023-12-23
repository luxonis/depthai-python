#!/usr/bin/env python3
import cv2
import depthai as dai

class Host(dai.Node):
    def __run__(self, image: dai.message.ImgFrame):
        cv2.imshow('still', image.getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            raise KeyboardInterrupt()

class Script(dai.Script):
    input_desc = {}
    output_desc = {"out": dai.message.CameraControl}
    def __script__(node):
        import time
        ctrl = CameraControl()
        ctrl.setCaptureStill(True)
        while True:
            time.sleep(1)
            node.io['out'].send(ctrl)

pipeline = dai.Pipeline()
with pipeline:
   Host(dai.node.ColorCamera(inputControl=Script()).still)
dai.run(pipeline)
