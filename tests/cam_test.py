#!/usr/bin/env python3

import os
#os.environ["DEPTHAI_LEVEL"] = "debug"

import cv2
import argparse
import depthai as dai

parser = argparse.ArgumentParser()
parser.add_argument('-mres', '--mono-resolution', type=int, default=800, choices={400, 720, 800},
                    help="Select mono camera resolution (height). Default: %(default)s")
parser.add_argument('-cres', '--color-resolution', default='800', choices={'720', '800', '1080', '4k', '12mp'},
                    help="Select color camera resolution / height. Default: %(default)s")
parser.add_argument('-rot', '--rotate', const='all', choices={'all', 'rgb', 'mono'}, nargs="?",
                    help="Which cameras to rotate 180 degrees. All if not filtered")
args = parser.parse_args()

# TODO as args
cam_list = ['rgb', 'left', 'right']

print("DepthAI version:", dai.__version__)
print("DepthAI path:", dai.__file__)

cam_socket_opts = {
    'rgb'  : dai.CameraBoardSocket.RGB,
    'left' : dai.CameraBoardSocket.LEFT,
    'right': dai.CameraBoardSocket.RIGHT,
}

rotate = {
    'rgb'  : args.rotate in ['all', 'rgb'],
    'left' : args.rotate in ['all', 'mono'],
    'right': args.rotate in ['all', 'mono'],
}

mono_res_opts = {
    400: dai.MonoCameraProperties.SensorResolution.THE_400_P,
    720: dai.MonoCameraProperties.SensorResolution.THE_720_P,
    800: dai.MonoCameraProperties.SensorResolution.THE_800_P,
}

color_res_opts = {
    '720':  dai.ColorCameraProperties.SensorResolution.THE_720_P,
    '800':  dai.ColorCameraProperties.SensorResolution.THE_800_P,
    '1080': dai.ColorCameraProperties.SensorResolution.THE_1080_P,
    '4k':   dai.ColorCameraProperties.SensorResolution.THE_4_K,
    '12mp': dai.ColorCameraProperties.SensorResolution.THE_12_MP,
}

# Start defining a pipeline
pipeline = dai.Pipeline()

cam = {}
xout = {}
for c in cam_list:
    xout[c] = pipeline.createXLinkOut()
    xout[c].setStreamName(c)
    if c == 'rgb':
        cam[c] = pipeline.createColorCamera()
        cam[c].setResolution(color_res_opts[args.color_resolution])
        cam[c].isp.link(xout[c].input)
    else:
        cam[c] = pipeline.createMonoCamera()
        cam[c].setResolution(mono_res_opts[args.mono_resolution])
        cam[c].out.link(xout[c].input)
    cam[c].setBoardSocket(cam_socket_opts[c])
    if rotate[c]:
        cam[c].setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)

# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    q = {}
    for c in cam_list:
        q[c] = device.getOutputQueue(name=c, maxSize=4, blocking=False)

    while True:
        for c in cam_list:
            pkt = q[c].tryGet()
            if pkt is not None:
                frame = pkt.getCvFrame()
                cv2.imshow(c, frame)
        if cv2.waitKey(1) == ord('q'):
            break
