#!/usr/bin/env python3

"""
This example shows usage of Camera Control message as well as ColorCamera configInput to change crop x and y
Uses 'WASD' controls to move the crop window, 'C' to capture a still image, 'T' to trigger autofocus, 'IOKL,.'
for manual exposure/focus:
  Control:      key[dec/inc]  min..max
  exposure time:     I   O      1..33000 [us]
  sensitivity iso:   K   L    100..1600
  focus:             ,   .      0..255 [far..near]
To go back to auto controls:
  'E' - autoexposure
  'F' - autofocus (continuous)

Other controls:
'1' - AWB lock (true / false)
'2' - AE lock (true / false)
'3' - Select control: AWB mode
'4' - Select control: AE compensation
'5' - Select control: anti-banding/flicker mode
'6' - Select control: effect mode
'7' - Select control: brightness
'8' - Select control: contrast
'9' - Select control: saturation
'0' - Select control: sharpness
'[' - Select control: luma denoise
']' - Select control: chroma denoise

For the 'Select control: ...' options, use these keys to modify the value:
  '-' or '_' to decrease
  '+' or '=' to increase
"""

import depthai as dai
import cv2

# Step size ('W','A','S','D' controls)
STEP_SIZE = 8
# Manual exposure/focus set step
EXP_STEP = 500  # us
ISO_STEP = 50
LENS_STEP = 3

# On some host systems it's faster to display uncompressed video
videoMjpeg = False

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
colorCam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_800_P)
colorCam.setStillSize(1280, 800)  # TODO: default in FW
#colorCam.setVideoSize(640, 360)
colorCam.setPreviewSize(300, 300)
controlIn.setStreamName('control')
configIn.setStreamName('config')
videoEncoder.setDefaultProfilePreset(colorCam.getVideoSize(), colorCam.getFps(), dai.VideoEncoderProperties.Profile.MJPEG)
stillEncoder.setDefaultProfilePreset(colorCam.getStillSize(), 1, dai.VideoEncoderProperties.Profile.MJPEG)
videoMjpegOut.setStreamName('video')
stillMjpegOut.setStreamName('still')
previewOut.setStreamName('preview')


# Link nodes
colorCam.still.link(stillEncoder.input)
colorCam.preview.link(previewOut.input)
controlIn.out.link(colorCam.inputControl)
configIn.out.link(colorCam.inputConfig)
stillEncoder.bitstream.link(stillMjpegOut.input)
if videoMjpeg:
    colorCam.video.link(videoEncoder.input)
    videoEncoder.bitstream.link(videoMjpegOut.input)
else:
    colorCam.video.link(videoMjpegOut.input)


def clamp(num, v0, v1):
    return max(v0, min(num, v1))


