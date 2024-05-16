#!/usr/bin/env python3

import time
import cv2
import depthai as dai
import numpy as np

print(dai.__version__)

cvColorMap = cv2.applyColorMap(np.arange(256, dtype=np.uint8), cv2.COLORMAP_JET)
cvColorMap[0] = [0, 0, 0]

def create_pipeline():
    pipeline = dai.Pipeline()

    tof = pipeline.create(dai.node.ToF)

    # Configure the ToF node
    tofConfig = tof.initialConfig.get()
    # Disable for debugging:
    tofConfig.enableFPPNCorrection = True
    tofConfig.enableWiggleCorrection = True
    # tofConfig.enableTemperatureCorrection = True

    # Optional. Best accuracy, but adds motion blur.
    # see ToF node docs on how to reduce/eliminate motion blur.
    tofConfig.enableOpticalCorrection = True
    tofConfig.enablePhaseShuffleTemporalFilter = True
    tofConfig.phaseUnwrappingLevel = 4
    tofConfig.phaseUnwrapErrorThreshold = 300
    xinTofConfig = pipeline.create(dai.node.XLinkIn)
    xinTofConfig.setStreamName("tofConfig")
    xinTofConfig.out.link(tof.inputConfig)

    tof.initialConfig.set(tofConfig)

    cam_tof = pipeline.create(dai.node.Camera)
    cam_tof.setFps(60) # ToF node will produce depth frames at /2 of this rate
    cam_tof.setBoardSocket(dai.CameraBoardSocket.CAM_A)
    cam_tof.raw.link(tof.input)

    xout = pipeline.create(dai.node.XLinkOut)
    xout.setStreamName("depth")
    tof.depth.link(xout.input)

    tofConfig = tof.initialConfig.get()

    return pipeline, tofConfig


if __name__ == '__main__':
    pipeline, tofConfig = create_pipeline()

    with dai.Device(pipeline) as device:
        print('Connected cameras:', device.getConnectedCameraFeatures())
        qDepth = device.getOutputQueue(name="depth")

        tofConfigInQueue = device.getInputQueue("tofConfig")

        counter = 0
        while True:
            start = time.time()
            key = cv2.waitKey(1)
            if key == ord('f'):
                tofConfig.enableFPPNCorrection = not tofConfig.enableFPPNCorrection
                tofConfigInQueue.send(tofConfig)
            elif key == ord('o'):
                tofConfig.enableOpticalCorrection = not tofConfig.enableOpticalCorrection
                tofConfigInQueue.send(tofConfig)
            elif key == ord('w'):
                tofConfig.enableWiggleCorrection = not tofConfig.enableWiggleCorrection
                tofConfigInQueue.send(tofConfig)
            elif key == ord('t'):
                tofConfig.enableTemperatureCorrection = not tofConfig.enableTemperatureCorrection
                tofConfigInQueue.send(tofConfig)
            elif key == ord('q'):
                break
            elif key == ord('0'):
                tofConfig.enablePhaseUnwrapping = False
                tofConfig.phaseUnwrappingLevel = 0
                tofConfigInQueue.send(tofConfig)
            elif key == ord('1'):
                tofConfig.enablePhaseUnwrapping = True
                tofConfig.phaseUnwrappingLevel = 1
                tofConfigInQueue.send(tofConfig)
            elif key == ord('2'):
                tofConfig.enablePhaseUnwrapping = True
                tofConfig.phaseUnwrappingLevel = 2
                tofConfigInQueue.send(tofConfig)
            elif key == ord('3'):
                tofConfig.enablePhaseUnwrapping = True
                tofConfig.phaseUnwrappingLevel = 3
                tofConfigInQueue.send(tofConfig)
            elif key == ord('4'):
                tofConfig.enablePhaseUnwrapping = True
                tofConfig.phaseUnwrappingLevel = 4
                tofConfigInQueue.send(tofConfig)
            elif key == ord('5'):
                tofConfig.enablePhaseUnwrapping = True
                tofConfig.phaseUnwrappingLevel = 5
                tofConfigInQueue.send(tofConfig)
            elif key == ord('m'):
                medianSettings = [dai.MedianFilter.MEDIAN_OFF, dai.MedianFilter.KERNEL_3x3, dai.MedianFilter.KERNEL_5x5,
                                  dai.MedianFilter.KERNEL_7x7]
                currentMedian = tofConfig.median
                nextMedian = medianSettings[(medianSettings.index(currentMedian) + 1) % len(medianSettings)]
                print(f"Changing median to {nextMedian.name} from {currentMedian.name}")
                tofConfig.median = nextMedian
                tofConfigInQueue.send(tofConfig)

            imgFrame = qDepth.get()  # blocking call, will wait until a new data has arrived
            depth_map = imgFrame.getFrame()
            max_depth = (tofConfig.phaseUnwrappingLevel + 1) * 1874 # 80MHz modulation freq.
            depth_colorized = np.interp(depth_map, (0, max_depth), (0, 255)).astype(np.uint8)
            depth_colorized = cv2.applyColorMap(depth_colorized, cvColorMap)

            cv2.imshow("Colorized depth", depth_colorized)
            counter += 1

    device.close()