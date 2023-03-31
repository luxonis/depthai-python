#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
tofCam = pipeline.create(dai.node.ColorCamera)
tofProcess = pipeline.create(dai.node.TofCamera)

#isp
xoutVideo = pipeline.create(dai.node.XLinkOut)

xoutPasstrough = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)

xoutPasstrough.setStreamName("passtroughTof")
xoutDepth.setStreamName("depth")
#isp
xoutVideo.setStreamName("video")
                     
# Properties
tofCam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1280X962)
tofCam.setBoardSocket(dai.CameraBoardSocket.CAM_E)
tofProcess.setTofModel(dai.TofCameraProperties.TofSensorModel.TOF_SAMSUNG33D)
tofProcess.setRawSize(1280, 962)
tofProcess.setDepthSize(640, 480)

# Linking
tofCam.raw.link(tofProcess.inputRaw)
tofProcess.passthroughInputRaw.link(xoutPasstrough.input)
#isp
tofCam.isp.link(xoutVideo.input)

#tofCam.initialControl.setManualExposure(100,100)

xoutPasstrough.input.setBlocking(False)
xoutPasstrough.input.setQueueSize(1)

tofProcess.depth.link(xoutDepth.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    print('Connected cameras:', device.getConnectedCameraFeatures())
    # Print out usb speed
    print('Usb speed:', device.getUsbSpeed().name)
    # Bootloader version
    if device.getBootloaderVersion() is not None:
        print('Bootloader version:', device.getBootloaderVersion())
    # Device name
    print('Device name:', device.getDeviceName())

    # Output queue will be used to get the rgb frames from the output defined above
    qPassthrough = device.getOutputQueue(name="passtroughTof", maxSize=4, blocking=False)
    video = device.getOutputQueue(name="video", maxSize=4, blocking=False)

    qDepth = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    while True:
        print("videoin")
        videoIn = video.get()

        inPassthrough = qPassthrough.get()  # blocking call, will wait until a new data has arrived
        inDepth = qDepth.get()  # blocking call, will wait until a new data has arrived
        print(inPassthrough.getData())
        # Preview
        cv2.imshow("passtroughTof(raw8)", (inPassthrough.getFrame()).clip(0, 255).astype(np.uint8))

        #cv2.imshow(name, (inDepth.getFrame() // 10).clip(0, 255).astype(np.uint8))
        name = "depth"
        if inDepth.getSequenceNum() % 2 :
            name += " amplitude/confidence"
        #cv2.imshow(name, (inDepth.getFrame()))
        cv2.imshow(name, (inDepth.getFrame()).clip(0, 255).astype(np.uint8))

        if cv2.waitKey(1) == ord('q'):
            break
