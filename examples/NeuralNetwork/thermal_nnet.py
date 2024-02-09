import depthai as dai
import cv2
from pathlib import Path
import numpy as np


device = dai.Device()
pipeline = dai.Pipeline()
nnet = pipeline.create(dai.node.YoloDetectionNetwork)
nnet.setBlobPath(Path("~/Downloads/yolov6n_sim.blob").expanduser())
nnet.setConfidenceThreshold(0.1)
nnet.setNumClasses(6)
nnet.setCoordinateSize(4)
nnet.setIouThreshold(0.5)

thermal_cam = pipeline.create(dai.node.Camera)
thermal_cam.setBoardSocket(dai.CameraBoardSocket.CAM_E)
thermal_cam.setPreviewSize(256, 192)

thermal_cam.raw.link(nnet.input)
xout_raw = pipeline.createXLinkOut()
xout_raw.setStreamName("raw")

xin_nn = pipeline.createXLinkIn()
xin_nn.setStreamName("in_nn")
xin_nn.out.link(nnet.input)


raw_out = pipeline.createXLinkOut()
raw_out.setStreamName("preview")
thermal_cam.preview.link(raw_out.input)


xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("nn")
nnet.out.link(xout_nn.input)

xout_pass = pipeline.createXLinkOut()
xout_pass.setStreamName("pass")
nnet.passthrough.link(xout_pass.input)

device.startPipeline(pipeline)

q_nn = device.getOutputQueue(name="nn", maxSize=2, blocking=False)
q_pass = device.getOutputQueue(name="pass", maxSize=2, blocking=False)
q_preview = device.getOutputQueue(name="preview", maxSize=2, blocking=False)
q_in_nn = device.getInputQueue("in_nn")
q_raw = device.getOutputQueue(name="raw", maxSize=2, blocking=False)


cv2.namedWindow("nnet", cv2.WINDOW_NORMAL)
cv2.namedWindow("raw", cv2.WINDOW_NORMAL)
cv2.resizeWindow("nnet", 640, 480)
cv2.resizeWindow("raw", 640, 480)
labels = ["walk_person", "ride_person", "squat_person", "people", "person?", "people?"]

while True:
    in_nn = q_nn.get()
    in_pass = q_pass.tryGet()
    in_preview = q_preview.get()
    if in_nn and in_pass:
        frame = in_pass.getCvFrame().astype(np.float32)
        min_, max_ = frame.min(), frame.max()
        colormapped_frame = cv2.normalize(
            frame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U
        )
        colormapped_frame = cv2.applyColorMap(colormapped_frame, cv2.COLORMAP_MAGMA)

        detections = in_nn.detections
        for detection in detections:
            xmin = max(0.0, detection.xmin)
            ymin = max(0.0, detection.ymin)
            xmax = min(1.0, detection.xmax)
            ymax = min(1.0, detection.ymax)
            pt1 = int(xmin * 256), int(ymin * 192)
            pt2 = int(xmax * 256), int(ymax * 192)
            cv2.rectangle(colormapped_frame, pt1, pt2, (0, 255, 0))
            cv2.putText(
                colormapped_frame,
                labels[detection.label],
                pt1,
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (0, 255, 0),
                2,
                cv2.LINE_AA,
            )
        cv2.imshow("nnet", colormapped_frame)
    if in_preview:
        cv2.imshow("raw", in_preview.getCvFrame())
    if cv2.waitKey(1) == ord("q"):
        break
