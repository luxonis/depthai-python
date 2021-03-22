#!/usr/bin/env python3

"""
Mono camera control demo:
  Control:      key[dec/inc]  min..max
  exposure time:     I   O      1..33000 [us]
  sensitivity iso:   K   L    100..1600
Back to autoexposure: 'E'
"""

import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) cameras
camLeft = pipeline.createMonoCamera()
camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

camRight = pipeline.createMonoCamera()
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
camRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

# Create outputs
xoutLeft = pipeline.createXLinkOut()
xoutLeft.setStreamName('left')
camLeft.out.link(xoutLeft.input)
xoutRight = pipeline.createXLinkOut()
xoutRight.setStreamName('right')
camRight.out.link(xoutRight.input)

# Create and link control input
control_in = pipeline.createXLinkIn()
control_in.setStreamName('control')
control_in.out.link(camLeft.inputControl)
control_in.out.link(camRight.inputControl)


def clamp(num, v0, v1): return max(v0, min(num, v1))


# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale frames from the outputs defined above
    qLeft = device.getOutputQueue(name="left", maxSize=4, blocking=False)
    qRight = device.getOutputQueue(name="right", maxSize=4, blocking=False)
    # Input queue for control commands
    controlQueue = device.getInputQueue('control')

    # Manual exposure: key-press step, defaults and limits
    EXP_STEP = 500  # us
    ISO_STEP = 50

    exp_time = 20000
    expMin = 1
    expMax = 33000

    sensIso = 800
    sensMin = 100
    sensMax = 1600

    frameLeft = None
    frameRight = None

    while True:
        # instead of get (blocking) used tryGet (nonblocking) which will return the available data or None otherwise
        inLeft = qLeft.tryGet()
        inRight = qRight.tryGet()

        if inLeft is not None:
            frameLeft = inLeft.getCvFrame()

        if inRight is not None:
            frameRight = inRight.getCvFrame()

        # show the frames if available
        if frameLeft is not None:
            cv2.imshow("left", frameLeft)
        if frameRight is not None:
            cv2.imshow("right", frameRight)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key in [ord('i'), ord('o'), ord('k'), ord('l')]:
            if key == ord('i'):
                exp_time -= EXP_STEP
            if key == ord('o'):
                exp_time += EXP_STEP
            if key == ord('k'):
                sensIso -= ISO_STEP
            if key == ord('l'):
                sensIso += ISO_STEP
            exp_time = clamp(exp_time, expMin, expMax)
            sensIso = clamp(sensIso, sensMin, sensMax)
            print("Setting manual exposure, time:", exp_time, "iso:", sensIso)
            ctrl = dai.CameraControl()
            ctrl.setManualExposure(exp_time, sensIso)
            controlQueue.send(ctrl)
        elif key == ord('e'):
            print("Autoexposure enable")
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureEnable()
            controlQueue.send(ctrl)
