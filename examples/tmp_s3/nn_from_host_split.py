#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
from time import monotonic

# Get argument first
parentDir = Path(__file__).parent
nnPath = str((parentDir / Path('../models/yolov4_tiny_coco_416x416_2022.1_vpux.blob')).resolve().absolute())
videoPath = str((parentDir / Path('../models/construction_vest.mp4')).resolve().absolute())
if len(sys.argv) > 2:
    nnPath = sys.argv[1]
    videoPath = sys.argv[2]

if not Path(nnPath).exists() or not Path(videoPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

# tiny yolo v4 label texts
labelMap = [
    "person",         "bicycle",    "car",           "motorbike",     "aeroplane",   "bus",           "train",
    "truck",          "boat",       "traffic light", "fire hydrant",  "stop sign",   "parking meter", "bench",
    "bird",           "cat",        "dog",           "horse",         "sheep",       "cow",           "elephant",
    "bear",           "zebra",      "giraffe",       "backpack",      "umbrella",    "handbag",       "tie",
    "suitcase",       "frisbee",    "skis",          "snowboard",     "sports ball", "kite",          "baseball bat",
    "baseball glove", "skateboard", "surfboard",     "tennis racket", "bottle",      "wine glass",    "cup",
    "fork",           "knife",      "spoon",         "bowl",          "banana",      "apple",         "sandwich",
    "orange",         "broccoli",   "carrot",        "hot dog",       "pizza",       "donut",         "cake",
    "chair",          "sofa",       "pottedplant",   "bed",           "diningtable", "toilet",        "tvmonitor",
    "laptop",         "mouse",      "remote",        "keyboard",      "cell phone",  "microwave",     "oven",
    "toaster",        "sink",       "refrigerator",  "book",          "clock",       "vase",          "scissors",
    "teddy bear",     "hair drier", "toothbrush"
]

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
nn = pipeline.create(dai.node.NeuralNetwork)
det = pipeline.create(dai.node.DetectionParser)

xinFrame = pipeline.create(dai.node.XLinkIn)
nnOut = pipeline.create(dai.node.XLinkOut)

xinFrame.setStreamName("inFrame")
nnOut.setStreamName("nn")
nn.setBlobPath(nnPath)
# Properties
det.setNNFamily(dai.DetectionNetworkType.YOLO)
det.setConfidenceThreshold(0.8)
det.setNumClasses(80)
det.setCoordinateSize(4)
det.setAnchors([10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319])
det.setAnchorMasks({"side26": [1, 2, 3], "side13": [3, 4, 5]})
det.setIouThreshold(0.2)

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

    def displayFrame(name, frame):
        for detection in detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
        # Show the frame
        cv2.imshow(name, frame)

    cap = cv2.VideoCapture(videoPath)
    while cap.isOpened():
        read_correctly, frame = cap.read()
        if not read_correctly:
            break

        img = dai.ImgFrame()
        img.setData(to_planar(frame, (416, 416)))
        img.setTimestamp(monotonic())
        img.setWidth(416)
        img.setHeight(416)
        qIn.send(img)

        inDet = qDet.get()

        if inDet is not None:
            detections = inDet.detections

        if frame is not None:
            displayFrame("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
