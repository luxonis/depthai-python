#!/usr/bin/env python3

import os
os.environ["DEPTHAI_LEVEL"] = "debug"

import cv2
import argparse
import depthai as dai

parser = argparse.ArgumentParser()
parser.add_argument('-res', '--resolution', type=int, default=400, choices={400, 720, 800},
                    help="Select camera resolution (height). Default: %(default)s")
parser.add_argument('-cam', '--camera-socket', default='left', choices={'left', 'right', 'rgb'},
                    help="Select camera socket")
args = parser.parse_args()

print("DepthAI version:", dai.__version__)
print("DepthAI path:", dai.__file__)

cam_socket_opts = {
    'rgb'  : dai.CameraBoardSocket.RGB,
    'left' : dai.CameraBoardSocket.LEFT,
    'right': dai.CameraBoardSocket.RIGHT,
}

cam_res_opts = {
    400: dai.MonoCameraProperties.SensorResolution.THE_400_P,
    720: dai.MonoCameraProperties.SensorResolution.THE_720_P,
    800: dai.MonoCameraProperties.SensorResolution.THE_800_P,
}

# Start defining a pipeline
pipeline = dai.Pipeline()

cam = pipeline.createMonoCamera()
cam.setBoardSocket(cam_socket_opts[args.camera_socket])
cam.setResolution(cam_res_opts[args.resolution])

xout = pipeline.createXLinkOut()
xout.setStreamName(args.camera_socket)
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
