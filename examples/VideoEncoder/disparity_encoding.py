#!/usr/bin/env python3

import depthai as dai

# NOTE Because we are encoding disparity values, output video will be a gray, and won't have many pixel levels - either 0..95 or 0..190

# Create pipeline
pipeline = dai.Pipeline()

# Create left/right mono cameras for Stereo depth
monoLeft = pipeline.create(dai.node.MonoCamera)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)

monoRight = pipeline.create(dai.node.MonoCamera)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create a node that will produce the depth map
depth = pipeline.create(dai.node.StereoDepth)
depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
depth.setLeftRightCheck(False)
depth.setExtendedDisparity(False)
# Subpixel disparity is of UINT16 format, which is unsupported by VideoEncoder
depth.setSubpixel(False)
monoLeft.out.link(depth.left)
monoRight.out.link(depth.right)

videoEnc = pipeline.create(dai.node.VideoEncoder)
# Depth resolution/FPS will be the same as mono resolution/FPS
videoEnc.setDefaultProfilePreset(monoLeft.getFps(), dai.VideoEncoderProperties.Profile.MJPEG)
depth.disparity.link(videoEnc.input)

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("enc")
videoEnc.bitstream.link(xout.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the encoded data from the output defined above
    q = device.getOutputQueue(name="enc")

    # The .h265 file is a raw stream file (not playable yet)
    with open('disparity.mjpeg', 'wb') as videoFile:
        print("Press Ctrl+C to stop encoding...")
        try:
            while True:
                videoFile.write(q.get().getData())
        except KeyboardInterrupt:
            # Keyboard interrupt (Ctrl + C) detected
            pass

    print("To view the encoded data, convert the stream file (.mjpeg) into a video file (.mp4) using a command below:")
    print("ffmpeg -framerate 30 -i disparity.mjpeg -c copy video.mp4")
