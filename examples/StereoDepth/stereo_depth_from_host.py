#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
from time import sleep
import datetime
import argparse
from pathlib import Path
import math

datasetDefault = str((Path(__file__).parent / Path('../models/dataset')).resolve().absolute())
parser = argparse.ArgumentParser()
parser.add_argument('-dataset', nargs='?', help="Path to recorded frames", default=datasetDefault)
parser.add_argument('-debug', "--debug", action="store_true", help="Enable debug outputs.")
parser.add_argument('-dumpdispcost', "--dumpdisparitycostvalues", action="store_true", help="Dumps the disparity cost values for each disparity range. 96 byte for each pixel.")
args = parser.parse_args()

if args.debug and args.dumpdisparitycostvalues:
    print("-debug and --dumpdisparitycostvalues are mutually exclusive!")
    exit(1)

if not Path(datasetDefault).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')


class StereoConfigHandler:

    class Trackbar:
        def __init__(self, trackbarName, windowName, minValue, maxValue, defaultValue, handler):
            self.min = minValue
            self.max = maxValue
            self.windowName = windowName
            self.trackbarName = trackbarName
            cv2.createTrackbar(trackbarName, windowName, minValue, maxValue, handler)
            cv2.setTrackbarPos(trackbarName, windowName, defaultValue)

        def set(self, value):
            if value < self.min:
                value = self.min
                print(f'{self.trackbarName} min value is {self.min}')
            if value > self.max:
                value = self.max
                print(f'{self.trackbarName} max value is {self.max}')
            cv2.setTrackbarPos(self.trackbarName, self.windowName, value)

    newConfig = False
    config = None
    trSigma = list()
    trConfidence = list()
    trLrCheck = list()
    trFractionalBits = list()
    trLineqAlpha = list()
    trLineqBeta = list()
    trLineqThreshold = list()

    def trackbarSigma(value):
        StereoConfigHandler.config.postProcessing.bilateralSigmaValue = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trSigma:
            tr.set(value)

    def trackbarConfidence(value):
        StereoConfigHandler.config.costMatching.confidenceThreshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trConfidence:
            tr.set(value)

    def trackbarLrCheckThreshold(value):
        StereoConfigHandler.config.algorithmControl.leftRightCheckThreshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLrCheck:
            tr.set(value)

    def trackbarFractionalBits(value):
        StereoConfigHandler.config.algorithmControl.subpixelFractionalBits = value
        for tr in StereoConfigHandler.trFractionalBits:
            tr.set(value)

    def trackbarLineqAlpha(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.alpha = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqAlpha:
            tr.set(value)

    def trackbarLineqBeta(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.beta = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqBeta:
            tr.set(value)

    def trackbarLineqThreshold(value):
        StereoConfigHandler.config.costMatching.linearEquationParameters.threshold = value
        StereoConfigHandler.newConfig = True
        for tr in StereoConfigHandler.trLineqThreshold:
            tr.set(value)

    def handleKeypress(key, stereoDepthConfigInQueue):
        if key == ord('m'):
            StereoConfigHandler.newConfig = True
            medianSettings = [dai.MedianFilter.MEDIAN_OFF, dai.MedianFilter.KERNEL_3x3, dai.MedianFilter.KERNEL_5x5, dai.MedianFilter.KERNEL_7x7]
            currentMedian = StereoConfigHandler.config.postProcessing.median
            nextMedian = medianSettings[(medianSettings.index(currentMedian)+1) % len(medianSettings)]
            print(f"Changing median to {nextMedian.name} from {currentMedian.name}")
            StereoConfigHandler.config.postProcessing.median = nextMedian
        elif key == ord('c'):
            StereoConfigHandler.newConfig = True
            censusSettings = [dai.RawStereoDepthConfig.CensusTransform.KernelSize.AUTO, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_5x5, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_7x7, dai.RawStereoDepthConfig.CensusTransform.KernelSize.KERNEL_7x9]
            currentCensus = StereoConfigHandler.config.censusTransform.kernelSize
            nextCensus = censusSettings[(censusSettings.index(currentCensus)+1) % len(censusSettings)]
            print(f"Changing census transform to {nextCensus.name} from {currentCensus.name}")
            StereoConfigHandler.config.censusTransform.kernelSize = nextCensus
        elif key == ord('d'):
            StereoConfigHandler.newConfig = True
            dispRangeSettings = [dai.RawStereoDepthConfig.CostMatching.DisparityWidth.DISPARITY_64, dai.RawStereoDepthConfig.CostMatching.DisparityWidth.DISPARITY_96]
            currentDispRange = StereoConfigHandler.config.costMatching.disparityWidth
            nextDispRange = dispRangeSettings[(dispRangeSettings.index(currentDispRange)+1) % len(dispRangeSettings)]
            print(f"Changing disparity range to {nextDispRange.name} from {currentDispRange.name}")
            StereoConfigHandler.config.costMatching.disparityWidth = nextDispRange
        elif key == ord('f'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.costMatching.enableCompanding = not StereoConfigHandler.config.costMatching.enableCompanding
            state = "on" if StereoConfigHandler.config.costMatching.enableCompanding else "off"
            print(f"Companding {state}")
        elif key == ord('v'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.censusTransform.enableMeanMode = not StereoConfigHandler.config.censusTransform.enableMeanMode
            state = "on" if StereoConfigHandler.config.censusTransform.enableMeanMode else "off"
            print(f"Census transform mean mode {state}")
        elif key == ord('1'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.algorithmControl.enableLeftRightCheck = not StereoConfigHandler.config.algorithmControl.enableLeftRightCheck
            state = "on" if StereoConfigHandler.config.algorithmControl.enableLeftRightCheck else "off"
            print(f"LR-check {state}")
        elif key == ord('2'):
            StereoConfigHandler.newConfig = True
            StereoConfigHandler.config.algorithmControl.enableSubpixel = not StereoConfigHandler.config.algorithmControl.enableSubpixel
            state = "on" if StereoConfigHandler.config.algorithmControl.enableSubpixel else "off"
            print(f"Subpixel {state}")

        StereoConfigHandler.sendConfig(stereoDepthConfigInQueue)

    def sendConfig(stereoDepthConfigInQueue):
        if StereoConfigHandler.newConfig:
            StereoConfigHandler.newConfig = False
            configMessage = dai.StereoDepthConfig()
            configMessage.set(StereoConfigHandler.config)
            stereoDepthConfigInQueue.send(configMessage)

    def registerWindow(stream):
        cv2.namedWindow(stream)
        StereoConfigHandler.trConfidence.append(StereoConfigHandler.Trackbar('Disparity confidence', stream, 0, 255, StereoConfigHandler.config.costMatching.confidenceThreshold, StereoConfigHandler.trackbarConfidence))
        StereoConfigHandler.trSigma.append(StereoConfigHandler.Trackbar('Bilateral sigma', stream, 0, 100, StereoConfigHandler.config.postProcessing.bilateralSigmaValue, StereoConfigHandler.trackbarSigma))
        StereoConfigHandler.trLrCheck.append(StereoConfigHandler.Trackbar('LR-check threshold', stream, 0, 16, StereoConfigHandler.config.algorithmControl.leftRightCheckThreshold, StereoConfigHandler.trackbarLrCheckThreshold))
        StereoConfigHandler.trFractionalBits.append(StereoConfigHandler.Trackbar('Subpixel fractional bits', stream, 3, 5, StereoConfigHandler.config.algorithmControl.subpixelFractionalBits, StereoConfigHandler.trackbarFractionalBits))
        StereoConfigHandler.trLineqAlpha.append(StereoConfigHandler.Trackbar('Linear equation alpha', stream, 0, 15, StereoConfigHandler.config.costMatching.linearEquationParameters.alpha, StereoConfigHandler.trackbarLineqAlpha))
        StereoConfigHandler.trLineqBeta.append(StereoConfigHandler.Trackbar('Linear equation beta', stream, 0, 15, StereoConfigHandler.config.costMatching.linearEquationParameters.beta, StereoConfigHandler.trackbarLineqBeta))
        StereoConfigHandler.trLineqThreshold.append(StereoConfigHandler.Trackbar('Linear equation threshold', stream, 0, 255, StereoConfigHandler.config.costMatching.linearEquationParameters.threshold, StereoConfigHandler.trackbarLineqThreshold))

    def __init__(self, config):
        print("Control median filter using the 'm' key.")
        print("Control census transform kernel size using the 'c' key.")
        print("Control disparity search range using the 'd' key.")
        print("Control disparity companding using the 'f' key.")
        print("Control census transform mean mode using the 'v' key.")
        print("Control left-right check mode using the '1' key.")
        print("Control subpixel mode using the '2' key.")

        StereoConfigHandler.config = config


# StereoDepth initial config options.
outDepth = True  # Disparity by default
outConfidenceMap = True  # Output disparity confidence map
outRectified = True   # Output and display rectified streams
lrcheck = True   # Better handling for occlusions
extended = False  # Closer-in minimum depth, disparity range is doubled. Unsupported for now.
subpixel = True   # Better accuracy for longer distance, fractional disparity 32-levels

width = 1280
height = 800

xoutStereoCfg = None

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
stereo = pipeline.create(dai.node.StereoDepth)

monoLeft = pipeline.create(dai.node.XLinkIn)
monoRight = pipeline.create(dai.node.XLinkIn)
xinStereoDepthConfig = pipeline.create(dai.node.XLinkIn)

xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutConfMap = pipeline.create(dai.node.XLinkOut)
xoutDisparity = pipeline.create(dai.node.XLinkOut)
xoutRectifLeft = pipeline.create(dai.node.XLinkOut)
xoutRectifRight = pipeline.create(dai.node.XLinkOut)
xoutStereoCfg = pipeline.create(dai.node.XLinkOut)
if args.debug:
    xoutDebugLrCheckIt1 = pipeline.create(dai.node.XLinkOut)
    xoutDebugLrCheckIt2 = pipeline.create(dai.node.XLinkOut)
if args.dumpdisparitycostvalues:
    xoutDebugCostDump = pipeline.create(dai.node.XLinkOut)

xinStereoDepthConfig.setStreamName("stereoDepthConfig")
monoLeft.setStreamName('in_left')
monoRight.setStreamName('in_right')

xoutLeft.setStreamName('left')
xoutRight.setStreamName('right')
xoutDepth.setStreamName('depth')
xoutConfMap.setStreamName('confidence_map')
xoutDisparity.setStreamName('disparity')
xoutRectifLeft.setStreamName('rectified_left')
xoutRectifRight.setStreamName('rectified_right')
xoutStereoCfg.setStreamName('stereo_cfg')
if args.debug:
    xoutDebugLrCheckIt1.setStreamName('disparity_lr_check_iteration1')
    xoutDebugLrCheckIt2.setStreamName('disparity_lr_check_iteration2')
if args.dumpdisparitycostvalues:
    xoutDebugCostDump.setStreamName('disparity_cost_dump')

# Properties
stereo.initialConfig.setConfidenceThreshold(245)
stereo.setRectifyEdgeFillColor(0) # Black, to better see the cutout
stereo.setLeftRightCheck(lrcheck)
stereo.setExtendedDisparity(extended)
stereo.setSubpixel(subpixel)
# allocates resources for worst case scenario
# allowing runtime switch of stereo modes
stereo.setRuntimeModeSwitch(True)

# Linking
monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)
xinStereoDepthConfig.out.link(stereo.inputConfig)
stereo.syncedLeft.link(xoutLeft.input)
stereo.syncedRight.link(xoutRight.input)
if outDepth:
    stereo.depth.link(xoutDepth.input)
if outConfidenceMap:
    stereo.confidenceMap.link(xoutConfMap.input)
stereo.disparity.link(xoutDisparity.input)
if outRectified:
    stereo.rectifiedLeft.link(xoutRectifLeft.input)
    stereo.rectifiedRight.link(xoutRectifRight.input)
stereo.outConfig.link(xoutStereoCfg.input)
if args.debug:
    stereo.debugDispLrCheckIt1.link(xoutDebugLrCheckIt1.input)
    stereo.debugDispLrCheckIt2.link(xoutDebugLrCheckIt2.input)
if args.dumpdisparitycostvalues:
    stereo.debugDispCostDump.link(xoutDebugCostDump.input)

StereoConfigHandler(stereo.initialConfig.get())
StereoConfigHandler.registerWindow('disparity')
if outDepth:
    StereoConfigHandler.registerWindow('depth')

stereo.setInputResolution(width, height)
stereo.setRectification(False)
baseline = 75
fov = 71.86
focal = width / (2 * math.tan(fov / 2 / 180 * math.pi))

streams = ['left', 'right']
if outRectified:
    streams.extend(['rectified_left', 'rectified_right'])
streams.append('disparity')
if outDepth:
    streams.append('depth')
if outConfidenceMap:
    streams.append('confidence_map')
debugStreams = []
if args.debug:
    debugStreams.extend(['disparity_lr_check_iteration1', 'disparity_lr_check_iteration2'])
if args.dumpdisparitycostvalues:
    debugStreams.append('disparity_cost_dump')

def convertToCv2Frame(name, image, config):

    maxDisp = config.getMaxDisparity()
    subpixelLevels = pow(2, config.get().algorithmControl.subpixelFractionalBits)
    subpixel = config.get().algorithmControl.enableSubpixel
    dispIntegerLevels = maxDisp if not subpixel else maxDisp / subpixelLevels

    frame = image.getFrame()

    # frame.tofile(name+".raw")

    if name == 'depth':
        dispScaleFactor = baseline * focal
        with np.errstate(divide='ignore'):
            frame = dispScaleFactor / frame

        frame = (frame * 255. / dispIntegerLevels).astype(np.uint8)
        frame = cv2.applyColorMap(frame, cv2.COLORMAP_HOT)
    elif 'confidence_map' in name:
        pass
    elif name == 'disparity_cost_dump':
        # frame.tofile(name+'.raw')
        pass
    elif 'disparity' in name:
        if 1: # Optionally, extend disparity range to better visualize it
            frame = (frame * 255. / maxDisp).astype(np.uint8)

        if 1: # Optionally, apply a color map
            frame = cv2.applyColorMap(frame, cv2.COLORMAP_HOT)

    return frame

print("Connecting and starting the pipeline")
# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    stereoDepthConfigInQueue = device.getInputQueue("stereoDepthConfig")
    inStreams = ['in_right', 'in_left']
    inStreamsCameraID = [dai.CameraBoardSocket.RIGHT, dai.CameraBoardSocket.LEFT]
    in_q_list = []
    for s in inStreams:
        q = device.getInputQueue(s)
        in_q_list.append(q)

    # Create a receive queue for each stream
    q_list = []
    q_list_debug = []
    for s in streams:
        q = device.getOutputQueue(s, 8, blocking=False)
        q_list.append(q)

    if args.debug or args.dumpdisparitycostvalues:
        q_list_debug = q_list.copy()
        for s in debugStreams:
            q = device.getOutputQueue(s, 8, blocking=False)
            q_list_debug.append(q)

    inCfg = device.getOutputQueue("stereo_cfg", 8, blocking=False)

    # Need to set a timestamp for input frames, for the sync stage in Stereo node
    timestamp_ms = 0
    index = 0
    while True:
        # Handle input streams, if any
        if in_q_list:
            dataset_size = 1  # Number of image pairs
            frame_interval_ms = 50
            for i, q in enumerate(in_q_list):
                path = args.dataset + '/' + str(index) + '/' + q.getName() + '.png'
                data = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
                data = cv2.resize(data, (width, height), interpolation = cv2.INTER_AREA)
                data = data.reshape(height*width)
                tstamp = datetime.timedelta(seconds = timestamp_ms // 1000,
                                            milliseconds = timestamp_ms % 1000)
                img = dai.ImgFrame()
                img.setData(data)
                img.setTimestamp(tstamp)
                img.setInstanceNum(inStreamsCameraID[i])
                img.setType(dai.ImgFrame.Type.RAW8)
                img.setWidth(width)
                img.setHeight(height)
                q.send(img)
                if timestamp_ms == 0:  # Send twice for first iteration
                    q.send(img)
                # print("Sent frame: {:25s}".format(path), 'timestamp_ms:', timestamp_ms)
            timestamp_ms += frame_interval_ms
            index = (index + 1) % dataset_size
            sleep(frame_interval_ms / 1000)

        # Handle output streams
        currentConfig = inCfg.get()

        lrCheckEnabled = currentConfig.get().algorithmControl.enableLeftRightCheck
        queues = q_list

        if (args.debug and lrCheckEnabled) or args.dumpdisparitycostvalues:
            queues = q_list_debug
        else:
            for s in debugStreams:
                cv2.destroyWindow(s)

        for q in queues:
            if q.getName() in ['left', 'right']: continue
            data = q.get()
            frame = convertToCv2Frame(q.getName(), data, currentConfig)
            cv2.imshow(q.getName(), frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break

        StereoConfigHandler.handleKeypress(key, stereoDepthConfigInQueue)
