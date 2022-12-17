#!/usr/bin/env python3
import cv2
import depthai as dai

# Connect to device
with dai.Device() as device:
    # Create pipeline
    pipeline = dai.Pipeline()
    cams = device.getConnectedCameraFeatures()
    streams = []
    for cam in cams:
        print(str(cam), str(cam.socket), cam.socket)
        c = pipeline.create(dai.node.Camera)
        x = pipeline.create(dai.node.XLinkOut)
        c.isp.link(x.input)
        c.setBoardSocket(cam.socket)
        stream = str(cam.socket)
        if cam.name:
            stream = f'{cam.name} ({stream})'
        x.setStreamName(stream)
        streams.append(stream)
    # Start pipeline
    device.startPipeline(pipeline)
    while not device.isClosed():
        queueNames = device.getQueueEvents(streams)
        for q in queueNames:
            message = device.getOutputQueue(q).get()
            # Display arrived frames
            if type(message) == dai.ImgFrame:
                cv2.imshow(q, message.getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            break
