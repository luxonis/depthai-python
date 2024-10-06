#!/usr/bin/env python3
import depthai as dai


# Create pipeline
pipeline = dai.Pipeline()

# xinFrame = pipeline.create(dai.node.XLinkIn)
xoutReport = pipeline.create(dai.node.XLinkOut)
benchmarkOut = pipeline.create(dai.node.BenchmarkOut)
benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
camRgb = pipeline.create(dai.node.ColorCamera)
imageManip = pipeline.create(dai.node.ImageManip)

benchmarkIn.setNumMessagesToGet(20)

camRgb.setBoardSocket(dai.CameraBoardSocket.RGB)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4000X3000)
camRgb.setVideoSize(800, 600)

imageManipConfig = dai.ImageManipConfig()
imageManip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)
imageManip.setMaxOutputFrameSize(416 * 416 * 3)
imageManip.inputImage.setBlocking(False)
imageManip.inputImage.setQueueSize(2)
imageManip.initialConfig.setResize((416, 416))

xoutReport.setStreamName("outReport")

# Linking
camRgb.video.link(benchmarkOut.input)
benchmarkOut.out.link(imageManip.inputImage)

imageManip.out.link(benchmarkIn.input)
benchmarkIn.report.link(xoutReport.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue to receive the report back from the device
    qOut = device.getOutputQueue(name="outReport")

    frame = None
    detections = []

    report = qOut.get()
    print(f"Got {report.fps} FPS, in {report.timeTotal} s for {report.numMessagesReceived} messages.")
