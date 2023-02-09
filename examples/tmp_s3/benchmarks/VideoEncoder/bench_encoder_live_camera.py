#!/usr/bin/env python3
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()

cameraNode = pipeline.create(dai.node.ColorCamera)
xoutReport = pipeline.create(dai.node.XLinkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
videoEnc = pipeline.create(dai.node.VideoEncoder)


videoEnc.setDefaultProfilePreset(30, dai.VideoEncoderProperties.Profile.H264_HIGH)

cameraNode.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4000X3000)
cameraNode.setVideoSize(4000, 3000)

benchmarkIn.setNumMessagesToGet(10)

xoutReport.setStreamName("outReport")

# Linking
cameraNode.video.link(videoEnc.input)
# cameraNode.isp.link(videoEnc.input) # Encoding frames from isp works at "camera FPS"
videoEnc.bitstream.link(benchmarkIn.input)
benchmarkIn.report.link(xoutReport.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")
    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")
