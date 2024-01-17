#!/usr/bin/env python3

import time

import depthai as dai

pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) cameras
mono_left = pipeline.createMonoCamera()
mono_right = pipeline.createMonoCamera()

mono_left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
mono_left.setBoardSocket(dai.CameraBoardSocket.CAM_B)

mono_right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
mono_right.setBoardSocket(dai.CameraBoardSocket.CAM_C)

# Create stereo depth
stereo = pipeline.createStereoDepth()
stereo.initialConfig.setConfidenceThreshold(255)
stereo.setLeftRightCheck(True)
# 0.190 values, better for visualization
stereo.setExtendedDisparity(True)

# Create an UVC (USB Video Class) output node
uvc = pipeline.createUVC()

# Manip for frame type conversion
manip = pipeline.createImageManip()
manip.initialConfig.setResize(1280, 720)
manip.initialConfig.setFrameType(dai.ImgFrame.Type.NV12)
manip.initialConfig.setColormap(dai.Colormap.STEREO_TURBO, stereo.initialConfig.getMaxDisparity())
manip.setMaxOutputFrameSize(int(1280*720*1.5))

# Linking
mono_left.out.link(stereo.left)
mono_right.out.link(stereo.right)
stereo.disparity.link(manip.inputImage)
manip.out.link(uvc.input)

# Note: if the pipeline is sent later to device (using startPipeline()),
# it is important to pass the device config separately when creating the device
config = dai.Device.Config()
config.board.uvc = dai.BoardConfig.UVC(1280, 720)
config.board.uvc.frameType = dai.ImgFrame.Type.NV12
# config.board.uvc.cameraName = "My Custom Cam"
pipeline.setBoardConfig(config.board)


# Standard UVC load with depthai
with dai.Device(pipeline) as device:
    # Dot projector
    device.setIrLaserDotProjectorBrightness(765)
    print("\nDevice started, please keep this process running")
    print("and open an UVC viewer to check the camera stream.")
    print("\nTo close: Ctrl+C")

    # Doing nothing here, just keeping the host feeding the watchdog
    while True:
        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            break