#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
xout = pipeline.create(dai.node.XLinkOut)
xoutLeft = pipeline.create(dai.node.XLinkOut)

xout.setStreamName("disparity")
xoutLeft.setStreamName("left")

# Properties
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("left")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setCamera("right")

stereo.enableDistortionCorrection(True)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
stereo.disparity.link(xout.input)
stereo.rectifiedLeft.link(xoutLeft.input)

cvColorMap = cv2.applyColorMap(np.arange(256, dtype=np.uint8), cv2.COLORMAP_JET)
cvColorMap[0] = [0, 0, 0]

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    try:
        calibration = device.readCalibration()
    except:
        print("Device is not calibrated!")
        exit()

    print("Press 'u' to update distortion coefficients with random values")

    # Output queue will be used to get the disparity frames from the outputs defined above
    q = device.getOutputQueue(name="disparity", maxSize=4, blocking=False)
    qLeft = device.getOutputQueue(name="left", maxSize=4, blocking=False)

    while True:
        inDisparity = q.get()  # blocking call, will wait until a new data has arrived
        frame = inDisparity.getFrame()
        # Normalization for better visualization
        frame = (frame * (255 / stereo.initialConfig.getMaxDisparity())).astype(np.uint8)

        cv2.imshow("disparity", frame)

        frame = cv2.applyColorMap(frame, cvColorMap)
        cv2.imshow("disparity_color", frame)

        inLeft = qLeft.get()
        frame = inLeft.getCvFrame()
        cv2.imshow("rectified left", frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('u'):
            randomDistortionCoeffs = np.random.rand(14)
            calibration.setDistortionCoefficients(dai.CameraBoardSocket.LEFT, randomDistortionCoeffs)
            try:
                device.setCalibration(calibration)
            except:
                print("Failed to update calibration!")
            try:
                updatedCalib = device.getCalibration()
                distortionCoeffs = updatedCalib.getDistortionCoefficients(dai.CameraBoardSocket.LEFT)
                print("Updated distortion coefficients: ", distortionCoeffs)
            except:
                pass

