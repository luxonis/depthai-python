#!/usr/bin/env python3
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()
cameraNode = pipeline.create(dai.node.MonoCamera)
cameraNode.setResolution(dai.MonoCameraProperties.SensorResolution.THE_800_P)
cameraNode.setFps(120)

xoutReport = pipeline.create(dai.node.XLinkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)

benchmarkIn.setNumMessagesToGet(50)
xoutReport.setStreamName("outReport")

# Linking
cameraNode.out.link(benchmarkIn.input)
benchmarkIn.report.link(xoutReport.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")
    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")

