#!/usr/bin/env python3

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
camRgb = pipeline.createColorCamera()
camRgb.setPreviewSize(300, 300)
camRgb.setBoardSocket(dai.CameraBoardSocket.RGB)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setInterleaved(True)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)

# Create output
xoutVideo = pipeline.createXLinkOut()
xoutVideo.setStreamName("video")
xoutPreview = pipeline.createXLinkOut()
xoutPreview.setStreamName("preview")

camRgb.preview.link(xoutPreview.input)
camRgb.video.link(xoutVideo.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    while True:
        # Get preview and video frames
        preview = device.getOutputQueue('preview').get()
        video = device.getOutputQueue('video').get()

        # Show 'preview' frame as is (already in correct format, no copy is made)
        cv2.imshow("preview", preview.getFrame())
        # Get BGR frame from NV12 encoded video frame to show with opencv
        cv2.imshow("video", video.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
