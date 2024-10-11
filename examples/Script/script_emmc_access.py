#!/usr/bin/env python3

import depthai as dai
import cv2

# Start defining a pipeline
pipeline = dai.Pipeline()

board = dai.BoardConfig()
board.emmc = True
pipeline.setBoardConfig(board)

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
jpegEncoder = pipeline.create(dai.node.VideoEncoder)

# Properties
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)
jpegEncoder.setDefaultProfilePreset(1, dai.VideoEncoderProperties.Profile.MJPEG)

#Set a write script
script_write = pipeline.createScript()
script_write.setProcessor(dai.ProcessorType.LEON_CSS)
script_write.setScript("""

    import os
    index = 1000
    import time
    while True:
        # Find an unused file name first
        while True:
            path = '/media/mmcsd-0-0/' + str(index) + '.jpg'
            if not os.path.exists(path):
                break
            index += 1
        frame = node.io['jpeg'].get()
        node.warn(f'Saving to EMMC: {path}')
        with open(path, 'wb') as f:
            f.write(frame.getData())
        index += 1
        time.sleep(3)

""")
                      
#Set a read script
script_read = pipeline.createScript()
script_read.setProcessor(dai.ProcessorType.LEON_CSS)
script_read.setScript("""

    import http.server
    import socketserver
    import socket
    import fcntl
    import struct
    import os

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

camRgb.video.link(jpegEncoder.input)
jpegEncoder.bitstream.link(script_write.inputs['jpeg'])
script_write.inputs['jpeg'].setBlocking(False)
xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("rgb")
script_read.outputs['jpeg'].link(xout.input)


# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Output queue will be used to get the rgb frames from the output defined above
    qRgb = device.getOutputQueue(name="rgb", maxSize=100, blocking=False)

    while True:
        inRgb = qRgb.tryGet() 
        
        if inRgb is not None:
            cv2.imshow("rgb", inRgb.getCvFrame())
            
        if cv2.waitKey(1) == ord('q'):
            break