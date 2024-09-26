#!/usr/bin/env python3

import depthai as dai
import cv2
from pathlib import Path
import numpy as np
import sys


nnPath = str((Path(__file__).parent / Path('../models/yolov6n_thermal_people_256x192_openvino_2022.1_6shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    nnPath = sys.argv[1]

if not Path(nnPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

labels = ["person"]

device = dai.Device()

pipeline = dai.Pipeline()
nnet = pipeline.create(dai.node.YoloDetectionNetwork)
nnet.setBlobPath(nnPath)
nnet.setConfidenceThreshold(0.5)
nnet.setNumClasses(1)
nnet.setCoordinateSize(4)
nnet.setIouThreshold(0.4)

thermalCam = pipeline.create(dai.node.Camera)
thermalCam.setBoardSocket(dai.CameraBoardSocket.CAM_E)
thermalCam.setPreviewSize(256, 192)

thermalCam.raw.link(nnet.input)

rawOut = pipeline.createXLinkOut()
rawOut.setStreamName("preview")
thermalCam.preview.link(rawOut.input)

xoutNn = pipeline.createXLinkOut()
xoutNn.setStreamName("nn")
nnet.out.link(xoutNn.input)

xoutPass = pipeline.createXLinkOut()
xoutPass.setStreamName("pass")
nnet.passthrough.link(xoutPass.input)

device.startPipeline(pipeline)

qNn = device.getOutputQueue(name="nn", maxSize=2, blocking=False)
qPass = device.getOutputQueue(name="pass", maxSize=2, blocking=False)
qPreview = device.getOutputQueue(name="preview", maxSize=2, blocking=False)

cv2.namedWindow("nnet", cv2.WINDOW_NORMAL)
cv2.namedWindow("raw", cv2.WINDOW_NORMAL)
cv2.resizeWindow("nnet", 640, 480)
cv2.resizeWindow("raw", 640, 480)

while True:
    inNn = qNn.get()
    inPass = qPass.tryGet()
    inPreview = qPreview.get()
    if inNn and inPass:
        frame = inPass.getCvFrame().astype(np.float32)
        min_, max_ = frame.min(), frame.max()
        colormappedFrame = cv2.normalize(frame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
        colormappedFrame = cv2.applyColorMap(colormappedFrame, cv2.COLORMAP_MAGMA)

        detections = inNn.detections
        for detection in detections:
            xmin = max(0.0, detection.xmin)
            ymin = max(0.0, detection.ymin)
            xmax = min(1.0, detection.xmax)
            ymax = min(1.0, detection.ymax)
            pt1 = int(xmin * 256), int(ymin * 192)
            pt2 = int(xmax * 256), int(ymax * 192)
            cv2.rectangle(colormappedFrame, pt1, pt2, (0, 255, 0))
            cv2.putText(colormappedFrame, labels[detection.label], pt1, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2, cv2.LINE_AA)
        cv2.imshow("nnet", colormappedFrame)
    if inPreview:
        cv2.imshow("raw", inPreview.getCvFrame())

    if cv2.waitKey(1) == ord("q"):
        break
