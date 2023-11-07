#!/usr/bin/env python3

import time
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-fb', '--flash-bootloader', default=False, action="store_true")
parser.add_argument('-f',  '--flash-app',        default=False, action="store_true")
parser.add_argument('-l',  '--load-and-exit',    default=False, action="store_true")
args = parser.parse_args()

if args.load_and_exit:
    import os
    # Disabling device watchdog, so it doesn't need the host to ping periodically.
    # Note: this is done before importing `depthai`
    os.environ["DEPTHAI_WATCHDOG"] = "0"

import depthai as dai

def getPipeline():
   
    pipeline = dai.Pipeline()

    # Define a source - two mono (grayscale) cameras
    mono_left = pipeline.createMonoCamera()
    mono_right = pipeline.createMonoCamera()

    mono_left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
    mono_left.setBoardSocket(dai.CameraBoardSocket.LEFT)

    mono_right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
    mono_right.setBoardSocket(dai.CameraBoardSocket.RIGHT)

    # Create stereo depth
    stereo = pipeline.createStereoDepth()
    stereo.initialConfig.setConfidenceThreshold(255)

    # Create an UVC (USB Video Class) output node
    uvc = pipeline.createUVC()

    # Manip for frame type conversion
    manip = pipeline.createImageManip()
    manip.initialConfig.setResize(1280, 720)
    manip.initialConfig.setFrameType(dai.RawImgFrame.Type.NV12)
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


    return pipeline

# Will flash the bootloader if no pipeline is provided as argument
def flash(pipeline=None):
    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    bootloader = dai.DeviceBootloader(bl, True)

    # Create a progress callback lambda
    progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')

    startTime = time.monotonic()
    if pipeline is None:
        print("Flashing bootloader...")
        bootloader.flashBootloader(progress)
    else:
        print("Flashing application pipeline...")
        bootloader.flash(progress, pipeline)

    elapsedTime = round(time.monotonic() - startTime, 2)
    print("Done in", elapsedTime, "seconds")

if args.flash_bootloader or args.flash_app:
    if args.flash_bootloader: flash()
    if args.flash_app: flash(getPipeline())
    print("Flashing successful. Please power-cycle the device")
    quit()

if args.load_and_exit:
    device = dai.Device(getPipeline())
    print("\nDevice started. Attempting to force-terminate this process...")
    print("Open an UVC viewer to check the camera stream.")
    print("To reconnect with depthai, a device power-cycle may be required in some cases")
    # We do not want the device to be closed, so terminate the process uncleanly.
    # (TODO add depthai API to be able to cleanly exit without closing device)
    import signal
    os.kill(os.getpid(), signal.SIGTERM)

# Standard UVC load with depthai
with dai.Device(getPipeline()) as device:
    print("\nDevice started, please keep this process running")
    print("and open an UVC viewer to check the camera stream.")
    print("\nTo close: Ctrl+C")

    # Doing nothing here, just keeping the host feeding the watchdog
    while True:
        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            break
