#!/usr/bin/env python3

import cv2
import depthai as dai
import sys

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutRgb = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("rgb")

# Properties
camRgb.setPreviewSize(300, 300)
camRgb.setInterleaved(False)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.RGB)

# Linking
camRgb.preview.link(xoutRgb.input)

compress = False
if len(sys.argv) > 1:

    if(sys.argv[1] == 'flash'):

        flash_mode = True
        # Search for first available PoE device
        devices = dai.Device.getAllAvailableDevices()
        device_info = None
        for dev_info in devices:
            device_info = dev_info
            break

        # Connect to a PoE device with pipeline
        if device_info is not None:
            # Ask if for confirmation if flash mode
            print(f"Are you sure you want to flash device '{device_info.getMxId()}' (compress: {compress})")
            print(f"Type 'y' and press enter to proceed, otherwise exits: ")
            if input() != 'y':
                print("Prompt declined, exiting...")
                exit(-1)
            with dai.DeviceBootloader(device_info) as bl:
                # Create a progress callback lambda
                progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
                (success, msg) = bl.flash(progress, pipeline, compress)
                print(f'Success: {success}, message: {msg}')
        else:
            print('No devices found')

    elif sys.argv[1] == 'connect':

        # Connect to device and start pipeline
        cfg = dai.Device.Config()
        cfg.connectToFlashBootedDevice = True
        with dai.Device(cfg) as device:
            print('Connected cameras: ', device.getConnectedCameras())
            # Print out usb speed
            print('Usb speed: ', device.getUsbSpeed().name)
            device.startPipeline(pipeline)

            # Output queue will be used to get the rgb frames from the output defined above
            qRgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)

            while True:
                inRgb = qRgb.get()  # blocking call, will wait until a new data has arrived

                # Retrieve 'bgr' (opencv format) frame
                cv2.imshow("rgb", inRgb.getCvFrame())

                if cv2.waitKey(1) == ord('q'):
                    break

    else:
        print('Unknown action...')

else:

    # Connect to device and start pipeline
    with dai.Device(pipeline) as device:

        print('Connected cameras: ', device.getConnectedCameras())
        # Print out usb speed
        print('Usb speed: ', device.getUsbSpeed().name)

        # Output queue will be used to get the rgb frames from the output defined above
        qRgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)

        while True:
            inRgb = qRgb.get()  # blocking call, will wait until a new data has arrived

            # Retrieve 'bgr' (opencv format) frame
            cv2.imshow("rgb", inRgb.getCvFrame())

            if cv2.waitKey(1) == ord('q'):
                break