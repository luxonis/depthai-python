#!/usr/bin/env python3
import depthai as dai
import time

color_atributes = ["video", "isp"]
video_size=(1920, 1080)

cams_to_test = [
    {
        "name": "rgb",
        "mono": False,
        "socket": dai.CameraBoardSocket.RGB,
        "resolution": dai.ColorCameraProperties.SensorResolution.THE_4000X3000,
        "fps": 30,
    },
    {
        "name": "night",
        "mono": False,
        "socket": dai.CameraBoardSocket.CAM_D,
        "resolution": dai.ColorCameraProperties.SensorResolution.THE_4000X3000,
        "fps": 30,
    },
]


def add_cam(pipeline, props, colorOut):
    xoutReport = pipeline.create(dai.node.XLinkOut)
    benchmarkIn = pipeline.create(dai.node.BenchmarkIn)
    videoEnc = pipeline.create(dai.node.VideoEncoder)
    videoEnc.setDefaultProfilePreset(30, dai.VideoEncoderProperties.Profile.H264_MAIN)
    benchmarkIn.setNumMessagesToGet(10)
    streamName = props["name"] + " - " + colorOut
    xoutReport.setStreamName(streamName)
    cameraNode = pipeline.create(dai.node.ColorCamera)
    # Change this to preview/video whatever is needed
    if colorOut == "isp":
        cameraNode.isp.link(videoEnc.input)
    elif colorOut == "video":
        cameraNode.setVideoSize(video_size)
        cameraNode.video.link(videoEnc.input)
    cameraNode.setResolution(props["resolution"])
    cameraNode.setFps(props["fps"])
    cameraNode.setBoardSocket(props["socket"])
    videoEnc.bitstream.link(benchmarkIn.input)
    benchmarkIn.report.link(xoutReport.input)
    return streamName

for color_out in color_atributes:
    # Create pipeline
    pipeline = dai.Pipeline()
    streamNames = []
    for props in cams_to_test:
        streamName = add_cam(pipeline, props, color_out)
        streamNames.append(streamName)

    # Connect to device and start pipeline
    with dai.Device(pipeline) as device:
        # Output queue to receive the report back from the device
        qOuts = [device.getOutputQueue(name=streamName) for streamName in streamNames]
        reports = [qOut.get() for qOut in qOuts]
        for i, report in enumerate(reports):
            print(f"Stream: {streamNames[i]}")
            print(f"Got {report.fps} FPS, in {report.timeTotal} seconds for {report.numMessagesReceived} messages.")
    time.sleep(7)
    print("----------------------------------------------\n\n\n")
