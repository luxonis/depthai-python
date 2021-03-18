#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np

# Get argument first
mobilenet_path = str((Path(__file__).parent / Path('models/mobilenet.blob')).resolve().absolute())
if len(sys.argv) > 1:
    mobilenet_path = sys.argv[1]


pipeline = dai.Pipeline()

cam = pipeline.createColorCamera()
cam.setBoardSocket(dai.CameraBoardSocket.RGB)
cam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
cam.setPreviewSize(300, 300)
cam.setInterleaved(False)

videoEncoder = pipeline.createVideoEncoder()
videoEncoder.setDefaultProfilePreset(1920, 1080, 30, dai.VideoEncoderProperties.Profile.H265_MAIN)
cam.video.link(videoEncoder.input)

detection_nn = pipeline.createMobileNetDetectionNetwork()
detection_nn.setConfidenceThreshold(0.5)
detection_nn.setBlobPath(mobilenet_path)
detection_nn.setNumInferenceThreads(2)
detection_nn.input.setBlocking(False)
cam.preview.link(detection_nn.input)

videoOut = pipeline.createXLinkOut()
videoOut.setStreamName('h265')
videoEncoder.bitstream.link(videoOut.input)

xout_rgb = pipeline.createXLinkOut()
xout_rgb.setStreamName("rgb")
cam.preview.link(xout_rgb.input)

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("nn")
detection_nn.out.link(xout_nn.input)

# MobilenetSSD label texts
nn_labels = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
             "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

with dai.Device(pipeline) as device, open('video.h265', 'wb') as videoFile:
    device.startPipeline()

    queue_size = 8
    q_rgb = device.getOutputQueue("rgb", queue_size)
    q_nn = device.getOutputQueue("nn", queue_size)
    q_rgb_enc = device.getOutputQueue('h265', maxSize=30, blocking=True)

    frame = None
    detections = []


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


    while True:
        in_rgb = q_rgb.tryGet()
        in_nn = q_nn.tryGet()

        while q_rgb_enc.has():
            q_rgb_enc.get().getData().tofile(videoFile)

        if in_rgb is not None:
            frame = in_rgb.getCvFrame()

        if in_nn is not None:
            detections = in_nn.detections

        if frame is not None:
            display_frame("rgb", frame)

        if cv2.waitKey(1) == ord('q'):
            break

print("To view the encoded data, convert the stream file (.h265) into a video file (.mp4) using a command below:")
print("ffmpeg -framerate 30 -i video.h265 -c copy video.mp4")
