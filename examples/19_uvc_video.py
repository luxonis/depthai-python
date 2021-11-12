#!/usr/bin/env python3

import depthai as dai
import numpy as np
import argparse
import time
import cv2

parser = argparse.ArgumentParser()
parser.add_argument('-fb', '--flash-bootloader', default=False, action="store_true")
parser.add_argument('-f',  '--flash-app',        default=False, action="store_true")
parser.add_argument('-r',  '--rotate-cam',       default=False, action="store_true")
parser.add_argument('-b',  '--back-mic',         default=False, action="store_true")
parser.add_argument('-xm', '--xlink-mic',        default=False, action="store_true")
parser.add_argument('-xc', '--xlink-cam',        default=False, action="store_true")
parser.add_argument('-nc', '--no-camera',        default=False, action="store_true")
parser.add_argument('-g',  '--mic-gain-db',      default=0, type=float)
parser.add_argument('-cd', '--create-dap', type=str, const='upgrade.dap', nargs='?',
                    help='Create a DepthAI Application Package file, optionally '
                         'specifying the file name/path (default: %(const)s)')
parser.add_argument('-fd', '--flash-dap', type=str, const='upgrade.dap', nargs='?',
                    help='Flash a DepthAI Application Package file, optionally '
                         'specifying the file name/path (default: %(const)s)')

args = parser.parse_args()
args.back_mic = False  # TODO again for UAC. Available with XLink

enable_4k = True

def try_reset_booted_device():
    try:
        import usb.core
    except ModuleNotFoundError:
        print("WARN The device might be already booted up with UVC firmware")
        print("==== Please install `pyusb` to allow auto-resetting it:")
        print("     python3 -m pip install pyusb")
        print()
        return
    dev = usb.core.find(idVendor=0x03e7, idProduct=0xf63b)
    if dev is not None:
        print("Attempting to reset already booted device")
        dev.ctrl_transfer(0x00, 0xF5, 0x0DA1, 0x0000)
        # Waiting a bit for device to boot (required when flashing)
        tstart = time.monotonic()
        while True:
            dev1 = usb.core.find(idVendor=0x03e7, idProduct=0xf63c)  # bootloader
            dev2 = usb.core.find(idVendor=0x03e7, idProduct=0x2485)  # bootROM
            if dev1 is not None or dev2 is not None:
                break
            if time.monotonic() - tstart > 3:
                raise RuntimeError("Failed to find device after reset. Please power-cycle")

def switch_to_usb_boot():
    switch_done = False
    try:
        import usb.core
    except ModuleNotFoundError:
        print("==== Please install `pyusb` for bootloader flash checks:")
        print("     python3 -m pip install pyusb")
        raise RuntimeError("'pyusb' required to ensure no soft-bricking happens")
    dev = usb.core.find(idVendor=0x03e7, idProduct=0xf63c)  # bootloader
    if dev is not None:
        print("Device in bootloader, resetting to USB bootROM before upgrade")
        try:
            dai.Device(dai.Pipeline(), "non-existing-file.bin")
        except RuntimeError as e:
            if not str(e).startswith("Error path"):
                raise
        # Wait for the new device to show up
        tstart = time.monotonic()
        while True:
            dev = usb.core.find(idVendor=0x03e7, idProduct=0x2485)  # bootROM
            if dev is not None:
                switch_done = True
                break
            if time.monotonic() - tstart > 3:
                raise RuntimeError("Failed to find USB bootROM device. Please run again")
    return switch_done

# This is used only if old 2.1-based FW is running (from flash)
try_reset_booted_device()

# Start defining a pipeline
pipeline = dai.Pipeline()

if not args.no_camera:
    # Define a source - color camera
    cam_rgb = pipeline.createColorCamera()
    cam_rgb.setBoardSocket(dai.CameraBoardSocket.RGB)
    cam_rgb.setInterleaved(False)
    cam_rgb.setPreviewSize(1920, 1080)
    cam_rgb.initialControl.setManualFocus(130)
    # Note: this file contains tuning info for all cameras, but for now setting it
    #       at pipeline level is not supported, so we set it for ColorCamera
    #cam_rgb.setCameraTuningBlobPath('/home/user/customTuning.bin')

    if enable_4k:
        cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)
        cam_rgb.setIspScale(1, 2)
    else:
        cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)

    if args.rotate_cam:
        cam_rgb.setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)

    if args.flash_app:
        # Create an UVC (USB Video Class) output node
        # Would hang when the pipeline is USB-loaded, as UVC descriptor is disabled
        uvc = pipeline.createUVC()
        cam_rgb.video.link(uvc.input)
        if 1:  # Set custom GPIOs
            uvc.setGpiosOnInit(      {58:0, 37:0, 34:0})
            uvc.setGpiosOnStreamOn(  {58:1, 37:1, 34:1})
            uvc.setGpiosOnStreamOff( {58:0, 37:0, 34:0})
        else:  # Safe test with SoM LED
            uvc.setGpiosOnInit(      {56:0})
            uvc.setGpiosOnStreamOn(  {56:1})
            uvc.setGpiosOnStreamOff( {56:0})

