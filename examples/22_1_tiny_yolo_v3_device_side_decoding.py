#!/usr/bin/env python3

"""
Tiny-yolo-v3 device side decoding demo
  YOLO v3 Tiny is a real-time object detection model implemented with Keras* from
  this repository <https://github.com/david8862/keras-YOLOv3-model-set> and converted
  to TensorFlow* framework. This model was pretrained on COCO* dataset with 80 classes.
"""

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
import time

# tiny yolo v3 label texts
nn_labels = ["person",         "bicycle",    "car",           "motorbike",     "aeroplane",   "bus",           "train",
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
             "teddy bear",     "hair drier", "toothbrush"]  


syncNN = True

# Get argument first
tiny_yolo_v3_path = str((Path(__file__).parent / Path('models/tiny-yolo-v3_openvino_2021.2_6shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    tiny_yolo_v3_path = sys.argv[1]

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setPreviewSize(416, 416)
cam_rgb.setInterleaved(False)
cam_rgb.setFps(40)

# network specific settings
detectionNetwork = pipeline.createYoloDetectionNetwork()
detectionNetwork.setConfidenceThreshold(0.5)
detectionNetwork.setNumClasses(80)
detectionNetwork.setCoordinateSize(4)
detectionNetwork.setAnchors(np.array([10,14, 23,27, 37,58, 81,82, 135,169, 344,319]))
detectionNetwork.setAnchorMasks({"side26": np.array([1, 2, 3]), "side13": np.array([3, 4, 5])})
detectionNetwork.setIouThreshold(0.5)

detectionNetwork.setBlobPath(tiny_yolo_v3_path)
detectionNetwork.setNumInferenceThreads(2)
detectionNetwork.input.setBlocking(False)

cam_rgb.preview.link(detectionNetwork.input)

# Create outputs
xout_rgb = pipeline.createXLinkOut()
xout_rgb.setStreamName("rgb")
if syncNN:
    detectionNetwork.passthrough.link(xout_rgb.input)
else:
    cam_rgb.preview.link(xout_rgb.input)

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("detections")
detectionNetwork.out.link(xout_nn.input)


# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the rgb frames and nn data from the outputs defined above
    q_rgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    q_nn = device.getOutputQueue(name="detections", maxSize=4, blocking=False)

    frame = None
    detections = []

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frame_norm(frame, bbox):
        norm_vals = np.full(len(bbox), frame.shape[0])
        norm_vals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * norm_vals).astype(int)

    def display_frame(name, frame):
        for detection in detections:
            bbox = frame_norm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
            cv2.putText(frame, nn_labels[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
        cv2.imshow(name, frame)

    start_time = time.monotonic()
    counter = 0

    while True:
        if syncNN:
            in_rgb = q_rgb.get()
            in_nn = q_nn.get()
        else:
            in_rgb = q_rgb.tryGet()
            in_nn = q_nn.tryGet()

        if in_rgb is not None:
            frame = in_rgb.getCvFrame()
            cv2.putText(frame, "NN fps: {:.2f}".format(counter / (time.monotonic() - start_time)),
                        (2, frame.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color=(255, 255, 255))

        if in_nn is not None:
            detections = in_nn.detections
            counter += 1

        if frame is not None:
            display_frame("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
