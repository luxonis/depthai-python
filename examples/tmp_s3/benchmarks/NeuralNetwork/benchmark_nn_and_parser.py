#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
from time import monotonic


BENCH_BOTH = True  # Benchmark both parser and neural network
FPS = 0  # Limit FPS (useful to measure latency at a certain FPS, to max out FPS set to 0)
PATH_INPUT_IMAGE = "/home/matevz/Pictures/croped.bmp" # Path to the image
# tiny yolo v4 label texts
labelMap = [
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train",
    "truck", "boat", "traffic light", "fire hydrant", "stop sign", "parking meter", "bench",
    "bird", "cat", "dog", "horse", "sheep", "cow", "elephant",
    "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie",
    "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite", "baseball bat",
    "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup",
    "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich",
    "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
    "chair", "sofa", "pottedplant", "bed", "diningtable", "toilet", "tvmonitor",
    "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave", "oven",
    "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors",
    "teddy bear", "hair drier", "toothbrush"
]

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
nn = pipeline.create(dai.node.NeuralNetwork)
det = pipeline.create(dai.node.DetectionParser)

xinFrame = pipeline.create(dai.node.XLinkIn)
nnOut = pipeline.create(dai.node.XLinkOut)
xOutReport = pipeline.create(dai.node.XLinkOut)
xOutReportDet = pipeline.create(dai.node.XLinkOut)
benchmarkOut = pipeline.create(dai.node.BenchmarkOut)
benchmarkOut.setFps(FPS)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
benchmarkInDetection = pipeline.create(dai.node.BenchmarkIn)
# benchmarkOut.setNumMessagesToSend(1)
benchmarkIn.setNumMessagesToGet(100)
benchmarkInDetection.setNumMessagesToGet(100)
xOutReport.setStreamName("outReport")
xOutReportDet.setStreamName("outReportDetection")

SIZE_W = 416
SIZE_H = 416
xinFrame.setStreamName("inFrame")
nnOut.setStreamName("nn")
nnPath = str((Path(__file__).parent / Path('../../../models/yolov6n_416x416_openvino2022.1_vpux.blob')).resolve().absolute())
nn.setBlobPath(nnPath)
nn.setNumInferenceThreads(4)
nn.setNumShavesPerInferenceThread(16)

det.setNNFamily(dai.DetectionNetworkType.YOLO)
det.setConfidenceThreshold(0.3)
det.setNumClasses(80)
det.setCoordinateSize(4)

det.setInputImageSize([SIZE_H, SIZE_W])
det.setIouThreshold(0.45)

if BENCH_BOTH:
    # Linking
    xinFrame.out.link(benchmarkOut.input)

    det.out.link(benchmarkInDetection.input)
    benchmarkInDetection.passthrough.link(nnOut.input)
    benchmarkInDetection.report.link(xOutReportDet.input)

    benchmarkOut.out.link(nn.input)
    nn.out.link(benchmarkIn.input)
    benchmarkIn.passthrough.link(det.input)
    benchmarkIn.report.link(xOutReport.input)
    nn.input.setQueueSize(1)
    nn.input.setBlocking(True)
    det.input.setQueueSize(10)
    det.input.setBlocking(False)
    nnOut.input.setBlocking(False)
else:
    # Linking
    xinFrame.out.link(nn.input)

    det.out.link(benchmarkInDetection.input)
    benchmarkInDetection.passthrough.link(nnOut.input)
    benchmarkInDetection.report.link(xOutReportDet.input)

    benchmarkOut.out.link(det.input)
    nn.out.link(benchmarkOut.input)
    benchmarkIn.passthrough.link(det.input)
    benchmarkIn.report.link(xOutReport.input)
    nn.input.setQueueSize(1)
    det.input.setQueueSize(10)
    det.input.setBlocking(False)
    nnOut.input.setBlocking(False)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Input queue will be used to send video frames to the device.
    qIn = device.getInputQueue(name="inFrame")
    # Output queue will be used to get nn data from the video frames.
    qDet = device.getOutputQueue(name="nn", maxSize=4, blocking=False)
    qOut = device.getOutputQueue(name="outReport")
    qOutDet = device.getOutputQueue(name="outReportDetection")
    frame = None
    detections = []

    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)


    def to_planar(arr: np.ndarray, shape: tuple) -> np.ndarray:
        return cv2.resize(arr, shape).transpose(2, 0, 1).flatten()


    def resize(arr: np.ndarray, shape: tuple) -> np.ndarray:
        return cv2.resize(arr, shape).flatten()


    def displayFrame(name, frame):
        for detection in detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5,
                        255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)

            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
        # Show the frame
        cv2.imshow(name, frame)

    img = dai.ImgFrame()
    out2 = cv2.imread(PATH_INPUT_IMAGE) # Insert a picture here
    frame = out2
    img.setData(to_planar(out2, (SIZE_W, SIZE_H)))
    img.setTimestamp(monotonic())
    img.setWidth(SIZE_W)
    img.setHeight(SIZE_H)
    img.setType(dai.ImgFrame.Type.RGB888i)
    qIn.send(img)

    while True:
        inDet = qDet.tryGet()
        report = qOut.tryGet()
        reportDet = qOutDet.tryGet()
        if report is not None:
            print(f"FPS NN is {report.fps}, average latency is {report.averageLatency * 1000} ms")

        if reportDet is not None:
            print(f"FPS Det is {reportDet.fps}, average latency is {reportDet.averageLatency * 1000} ms")

        if report is not None or reportDet is not None:
            break
        if inDet is not None:
            detections = inDet.detections

        if frame is not None:
            displayFrame("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
    if reportDet is None:
        reportDet = qOutDet.get()
        print(f"FPS Det is {reportDet.fps}, average latency is {reportDet.averageLatency * 1000} ms")
    if report is None:
        report = qOut.get()
        print(f"FPS NN is {report.fps}, average latency is {report.averageLatency * 1000} ms")
