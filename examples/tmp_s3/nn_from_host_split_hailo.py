#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
from time import monotonic

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

SIZE_W = 640
SIZE_H = 640
xinFrame.setStreamName("inFrame")
nnOut.setStreamName("nn")
nn.setXmlModelPath("/home/matevz/Downloads/yolov5m.hef", "/dev/null") # Model path here
# nn.setXmlModelPath("/home/matevz/Downloads/tiny_yolov4.hef", "/dev/null")
nn.setBackend("hailo")
nn.setCustomSettings({"output_format": "float"})
# Properties
det.setNNFamily(dai.DetectionNetworkType.YOLO)
det.setConfidenceThreshold(0.3)
det.setNumClasses(80)
det.setCoordinateSize(4)
# det.setAnchors([10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319])
# det.setAnchorMasks({"side26": [1, 2, 3], "side13": [3, 4, 5]})
det.setAnchors([10,13, 16,30, 33,23, 30,61, 62,45, 59,119, 116,90, 156,198, 373,326])
det.setAnchorMasks({
    "side80": [
        0,
        1,
        2
    ],
    "side40": [
        3,
        4,
        5
    ],
    "side20": [
        6,
        7,
        8
    ]
})

det.setInputImgSize([SIZE_H, SIZE_W])
det.setIouThreshold(0.45)

# Linking
xinFrame.out.link(nn.input)
nn.out.link(det.input)
det.out.link(nnOut.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Input queue will be used to send video frames to the device.
    qIn = device.getInputQueue(name="inFrame")
    # Output queue will be used to get nn data from the video frames.
    qDet = device.getOutputQueue(name="nn", maxSize=4, blocking=False)

    frame = None
    detections = []

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
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

    while True:

        img = dai.ImgFrame()
        out2 = cv2.imread("/home/matevz/Pictures/croped.bmp") # Insert a picture here
        frame = out2
        img.setData(resize(out2, (SIZE_W, SIZE_H)))
        img.setTimestamp(monotonic())
        img.setWidth(SIZE_W)
        img.setHeight(SIZE_H)
        img.setType(dai.ImgFrame.Type.RGB888i)
        qIn.send(img)

        inDet = qDet.get()

        if inDet is not None:
            detections = inDet.detections

        if frame is not None:
            displayFrame("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
