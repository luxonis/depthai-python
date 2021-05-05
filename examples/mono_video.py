#!/usr/bin/env python3

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define source and output
monoLeft = pipeline.createMonoCamera()
xoutVideo = pipeline.createXLinkOut()

xoutVideo.setStreamName("video")

# Properties
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)

xoutVideo.input.setBlocking(False)
xoutVideo.input.setQueueSize(1)

# Linking
monoLeft.out.link(xoutVideo.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    video = device.getOutputQueue(name="video", maxSize=1, blocking=False)

    while True:
        # Get preview and video frames
        videoIn = video.get()

        # Get BGR frame from NV12 encoded video frame to show with opencv
        # Visualizing the frame on slower hosts might have overhead
        cv2.imshow("video", videoIn.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