# Create an UAC (USB Audio Class) node
uac = pipeline.createUAC()
uac.setStreamBackMic(args.back_mic)
uac.setMicGainDecibels(args.mic_gain_db)
print("UAC using:", "Back mic," if args.back_mic else "Front mics,",
      "Gain {} dB".format(args.mic_gain_db))

uac.setXlinkApplyMicGain(True)
uac.setXlinkSampleSizeBytes(3)

#uac.setMicAutoGain(True) # Not yet implemented

filename = "audio.raw"
if args.xlink_mic:
    xout = pipeline.createXLinkOut()
    xout.setStreamName("mic")
    uac.out.link(xout.input)
    print("Writing XLink audio data to:", filename)

if args.xlink_cam:
    xout = pipeline.createXLinkOut()
    xout.setStreamName("cam")
    cam_rgb.video.link(xout.input)

if args.flash_bootloader or args.flash_app or args.create_dap or args.flash_dap:
    #if args.flash_bootloader:
        # TODO! Still needed with latest bootloader? No transition to bootROM otherwise!
        #did_switch = switch_to_usb_boot()  # safety measure

    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    bootloader = dai.DeviceBootloader(bl, True)

    # Workaround for a bug with the timeout-enabled bootloader
    progressCalled = False

    def progress(p):
        global progressCalled
        progressCalled = True
        print(f'Flashing progress: {p*100:.1f}%')

    if args.create_dap:
        path = args.create_dap
        bootloader.saveDepthaiApplicationPackage(path, pipeline)
        print("Created DAP file:", path)
        quit()

    startTime = time.monotonic()
    if args.flash_bootloader:
        print("Flashing bootloader...")
        bootloader.flashBootloader(progress)
        #if did_switch:
        #    print("Bootloader upgraded, please do a power-cycle test after everything is flashed")
    elif args.flash_app or args.flash_dap:
        if args.flash_app:
            print("Flashing application pipeline...")
            bootloader.flash(progress, pipeline)
        else:
            path = args.flash_dap
            print("Flashing DAP file:", path)
            content = np.fromfile(path, dtype=np.uint8)
            bootloader.flashDepthaiApplicationPackage(progress, content)
    if not progressCalled:
        raise RuntimeError('Flashing failed, please try again')
    elapsedTime = round(time.monotonic() - startTime, 2)
    print("Done in", elapsedTime, "seconds. Exiting.")
    quit()

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device, open(filename, "wb") as f:
    # Start pipeline
    tstart = time.monotonic()
    device.startPipeline()

    print("\nDevice started, please keep this process running")
    print("and open an UVC viewer. Example on Linux:")
    print("    guvcview -d /dev/video0")
    print("\nTo close: Ctrl+C")

    if args.xlink_mic:
        qmic = device.getOutputQueue(name="mic", maxSize=16, blocking=True)
    if args.xlink_cam:
        qcam = device.getOutputQueue(name="cam", maxSize=4, blocking=False)

    while True:
        if args.xlink_mic or args.xlink_cam:
            if args.xlink_mic:
                pkt = qmic.tryGet()
                if pkt is not None:
                    print('MIC seq:', pkt.getSequenceNum(),
                              'timestamp:{:.6f}'.format(
                                  pkt.getTimestamp().total_seconds() - tstart),
                              'samples:', pkt.getHeight(),
                              'mics:', pkt.getWidth())
                    data = pkt.getData()
                    data.tofile(f)
            if args.xlink_cam:
                pkt = qcam.tryGet()
                if pkt is not None:
                    print('CAM seq:', pkt.getSequenceNum(),
                              'timestamp:{:.6f}'.format(
                                  pkt.getTimestamp().total_seconds() - tstart),
                              'height:', pkt.getHeight(),
                              'width:', pkt.getWidth())
                    frame = pkt.getCvFrame()
                    cv2.imshow('cam', frame)
                    if cv2.waitKey(1) == ord('q'):
                        break
            continue

        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            break
