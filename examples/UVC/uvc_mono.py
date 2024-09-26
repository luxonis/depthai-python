#!/usr/bin/env python3

import time

import depthai as dai

pipeline = dai.Pipeline()

# Define a source - left mono (grayscale) camera
mono_left = pipeline.createMonoCamera()

mono_left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
mono_left.setBoardSocket(dai.CameraBoardSocket.CAM_B)

# Create an UVC (USB Video Class) output node
uvc = pipeline.createUVC()

# Manip for frame type conversion
manip = pipeline.createImageManip()
manip.initialConfig.setResize(1280, 720)
manip.initialConfig.setFrameType(dai.RawImgFrame.Type.NV12)
manip.setMaxOutputFrameSize(int(1280*720*1.5))

# Linking
manip.out.link(uvc.input)
mono_left.out.link(manip.inputImage)

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