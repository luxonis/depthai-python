#!/usr/bin/env python3

import time
from pathlib import Path
import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)

xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutRgb.setStreamName("rgb")
camRgb.video.link(xoutRgb.input)

xin = pipeline.create(dai.node.XLinkIn)
xin.setStreamName("control")
xin.out.link(camRgb.inputControl)

# Properties
videoEnc = pipeline.create(dai.node.VideoEncoder)
videoEnc.setDefaultProfilePreset(1, dai.VideoEncoderProperties.Profile.MJPEG)
camRgb.still.link(videoEnc.input)

# Linking
xoutStill = pipeline.create(dai.node.XLinkOut)
xoutStill.setStreamName("still")
videoEnc.bitstream.link(xoutStill.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the rgb frames from the output defined above
    qRgb = device.getOutputQueue(name="rgb", maxSize=30, blocking=False)
    qStill = device.getOutputQueue(name="still", maxSize=30, blocking=True)
    qControl = device.getInputQueue(name="control")

    # Make sure the destination path is present before starting to store the examples
    dirName = "rgb_data"
    Path(dirName).mkdir(parents=True, exist_ok=True)

    while True:
        inRgb = qRgb.tryGet()  # Non-blocking call, will return a new data that has arrived or None otherwise
        if inRgb is not None:
            frame = inRgb.getCvFrame()
            # 4k / 4
            frame = cv2.pyrDown(frame)
            frame = cv2.pyrDown(frame)
            cv2.imshow("rgb", frame)

        if qStill.has():
            fName = f"{dirName}/{int(time.time() * 1000)}.jpeg"
            with open(fName, "wb") as f:
                f.write(qStill.get().getData())
                print('Image saved to', fName)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('c'):
            ctrl = dai.CameraControl()
            ctrl.setCaptureStill(True)
            qControl.send(ctrl)
            print("Sent 'still' event to the camera!")
