#!/usr/bin/env python3
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()

xinFrame = pipeline.create(dai.node.XLinkIn)
benchmarkOut = pipeline.create(dai.node.BenchmarkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
benchmarkOut.setNumMessagesToSend(5000)
benchmarkIn.setNumMessagesToGet(5000)
xinFrame.setStreamName("inFrame")

# Linking
xinFrame.out.link(benchmarkOut.input)
benchmarkOut.out.link(benchmarkIn.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Input queue will be used to send video frames to the device.
    qIn = device.getInputQueue(name="inFrame")

    frame = None
    detections = []

    img = dai.ImgFrame()
    qIn.send(img)

    import time
    while True:
       time.sleep(1)