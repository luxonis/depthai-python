#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - mono (grayscale) camera
camRight = pipeline.createMonoCamera()
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
camRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Create a node to convert the grayscale frame into the nn-acceptable form
manip = pipeline.createImageManip()
manip.initialConfig.setCropRect(0.4, 0.4, 0.6, 0.6)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)

camRight.out.link(manip.inputImage)

# Create outputs
manipOut = pipeline.createXLinkOut()
manipOut.setStreamName("right")
manip.out.link(manipOut.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale frames and nn data from the outputs defined above
    qRight = device.getOutputQueue("right", maxSize=4, blocking=False)

    def displayFrame(name, frame):
        cv2.imshow(name, frame)

    while True:
        inRight = qRight.get()
        frame = inRight.getCvFrame()
        displayFrame("right", frame)

        if cv2.waitKey(1) == ord('q'):
            break
