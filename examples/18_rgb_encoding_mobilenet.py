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

detection_nn = pipeline.createNeuralNetwork()
detection_nn.setBlobPath(mobilenet_path)
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
texts = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
         "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

with dai.Device(pipeline) as device:
    device.startPipeline()

    queue_size = 8
    q_rgb = device.getOutputQueue("rgb", queue_size)
    q_nn = device.getOutputQueue("nn", queue_size)
    q_rgb_enc = device.getOutputQueue('h265', maxSize=30, blocking=True)

    frame = None
    bboxes = []
    labels = []


    def frame_norm(frame, bbox):
        return (np.array(bbox) * np.array([*frame.shape[:2], *frame.shape[:2]])[::-1]).astype(int)


    with open('video.h265', 'wb') as videoFile:
        while True:
            in_rgb = q_rgb.tryGet()
            in_nn = q_nn.tryGet()

            while q_rgb_enc.has():
                q_rgb_enc.get().getData().tofile(videoFile)


            if in_rgb is not None:
                # if the data from the rgb camera is available, transform the 1D data into a HxWxC frame
                shape = (3, in_rgb.getHeight(), in_rgb.getWidth())
                frame = in_rgb.getData().reshape(shape).transpose(1, 2, 0).astype(np.uint8)
                frame = np.ascontiguousarray(frame)

            if in_nn is not None:
                bboxes = np.array(in_nn.getFirstLayerFp16())
                bboxes = bboxes.reshape((bboxes.size // 7, 7))
                bboxes = bboxes[bboxes[:, 2] > 0.5]
                # Cut bboxes and labels
                labels = bboxes[:, 1].astype(int)
                bboxes = bboxes[:, 3:7]

            if frame is not None:
                for raw_bbox, label in zip(bboxes, labels):
                    bbox = frame_norm(frame, raw_bbox)
                    cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
                    cv2.putText(frame, texts[label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
                cv2.imshow("rgb", frame)

            if cv2.waitKey(1) == ord('q'):
                break

print("To view the encoded data, convert the stream file (.h265) into a video file (.mp4) using a command below:")
print("ffmpeg -framerate 30 -i video.h265 -c copy video.mp4")
