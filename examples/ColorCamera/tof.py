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
    qPassthrough = device.getOutputQueue(name="passtroughTof", maxSize=4, blocking=True)
    video = device.getOutputQueue(name="video", maxSize=1, blocking=True)

    qDepth = device.getOutputQueue(name="depth", maxSize=4, blocking=True)

    while True:
        print("==videoin")
        videoIn = video.get()

        inPassthrough = qPassthrough.get()  # blocking call, will wait until a new data has arrived
        inDepth = qDepth.get()  # blocking call, will wait until a new data has arrived
        print(inPassthrough.getData())
        # Retrieve 'bgr' (opencv format) frame
        cv2.imshow("passtroughTof", (inPassthrough.getFrame()).clip(0, 255).astype(np.uint8))
        # depth in cm
        cv2.imshow("depth",(inDepth.getFrame()).clip(0, 255).astype(np.uint8))
        #w = 240
        #cv2.imshow("depth-" + str(inDepth.getSequenceNum() % 4), (inDepth.getFrame() // 10).clip(0, 255).astype(np.uint8).reshape(640 * 120 // w,w))
        #cv2.imshow("depth-" + str(inDepth.getSequenceNum() % 4), (inDepth.getFrame() // 10).clip(0, 255).astype(np.uint8))
        #cv2.imshow("depth-" + str(inDepth.getSequenceNum() % 4), (inDepth.getFrame()).clip(0, 255).astype(np.uint8))
        #cv2.imshow("video", videoIn.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break
