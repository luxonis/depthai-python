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

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setPreviewSize(300, 300)    # NN input
cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)
cam_rgb.setInterleaved(False)
cam_rgb.setPreviewKeepAspectRatio(False)

# Define a neural network that will make predictions based on the source frames
detection_nn = pipeline.createNeuralNetwork()
detection_nn.setBlobPath(mobilenet_path)
cam_rgb.preview.link(detection_nn.input)

# Create outputs
xout_video = pipeline.createXLinkOut()
xout_video.setStreamName("video")
cam_rgb.video.link(xout_video.input)

xout_preview = pipeline.createXLinkOut()
xout_preview.setStreamName("preview")
cam_rgb.preview.link(xout_preview.input)

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("nn")
detection_nn.out.link(xout_nn.input)

# Pipeline defined, now the device is assigned and pipeline is started
device = dai.Device(pipeline)
device.startPipeline()

# Output queues will be used to get the frames and nn data from the outputs defined above
q_video = device.getOutputQueue(name="video", maxSize=4, blocking=False)
q_preview = device.getOutputQueue(name="preview", maxSize=4, blocking=False)
q_nn = device.getOutputQueue(name="nn", maxSize=4, blocking=False)

preview_frame = None
video_frame = None
bboxes = []


# nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
def frame_norm(frame, bbox):
    return (np.array(bbox) * np.array([*frame.shape[:2], *frame.shape[:2]])[::-1]).astype(int)


def display_frame(name, frame, bboxes):
    for raw_bbox in bboxes:
        bbox = frame_norm(frame, raw_bbox)
        cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]),
                      (255, 0, 0), 2)
    cv2.imshow(name, frame)


while True:
    # instead of get (blocking) used tryGet (nonblocking) which will return the available data or None otherwise
    in_video = q_video.tryGet()
    in_preview = q_preview.tryGet()
    in_nn = q_nn.tryGet()

    if in_video is not None:
        # if the data from the video camera is available, transform the 1D data into a HxWxC frame
        packetData = in_video.getData()
        w = in_video.getWidth()
        h = in_video.getHeight()
        yuv420p = packetData.reshape((h * 3 // 2, w))
        video_frame = cv2.cvtColor(yuv420p, cv2.COLOR_YUV2BGR_NV12)

    if in_preview is not None:
        shape = (3, in_preview.getHeight(), in_preview.getWidth())
        preview_frame = in_preview.getData().reshape(shape).transpose(1, 2, 0).astype(np.uint8)
        preview_frame = np.ascontiguousarray(preview_frame)

    if in_nn is not None:
        # one detection has 7 numbers, and the last detection is followed by -1 digit, which later is filled with 0
        bboxes = np.array(in_nn.getFirstLayerFp16())
        # take only the results before -1 digit
        bboxes = bboxes[:np.where(bboxes == -1)[0][0]]
        # transform the 1D array into Nx7 matrix
        bboxes = bboxes.reshape((bboxes.size // 7, 7))
        # filter out the results which confidence less than a defined threshold
        bboxes = bboxes[bboxes[:, 2] > 0.5][:, 3:7]

    # if the frame is available, draw bounding boxes on it and show the frame
    if video_frame is not None:
        display_frame("video", video_frame, bboxes)

    if preview_frame is not None:
        display_frame("preview", preview_frame, bboxes)

    if cv2.waitKey(1) == ord('q'):
        break
