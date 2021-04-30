#!/usr/bin/env python3

import os
#os.environ["DEPTHAI_LEVEL"] = "debug"

import cv2
import argparse
import depthai as dai

parser = argparse.ArgumentParser()
parser.add_argument('-res', '--resolution', type=int, default=400, choices={400, 720, 800},
                    help="Select camera resolution (height). Default: %(default)s")
parser.add_argument('-cam', '--camera-socket', default='left', choices={'left', 'right', 'rgb'},
                    help="Select camera socket")
parser.add_argument('-color', '--color-cam', default=False, action="store_true",
                    help="Create ColorCamera instead of MonoCamera")
args = parser.parse_args()

if args.color_cam and args.resolution == 400:
    print('OV9782 ColorCamera 400p resolution not supported. Defaulting to 800p')
    args.resolution = 800

print("DepthAI version:", dai.__version__)
print("DepthAI path:", dai.__file__)

cam_socket_opts = {
    'rgb'  : dai.CameraBoardSocket.RGB,
    'left' : dai.CameraBoardSocket.LEFT,
    'right': dai.CameraBoardSocket.RIGHT,
}

mono_res_opts = {
    400: dai.MonoCameraProperties.SensorResolution.THE_400_P,
    720: dai.MonoCameraProperties.SensorResolution.THE_720_P,
    800: dai.MonoCameraProperties.SensorResolution.THE_800_P,
}

color_res_opts = {
    720: dai.ColorCameraProperties.SensorResolution.THE_720_P,
    800: dai.ColorCameraProperties.SensorResolution.THE_800_P,
}

# Start defining a pipeline
pipeline = dai.Pipeline()

if args.color_cam:
    cam = pipeline.createColorCamera()
    cam.setResolution(color_res_opts[args.resolution])
else:
    cam = pipeline.createMonoCamera()
    cam.setResolution(mono_res_opts[args.resolution])

cam.setBoardSocket(cam_socket_opts[args.camera_socket])

xout = pipeline.createXLinkOut()
xout.setStreamName(args.camera_socket)
if args.color_cam:
    cam.isp.link(xout.input)
else:
    cam.out.link(xout.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    q = device.getOutputQueue(name=args.camera_socket, maxSize=4, blocking=False)

    while True:
        frame = q.get().getCvFrame()
        cv2.imshow(args.camera_socket, frame)

        if cv2.waitKey(1) == ord('q'):
            break
