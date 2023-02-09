#!/usr/bin/env python3
import depthai as dai

# This test is likely very unrealistic, since the video encoder is always getting the same image.

pipeline = dai.Pipeline()
videoEnc = pipeline.create(dai.node.VideoEncoder)
videoEnc.setDefaultProfilePreset(30, dai.VideoEncoderProperties.Profile.H264_HIGH)


def fill_out_pipeline(pipeline, videoEnc):
    cameraNode = pipeline.create(dai.node.ColorCamera)

    xoutReport = pipeline.create(dai.node.XLinkOut)
    benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
    benchmarkOut = pipeline.create(dai.node.BenchmarkOut)

    cameraNode.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4000X3000)
    cameraNode.setVideoSize(4000, 3000)

    benchmarkOut.input.setBlocking(False)
    benchmarkIn.setNumMessagesToGet(30)
    xoutReport.setStreamName("outReport")

    # Linking
    cameraNode.video.link(benchmarkOut.input)
    benchmarkOut.out.link(videoEnc.input)
    videoEnc.bitstream.link(benchmarkIn.input)
    benchmarkIn.report.link(xoutReport.input)

fill_out_pipeline(pipeline, videoEnc)
# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")
    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")