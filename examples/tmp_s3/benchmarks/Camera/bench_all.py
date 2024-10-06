#!/usr/bin/env python3
import depthai as dai
import time

color_atributes = ["preview", "video", "isp"]
video_size=(4000,3000)
preview_size=(500,500)

cams_to_test = [
    {
        "name": "left",
        "mono": True,
        "socket": dai.CameraBoardSocket.LEFT,
        "resolution": dai.MonoCameraProperties.SensorResolution.THE_800_P,
        "fps": 30,
    },
    {
        "name": "right",
        "mono": True,
        "socket": dai.CameraBoardSocket.RIGHT,
        "resolution": dai.MonoCameraProperties.SensorResolution.THE_800_P,
        "fps": 30,
    },
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
    benchmarkIn.setNumMessagesToGet(10)
    streamName = props["name"] + " - " + colorOut
    xoutReport.setStreamName(streamName)
    if props["mono"]:
        cameraNode = pipeline.create(dai.node.MonoCamera)
        cameraNode.out.link(benchmarkIn.input)
    else:
        cameraNode = pipeline.create(dai.node.ColorCamera)
        # Change this to preview/video whatever is needed
        if colorOut == "isp":
            cameraNode.isp.link(benchmarkIn.input)
        elif colorOut == "video":
            cameraNode.setVideoSize(video_size)
            cameraNode.video.link(benchmarkIn.input)
        elif colorOut == "preview":
            cameraNode.setPreviewSize(preview_size)
            cameraNode.preview.link(benchmarkIn.input)
    cameraNode.setResolution(props["resolution"])
    cameraNode.setFps(props["fps"])
    cameraNode.setBoardSocket(props["socket"])
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
