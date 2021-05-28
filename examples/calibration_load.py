#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import argparse
import numpy as np
import cv2

def createStereoPipeline():
    p = dai.Pipeline()

    monoLeft = p.createMonoCamera();
    monoRight = p.createMonoCamera();
    stereo = p.createStereoDepth();
    xoutDepth = p.createXLinkOut();
    xoutDepth.setStreamName("depth");

    # MonoCamera
    monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P);
    monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT);
    # monoLeft.setFps(5.0);
    monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P);
    monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT);
    # monoRight.setFps(5.0);

    monoLeft.out.link(stereo.left);
    monoRight.out.link(stereo.right);
    stereo.depth.link(xoutDepth.input);

    return p;

# Connect Device
calibJsonFile = str((Path(__file__).parent / Path('models/depthai_calib.json')).resolve().absolute())

parser = argparse.ArgumentParser()
parser.add_argument('calibJsonFile', nargs='?', help="Path to V6 calibration file in json", default=calibJsonFile)
args = parser.parse_args()


calibData = dai.CalibrationHandler(args.calibJsonFile);
pipeline = createStereoPipeline()
pipeline.setCalibrationData(calibData)

with dai.Device() as device:
    device.startPipeline(pipeline)
    q = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
    while True:
        inDepth = q.get()  # blocking call, will wait until a new data has arrived
        frame = inDepth.getFrame()

        # frame is ready to be shown
        cv2.imshow("depth", frame)

        if cv2.waitKey(1) == ord('q'):
            break