# Connect and start the pipeline
with dai.Device(pipeline) as dev:

    # Get data queues
    controlQueue = dev.getInputQueue('control')
    configQueue = dev.getInputQueue('config')
    previewQueue = dev.getOutputQueue('preview')
    videoQueue = dev.getOutputQueue('video')
    stillQueue = dev.getOutputQueue('still')

    # Max cropX & cropY
    maxCropX = (colorCam.getResolutionWidth() - colorCam.getVideoWidth()) / colorCam.getResolutionWidth()
    maxCropY = (colorCam.getResolutionHeight() - colorCam.getVideoHeight()) / colorCam.getResolutionHeight()

    # Default crop
    cropX = 0
    cropY = 0
    sendCamConfig = True

    # Defaults and limits for manual focus/exposure controls
    lensPos = 150
    lensMin = 0
    lensMax = 255

    expTime = 20000
    expMin = 1
    expMax = 33000

    sensIso = 800
    sensMin = 100
    sensMax = 1600

    # TODO make auto-iterable
    awb_mode_idx = -1
    awb_mode_list = [
        dai.CameraControl.AutoWhiteBalanceMode.OFF,
        dai.CameraControl.AutoWhiteBalanceMode.AUTO,
        dai.CameraControl.AutoWhiteBalanceMode.INCANDESCENT,
        dai.CameraControl.AutoWhiteBalanceMode.FLUORESCENT,
        dai.CameraControl.AutoWhiteBalanceMode.WARM_FLUORESCENT,
        dai.CameraControl.AutoWhiteBalanceMode.DAYLIGHT,
        dai.CameraControl.AutoWhiteBalanceMode.CLOUDY_DAYLIGHT,
        dai.CameraControl.AutoWhiteBalanceMode.TWILIGHT,
        dai.CameraControl.AutoWhiteBalanceMode.SHADE,
    ]

    anti_banding_mode_idx = -1
    anti_banding_mode_list = [
        dai.CameraControl.AntiBandingMode.OFF,
        dai.CameraControl.AntiBandingMode.MAINS_50_HZ,
        dai.CameraControl.AntiBandingMode.MAINS_60_HZ,
        dai.CameraControl.AntiBandingMode.AUTO,
    ]

    effect_mode_idx = -1
    effect_mode_list = [
        dai.CameraControl.EffectMode.OFF,
        dai.CameraControl.EffectMode.MONO,
        dai.CameraControl.EffectMode.NEGATIVE,
        dai.CameraControl.EffectMode.SOLARIZE,
        dai.CameraControl.EffectMode.SEPIA,
        dai.CameraControl.EffectMode.POSTERIZE,
        dai.CameraControl.EffectMode.WHITEBOARD,
        dai.CameraControl.EffectMode.BLACKBOARD,
        dai.CameraControl.EffectMode.AQUA,
    ]

    ae_comp = 0
    ae_lock = False
    awb_lock = False
    saturation = 0
    contrast = 0
    brightness = 0
    sharpness = 0
    luma_denoise = 0
    chroma_denoise = 0
    control = 'none'

    while True:

        previewFrames = previewQueue.tryGetAll()
        for previewFrame in previewFrames:
            cv2.imshow('preview', previewFrame.getData().reshape(previewFrame.getWidth(), previewFrame.getHeight(), 3))

        videoFrames = videoQueue.tryGetAll()
        for videoFrame in videoFrames:
            # Decode JPEG
            if videoMjpeg:
                frame = cv2.imdecode(videoFrame.getData(), cv2.IMREAD_UNCHANGED)
            else:
                frame = videoFrame.getCvFrame()
            # Display
            cv2.imshow('video', frame)

            # Send new cfg to camera
            if sendCamConfig:
                cfg = dai.ImageManipConfig()
                cfg.setCropRect(cropX, cropY, 0, 0)
                configQueue.send(cfg)
                print('Sending new crop - x: ', cropX, ' y: ', cropY)
                sendCamConfig = False

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
        elif key == ord('t'):
            print("Autofocus trigger (and disable continuous)")
            ctrl = dai.CameraControl()
            ctrl.setAutoFocusMode(dai.CameraControl.AutoFocusMode.AUTO)
            ctrl.setAutoFocusTrigger()
            controlQueue.send(ctrl)
        elif key == ord('f'):
            print("Autofocus enable, continuous")
            ctrl = dai.CameraControl()
            ctrl.setAutoFocusMode(dai.CameraControl.AutoFocusMode.CONTINUOUS_VIDEO)
            controlQueue.send(ctrl)
        elif key == ord('e'):
            print("Autoexposure enable")
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureEnable()
            controlQueue.send(ctrl)
        elif key in [ord(','), ord('.')]:
            if key == ord(','): lensPos -= LENS_STEP
            if key == ord('.'): lensPos += LENS_STEP
            lensPos = clamp(lensPos, lensMin, lensMax)
            print("Setting manual focus, lens position:", lensPos)
            ctrl = dai.CameraControl()
            ctrl.setManualFocus(lensPos)
            controlQueue.send(ctrl)
        elif key in [ord('i'), ord('o'), ord('k'), ord('l')]:
            if key == ord('i'): expTime -= EXP_STEP
            if key == ord('o'): expTime += EXP_STEP
            if key == ord('k'): sensIso -= ISO_STEP
            if key == ord('l'): sensIso += ISO_STEP
            expTime = clamp(expTime, expMin, expMax)
            sensIso = clamp(sensIso, sensMin, sensMax)
            print("Setting manual exposure, time:", expTime, "iso:", sensIso)
            ctrl = dai.CameraControl()
            ctrl.setManualExposure(expTime, sensIso)
            controlQueue.send(ctrl)
        elif key in [ord('w'), ord('a'), ord('s'), ord('d')]:
            if key == ord('a'):
                cropX = cropX - (maxCropX / colorCam.getResolutionWidth()) * STEP_SIZE
                if cropX < 0: cropX = maxCropX
            elif key == ord('d'):
                cropX = cropX + (maxCropX / colorCam.getResolutionWidth()) * STEP_SIZE
                if cropX > maxCropX: cropX = 0
            elif key == ord('w'):
                cropY = cropY - (maxCropY / colorCam.getResolutionHeight()) * STEP_SIZE
                if cropY < 0: cropY = maxCropY
            elif key == ord('s'):
                cropY = cropY + (maxCropY / colorCam.getResolutionHeight()) * STEP_SIZE
                if cropY > maxCropY: cropY = 0
            sendCamConfig = True
        elif key == ord('1'):
            awb_lock = not awb_lock
            print("Auto white balance lock:", awb_lock)
            ctrl = dai.CameraControl()
            ctrl.setAutoWhiteBalanceLock(awb_lock)
            controlQueue.send(ctrl)
        elif key == ord('2'):
            ae_lock = not ae_lock
            print("Auto exposure lock:", ae_lock)
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureLock(ae_lock)
            controlQueue.send(ctrl)
        elif key >= 0 and chr(key) in '34567890[]':
            if   key == ord('3'): control = 'awb_mode'
            elif key == ord('4'): control = 'ae_comp'
            elif key == ord('5'): control = 'anti_banding_mode'
            elif key == ord('6'): control = 'effect_mode'
            elif key == ord('7'): control = 'brightness'
            elif key == ord('8'): control = 'contrast'
            elif key == ord('9'): control = 'saturation'
            elif key == ord('0'): control = 'sharpness'
            elif key == ord('['): control = 'luma_denoise'
            elif key == ord(']'): control = 'chroma_denoise'
            print("Selected control:", control)
        elif key in [ord('-'), ord('_'), ord('+'), ord('=')]:
            change = 0
            if key in [ord('-'), ord('_')]: change = -1
            if key in [ord('+'), ord('=')]: change = 1
            ctrl = dai.CameraControl()
            if control == 'none':
                print("Please select a control first using keys 3..9 0 [ ]")
            elif control == 'ae_comp':
                ae_comp = clamp(ae_comp + change, -9, 9)
                print("Auto exposure compensation:", ae_comp)
                ctrl.setAutoExposureCompensation(ae_comp)
            elif control == 'anti_banding_mode':
                cnt = len(anti_banding_mode_list)
                anti_banding_mode_idx = (anti_banding_mode_idx + cnt + change) % cnt
                anti_banding_mode = anti_banding_mode_list[anti_banding_mode_idx]
                print("Anti-banding mode:", anti_banding_mode)
                ctrl.setAntiBandingMode(anti_banding_mode)
            elif control == 'awb_mode':
                cnt = len(awb_mode_list)
                awb_mode_idx = (awb_mode_idx + cnt + change) % cnt
                awb_mode = awb_mode_list[awb_mode_idx]
                print("Auto white balance mode:", awb_mode)
                ctrl.setAutoWhiteBalanceMode(awb_mode)
            elif control == 'effect_mode':
                cnt = len(effect_mode_list)
                effect_mode_idx = (effect_mode_idx + cnt + change) % cnt
                effect_mode = effect_mode_list[effect_mode_idx]
                print("Effect mode:", effect_mode)
                ctrl.setEffectMode(effect_mode)
            elif control == 'brightness':
                brightness = clamp(brightness + change, -10, 10)
                print("Brightness:", brightness)
                ctrl.setBrightness(brightness)
            elif control == 'contrast':
                contrast = clamp(contrast + change, -10, 10)
                print("Contrast:", contrast)
                ctrl.setContrast(contrast)
            elif control == 'saturation':
                saturation = clamp(saturation + change, -10, 10)
                print("Saturation:", saturation)
                ctrl.setSaturation(saturation)
            elif control == 'sharpness':
                sharpness = clamp(sharpness + change, 0, 4)
                print("Sharpness:", sharpness)
                ctrl.setSharpness(sharpness)
            elif control == 'luma_denoise':
                luma_denoise = clamp(luma_denoise + change, 0, 4)
                print("Luma denoise:", luma_denoise)
                ctrl.setLumaDenoise(luma_denoise)
            elif control == 'chroma_denoise':
                chroma_denoise = clamp(chroma_denoise + change, 0, 4)
                print("Chroma denoise:", chroma_denoise)
                ctrl.setChromaDenoise(chroma_denoise)
            controlQueue.send(ctrl)
