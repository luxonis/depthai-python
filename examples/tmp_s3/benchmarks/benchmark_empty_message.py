#!/usr/bin/env python3
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()

xinFrame = pipeline.create(dai.node.XLinkIn)
xoutReport = pipeline.create(dai.node.XLinkOut)
benchmarkOut = pipeline.create(dai.node.BenchmarkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
# benchmarkOut.setNumMessagesToSend() # Specify how many messages you want to be sent
benchmarkIn.setNumMessagesToGet(10000)
xinFrame.setStreamName("inFrame")
xoutReport.setStreamName("outReport")

# Linking
xinFrame.out.link(benchmarkOut.input)
benchmarkOut.out.link(benchmarkIn.input)
benchmarkIn.report.link(xoutReport.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Input queue will be used to send video frames to the device.
    qIn = device.getInputQueue(name="inFrame")
    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")

    frame = None
    detections = []

    img = dai.ImgFrame()
    qIn.send(img)
    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")
    print(f"Average latency was {report.averageLatency * 1e6} us")
    print(f"First 10 latencies in seconds were: {report.latencies[:10]}")
