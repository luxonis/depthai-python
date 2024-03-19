#!/usr/bin/env python3

"""
 This example demonstrates use of queue events to block a thread until a message
 arrives to any (of the specified) queue
"""

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
camMono = pipeline.create(dai.node.MonoCamera)
xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutMono = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("rgb")
xoutMono.setStreamName("mono")

# Properties
camRgb.setInterleaved(True)
camRgb.setPreviewSize(300, 300)

# Linking
camRgb.preview.link(xoutRgb.input)
camMono.out.link(xoutMono.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Clear queue events
    device.getQueueEvents()

    while True:
        # Block until a message arrives to any of the specified queues
        queueName = device.getQueueEvent(("rgb", "mono"))

        # Getting that message from queue with name specified by the event
        # Note: number of events doesn't necessarily match number of messages in queues
        # because queues can be set to non-blocking (overwriting) behavior
        message = device.getOutputQueue(queueName).get()

        # Display arrived frames
        if type(message) == dai.ImgFrame:
            cv2.imshow(queueName, message.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
