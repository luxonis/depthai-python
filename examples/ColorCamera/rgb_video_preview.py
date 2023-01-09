#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutVideo = pipeline.create(dai.node.XLinkOut)
xoutPreview = pipeline.create(dai.node.XLinkOut)

xoutVideo.setStreamName("video")
xoutPreview.setStreamName("preview")

# Properties
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4000X3000)
camRgb.setVideoSize(1920, 1080)
camRgb.setPreviewSize(300, 300)

xoutVideo.input.setBlocking(False)
xoutVideo.input.setQueueSize(1)

xoutPreview.input.setBlocking(False)
xoutPreview.input.setQueueSize(1)

# Linking
camRgb.video.link(xoutVideo.input)
camRgb.preview.link(xoutPreview.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    video = device.getOutputQueue(name="video", maxSize=1, blocking=False)
    preview = device.getOutputQueue(name="preview", maxSize=1, blocking=False)

    while True:
        videoIn = video.get()
        previewIn = preview.get()

        # Get BGR frame from NV12 encoded video frame to show with opencv
        # Visualizing the frame on slower hosts might have overhead
        cv2.imshow("video", videoIn.getCvFrame())
        cv2.imshow("preview", previewIn.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
