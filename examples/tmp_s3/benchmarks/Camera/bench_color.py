#!/usr/bin/env python3
import depthai as dai

USE_VIDE0 = False
VIDEO_SIZE = (1920, 1080)

# Create pipeline
pipeline = dai.Pipeline()

cameraNode = pipeline.create(dai.node.ColorCamera)
xoutReport = pipeline.create(dai.node.XLinkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)

cameraNode.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4000X3000)
cameraNode.setVideoSize(VIDEO_SIZE)
benchmarkIn.setNumMessagesToGet(30)
xoutReport.setStreamName("outReport")

# Linking
if USE_VIDE0:
    cameraNode.video.link(benchmarkIn.input)
else:
    cameraNode.isp.link(benchmarkIn.input)
benchmarkIn.report.link(xoutReport.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")
    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")
