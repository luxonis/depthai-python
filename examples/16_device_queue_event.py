#!/usr/bin/env python3

# This example demonstrates use of queue events to block a thread until a message
# arrives to any (of the specified) queue

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Create Color and Mono cameras
camRgb = pipeline.createColorCamera()
camMono = pipeline.createMonoCamera()
# Create separate streams for them
xoutRgb = pipeline.createXLinkOut()
xoutMono = pipeline.createXLinkOut()

# Set properties
xoutRgb.setStreamName("rgb")
xoutMono.setStreamName("mono")
# Cap color camera to 5 fps
camRgb.setFps(5)
camRgb.setInterleaved(True)
camRgb.setPreviewSize(300, 300)

# Connect
camRgb.preview.link(xoutRgb.input)
camMono.out.link(xoutMono.input)


# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()
        
    # Clear queue events
    device.getQueueEvents()

    while True:
        # Block until a message arrives to any of the specified queues
        queueName = device.getQueueEvent(("rgb", "mono"))

        # Getting that message from queue with name specified by the event
        # Note: number of events doesn't necessarily match number of messages in queues
        # because queues can be set to non-blocking (overwriting) behavior
        message = device.getOutputQueue(queueName).get()

        # display arrived frames
        if type(message) == dai.ImgFrame:
            cv2.imshow(queueName, message.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
