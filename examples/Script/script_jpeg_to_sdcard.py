#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Create pipeline
pipeline = dai.Pipeline()

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
xoutRgb = pipeline.create(dai.node.XLinkOut)
xinCtrl = pipeline.create(dai.node.XLinkIn)
jpegEncoder = pipeline.create(dai.node.VideoEncoder)
scriptSave = pipeline.create(dai.node.Script)
scriptServer = pipeline.create(dai.node.Script)

# Properties
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_12_MP)
camRgb.setPreviewSize(640, 360)
jpegEncoder.setDefaultProfilePreset(1, dai.VideoEncoderProperties.Profile.MJPEG)
xoutRgb.setStreamName("rgb")
xinCtrl.setStreamName('control')

# Note: all scripts accessing the media storage must run on LEON_CSS
scriptSave.setProcessor(dai.ProcessorType.LEON_CSS)
scriptSave.setScript("""
import os

index = 1000

while True:
    # Find an unused file name first
    while True:
        path = '/media/mmcsd-0-0/' + str(index) + '.jpg'
        if not os.path.exists(path):
            break
        index += 1
    frame = node.io['jpeg'].get()
    node.warn(f'Saving to SDcard: {path}')
    with open(path, 'wb') as f:
        f.write(frame.getData())
    index += 1
""")

scriptServer.setProcessor(dai.ProcessorType.LEON_CSS)
scriptServer.setScript("""
import os
# To avoid a crash on non-PoE devices, stop here
if not os.path.exists('/etc/resolve.conf'):
    import time
    while True: time.sleep(1)

import http.server
import socketserver
import socket
import fcntl
import struct

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        -1071617759,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15].encode())
    )[20:24])

# Note: `chdir` here will prevent unmount, this should be improved!
os.chdir('/media/mmcsd-0-0')

PORT = 80
Handler = http.server.SimpleHTTPRequestHandler

with socketserver.TCPServer(("", PORT), Handler) as httpd:
    ip = get_ip_address('re0')
    node.warn(f'===== HTTP file server accessible at: http://{ip}')
    httpd.serve_forever()
""")

# Linking
camRgb.preview.link(xoutRgb.input)
camRgb.still.link(jpegEncoder.input)
jpegEncoder.bitstream.link(scriptSave.inputs['jpeg'])
xinCtrl.out.link(camRgb.inputControl)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Get queues for RGB preview and capture control
    qRgb = device.getOutputQueue(name='rgb', maxSize=4, blocking=False)
    qControl = device.getInputQueue('control')

    print('======== Press C to capture JPEG images to SDcard')

    while True:
        inRgb = qRgb.get()
        cv2.imshow('rgb', inRgb.getCvFrame())

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('c'):
            ctrl = dai.CameraControl()
            ctrl.setCaptureStill(True)
            qControl.send(ctrl)
