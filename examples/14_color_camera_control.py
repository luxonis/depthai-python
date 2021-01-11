#!/usr/bin/env python3

# This example shows usage of Camera Control message as well as ColorCamera configInput to change crop x and y
# Uses 'WASD' controls to move the crop window and 'C' to capture a still image

import depthai as dai
import cv2
import numpy as np


# Step size ('W','A','S','D' controls)
STEP_SIZE = 8


pipeline = dai.Pipeline()

# Nodes
colorCam = pipeline.createColorCamera()
controlIn = pipeline.createXLinkIn()
configIn = pipeline.createXLinkIn()
videoEncoder = pipeline.createVideoEncoder()
stillEncoder = pipeline.createVideoEncoder()
videoMjpegOut = pipeline.createXLinkOut()
stillMjpegOut = pipeline.createXLinkOut()
previewOut = pipeline.createXLinkOut()


# Properties
colorCam.setVideoSize(640, 360)
colorCam.setPreviewSize(300, 300)
controlIn.setStreamName('control')
configIn.setStreamName('config')
videoEncoder.setDefaultProfilePreset(colorCam.getVideoSize(), colorCam.getFps(), dai.VideoEncoderProperties.Profile.MJPEG)
stillEncoder.setDefaultProfilePreset(colorCam.getStillSize(), 1, dai.VideoEncoderProperties.Profile.MJPEG)
videoMjpegOut.setStreamName('video')
stillMjpegOut.setStreamName('still')
previewOut.setStreamName('preview')


# Link nodes
colorCam.video.link(videoEncoder.input)
colorCam.still.link(stillEncoder.input)
colorCam.preview.link(previewOut.input)
controlIn.out.link(colorCam.inputControl)
configIn.out.link(colorCam.inputConfig)
videoEncoder.bitstream.link(videoMjpegOut.input)
stillEncoder.bitstream.link(stillMjpegOut.input)


# Connect to device
dev = dai.Device(pipeline)

# Create data queues
controlQueue = dev.getInputQueue('control')
configQueue = dev.getInputQueue('config')
previewQueue = dev.getOutputQueue('preview')
videoQueue = dev.getOutputQueue('video')
stillQueue = dev.getOutputQueue('still')

# Start pipeline
dev.startPipeline()

# Max crop_x & crop_y
max_crop_x = (colorCam.getResolutionWidth() - colorCam.getVideoWidth()) / colorCam.getResolutionWidth()
max_crop_y = (colorCam.getResolutionHeight() - colorCam.getVideoHeight()) / colorCam.getResolutionHeight()

# Default crop
crop_x = 0
crop_y = 0

while True:

    previewFrames = previewQueue.tryGetAll()
    for previewFrame in previewFrames:
        cv2.imshow('preview', previewFrame.getData().reshape(previewFrame.getWidth(), previewFrame.getHeight(), 3))

    videoFrames = videoQueue.tryGetAll()
    for videoFrame in videoFrames:
        # Decode JPEG
        frame = cv2.imdecode(videoFrame.getData(), cv2.IMREAD_UNCHANGED)
        # Display
        cv2.imshow('video', frame)

        # Send new cfg to camera
        cfg = dai.ImageManipConfig()
        cfg.setCropRect(crop_x, crop_y, 0, 0)
        configQueue.send(cfg)
        print('Sending new crop - x: ', crop_x, ' y: ', crop_y)

    stillFrames = stillQueue.tryGetAll()
    for stillFrame in stillFrames:
        # Decode JPEG
        frame = cv2.imdecode(stillFrame.getData(), cv2.IMREAD_UNCHANGED)
        # Display
        cv2.imshow('still', frame)


    # Update screen
    key = cv2.waitKey(1)
    if key == ord('q'):
        break
    elif key == ord('c'):
        ctrl = dai.CameraControl()
        ctrl.setCaptureStill(True)
        controlQueue.send(ctrl)
    elif key == ord('a'):
        crop_x = crop_x - (max_crop_x / colorCam.getResolutionWidth()) * STEP_SIZE
        if crop_x < 0: crop_x = max_crop_x
    elif key == ord('d'):
        crop_x = crop_x + (max_crop_x / colorCam.getResolutionWidth()) * STEP_SIZE
        if crop_x > max_crop_x: crop_x = 0
    elif key == ord('w'):
        crop_y = crop_y - (max_crop_y / colorCam.getResolutionHeight()) * STEP_SIZE
        if crop_y < 0: crop_y = max_crop_y
    elif key == ord('s'):
        crop_y = crop_y + (max_crop_y / colorCam.getResolutionHeight()) * STEP_SIZE
        if crop_y > max_crop_y: crop_y = 0