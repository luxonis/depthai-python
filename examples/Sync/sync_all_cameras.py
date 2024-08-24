#!/usr/bin/env python3

import cv2
import depthai as dai

with dai.Device() as device:
    pipeline = dai.Pipeline()
    cams = device.getConnectedCameraFeatures()
    sync = pipeline.create(dai.node.Sync)
    for cam in cams:
        print(str(cam), str(cam.socket), cam.socket)
        cam_node = pipeline.create(dai.node.Camera)
        cam_node.setBoardSocket(cam.socket)
        cam_node.isp.link(sync.inputs[str(cam.socket)])

    xout = pipeline.create(dai.node.XLinkOut)
    xout.setStreamName('sync')
    sync.out.link(xout.input)

    # Start pipeline
    device.startPipeline(pipeline)
    q = device.getOutputQueue('sync', maxSize=10, blocking=False)
    while not device.isClosed():
        msgs = q.get()
        for (name, imgFrame) in msgs:
            print(f'Cam {name}, seq {imgFrame.getSequenceNum()}, tiemstamp {imgFrame.getTimestamp()}')
            cv2.imshow(name, imgFrame.getCvFrame())
        print('----')

        if cv2.waitKey(1) == ord('q'):
            break
