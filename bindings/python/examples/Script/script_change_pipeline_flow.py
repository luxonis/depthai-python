#!/usr/bin/env python3
import depthai as dai
import cv2
from pathlib import Path
import numpy as np

parentDir = Path(__file__).parent
nnPath = str((parentDir / Path('../models/mobilenet-ssd_openvino_2021.4_5shave.blob')).resolve().absolute())

pipeline = dai.Pipeline()

cam = pipeline.createColorCamera()
cam.setBoardSocket(dai.CameraBoardSocket.CAM_A)
cam.setInterleaved(False)
cam.setIspScale(2,3)
cam.setVideoSize(720,720)
cam.setPreviewSize(300,300)

xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutRgb.setStreamName('rgb')
cam.video.link(xoutRgb.input)

script = pipeline.createScript()

xin = pipeline.create(dai.node.XLinkIn)
xin.setStreamName('in')
xin.out.link(script.inputs['toggle'])

cam.preview.link(script.inputs['rgb'])
script.setScript("""
    toggle = False
    while True:
        msg = node.io['toggle'].tryGet()
        if msg is not None:
            toggle = msg.getData()[0]
            node.warn('Toggle! Perform NN inferencing: ' + str(toggle))

        frame = node.io['rgb'].get()

        if toggle:
            node.io['nn'].send(frame)
""")

nn = pipeline.create(dai.node.MobileNetDetectionNetwork)
nn.setBlobPath(nnPath)
script.outputs['nn'].link(nn.input)

xoutNn = pipeline.create(dai.node.XLinkOut)
xoutNn.setStreamName('nn')
nn.out.link(xoutNn.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    inQ = device.getInputQueue("in")
    qRgb = device.getOutputQueue("rgb")
    qNn = device.getOutputQueue("nn")

    runNn = False

    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

    color = (255, 127, 0)
    def drawDetections(frame, detections):
        for detection in detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, 2)


    while True:
        frame = qRgb.get().getCvFrame()

        if qNn.has():
            detections = qNn.get().detections
            drawDetections(frame, detections)

        cv2.putText(frame, f"NN inferencing: {runNn}", (20,20), cv2.FONT_HERSHEY_TRIPLEX, 0.7, color)
        cv2.imshow('Color frame', frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('t'):
            runNn = not runNn
            print(f"{'Enabling' if runNn else 'Disabling'} NN inferencing")
            buf = dai.Buffer()
            buf.setData(runNn)
            inQ.send(buf)