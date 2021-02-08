#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np

syncNN = True

# Get argument first
mobilenet_path = str((Path(__file__).parent / Path('models/mobilenet.blob')).resolve().absolute())
if len(sys.argv) > 1:
    mobilenet_path = sys.argv[1]

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setPreviewSize(300, 300)
cam_rgb.setInterleaved(False)

# Define a neural network that will make predictions based on the source frames
detectionNetwork = pipeline.createMobileNetDetectionNetwork()
detectionNetwork.setConfidenceThreshold(0.5)
detectionNetwork.setBlobPath(mobilenet_path)

cam_rgb.preview.link(detectionNetwork.input)

# Create outputs
xout_rgb = pipeline.createXLinkOut()
xout_rgb.setStreamName("rgb")
if(syncNN):
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
    bboxes = []


    while True:
        if(syncNN):
            in_rgb = q_rgb.get()
            in_nn = q_nn.get()
        else:
            in_rgb = q_rgb.tryGet()
            in_nn = q_nn.tryGet()

        if in_rgb is not None:
            # if the data from the rgb camera is available, transform the 1D data into a HxWxC frame
            shape = (3, in_rgb.getHeight(), in_rgb.getWidth())
            frame = in_rgb.getData().reshape(shape).transpose(1, 2, 0).astype(np.uint8)
            frame = np.ascontiguousarray(frame)

        if in_nn is not None:
            bboxes = in_nn.getDetections()


        if frame is not None:
            # if the frame is available, draw bounding boxes on it and show the frame
            height = frame.shape[0]
            width  = frame.shape[1]
            for bbox in bboxes:
                #denormalize bounging box
                x1 = int(bbox.xmin * width)
                x2 = int(bbox.xmax * width)
                y1 = int(bbox.ymin * height)
                y2 = int(bbox.ymax * height)

                cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), cv2.FONT_HERSHEY_SIMPLEX)
            cv2.imshow("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break
