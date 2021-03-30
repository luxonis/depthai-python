#!/usr/bin/env python3

"""
This example shows usage of Camera Control message as well as MononCamera configInput to change crop x and y
Uses 'WASD' controls to move the crop window, 'C' to capture a still image, 'T' to trigger autofocus, 'IOKL,.'
for manual exposure/focus:
  Control:      key[dec/inc]  min..max
  exposure time:     I   O      1..33000 [us]
  sensitivity iso:   K   L    100..1600
  focus:             ,   .      0..255 [far..near]
To go back to auto controls:
  'E' - autoexposure
  'F' - autofocus (continuous)
"""


import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) camera
camRight = pipeline.createMonoCamera()
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
camRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

camLeft = pipeline.createMonoCamera()
camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Create a node to convert the grayscale frame into the nn-acceptable form
manipRight = pipeline.createImageManip()
manipRight.initialConfig.setCropRect(0.4, 0.4, 0.6, 0.6)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manipRight.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)

manipLeft = pipeline.createImageManip()
manipLeft.initialConfig.setCropRect(0.4, 0.4, 0.6, 0.6)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manipLeft.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)

camRight.out.link(manipRight.inputImage)
camLeft.out.link(manipLeft.inputImage)

# Create outputs
manipOutRight = pipeline.createXLinkOut()
manipOutRight.setStreamName("right")
manipRight.out.link(manipOutRight.input)

manipOutLeft = pipeline.createXLinkOut()
manipOutLeft.setStreamName("left")
manipLeft.out.link(manipOutLeft.input)

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale frames and nn data from the outputs defined above
    qRight = device.getOutputQueue(manipOutRight.getStreamName(), maxSize=4, blocking=False)
    qLeft = device.getOutputQueue(manipOutLeft.getStreamName(), maxSize=4, blocking=False)

    def displayFrame(name, frame):
        cv2.imshow(name, frame)

    while True:
        inRight = qRight.get()
        inLeft = qLeft.get()
        frameRight = inRight.getCvFrame()
        frameLeft = inLeft.getCvFrame()
        displayFrame("right", frameRight)
        displayFrame("left", frameLeft)

        if cv2.waitKey(1) == ord('q'):
            break
