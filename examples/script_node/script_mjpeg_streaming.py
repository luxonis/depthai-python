#!/usr/bin/env python3

import cv2
import depthai as dai
import time
import sys

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam = pipeline.create(dai.node.ColorCamera)
# VideoEncoder
jpeg = pipeline.create(dai.node.VideoEncoder)
jpeg.setDefaultProfilePreset(cam.getVideoSize(), cam.getFps(), dai.VideoEncoderProperties.Profile.MJPEG)

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
    import time
    from socketserver import ThreadingMixIn
    from http.server import BaseHTTPRequestHandler, HTTPServer

    PORT = 8080

    class ThreadingSimpleServer(ThreadingMixIn, HTTPServer):
        pass

    class HTTPHandler(BaseHTTPRequestHandler):
        def do_GET(self):
            if self.path == '/':
                try:
                    self.send_response(200)
                    self.send_header('Content-type', 'multipart/x-mixed-replace; boundary=--jpgboundary')
                    self.end_headers()
                    fpsCounter = 0
                    timeCounter = time.time()
                    while True:
                        jpegImage = node.io['jpeg'].get()
                        self.wfile.write("--jpgboundary".encode())
                        self.wfile.write(bytes([13, 10]))
                        self.send_header('Content-type', 'image/jpeg')
                        self.send_header('Content-length', str(len(jpegImage.getData())))
                        self.end_headers()
                        self.wfile.write(jpegImage.getData())
                        self.end_headers()

                        fpsCounter = fpsCounter + 1
                        if time.time() - timeCounter > 1:
                            node.warn(f'FPS: {fpsCounter}')
                            fpsCounter = 0
                            timeCounter = time.time()
                except Exception as ex:
                    node.warn(str(ex))
            else:
                self.send_response(200)
                self.end_headers()
                self.wfile.write(b'<h1>[DepthAI] Not found</h1><p>Click <a href="/">here</a> to get to MJPEG streaming</p>')

    with ThreadingSimpleServer(("", PORT), HTTPHandler) as httpd:
        node.warn(f"Serving at port {PORT}")
        httpd.serve_forever()
""")

# Connections
cam.video.link(jpeg.input)
jpeg.bitstream.link(script.inputs['jpeg'])


# Flash conditionally
if len(sys.argv) > 1:
    (found, info) = dai.DeviceBootloader.getFirstAvailableDevice()
    if not found:
        print("No device found to flash. Exiting.")
        exit(-1)

    # Open DeviceBootloader and allow flashing bootloader
    with dai.DeviceBootloader(info) as bl:
        # Create a progress callback lambda
        progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
        compress = False
        if len(sys.argv) > 2:
            compress = True
        print(f'About to flash application (compression: {compress})')
        (success, errorMsg) = bl.flash(progress, pipeline, compress)
        if success:
            print('Successfully flashed the application')
        else:
            print(f'Failed to flash the application: {errorMsg}')

else:

    # Connect to device with pipeline
    with dai.Device(pipeline) as device:
        while not device.isClosed():
            time.sleep(1)