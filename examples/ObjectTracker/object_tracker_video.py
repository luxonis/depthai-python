#!/usr/bin/env python3

from pathlib import Path
import cv2
import depthai as dai
import numpy as np
import time
import argparse

labelMap = ["person", ""]

nnPathDefault = str((Path(__file__).parent / Path('../models/person-detection-retail-0013_openvino_2021.4_7shave.blob')).resolve().absolute())
videoPathDefault = str((Path(__file__).parent / Path('../models/construction_vest.mp4')).resolve().absolute())
parser = argparse.ArgumentParser()
parser.add_argument('-nnPath', help="Path to mobilenet detection network blob", default=nnPathDefault)
parser.add_argument('-v', '--videoPath', help="Path to video frame", default=videoPathDefault)

args = parser.parse_args()

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
manip = pipeline.create(dai.node.ImageManip)
objectTracker = pipeline.create(dai.node.ObjectTracker)
detectionNetwork = pipeline.create(dai.node.MobileNetDetectionNetwork)

manipOut = pipeline.create(dai.node.XLinkOut)
xinFrame = pipeline.create(dai.node.XLinkIn)
trackerOut = pipeline.create(dai.node.XLinkOut)
xlinkOut = pipeline.create(dai.node.XLinkOut)
nnOut = pipeline.create(dai.node.XLinkOut)

manipOut.setStreamName("manip")
xinFrame.setStreamName("inFrame")
xlinkOut.setStreamName("trackerFrame")
trackerOut.setStreamName("tracklets")
nnOut.setStreamName("nn")

# Properties
xinFrame.setMaxDataSize(1920*1080*3)

manip.initialConfig.setResizeThumbnail(544, 320)
# manip.initialConfig.setResize(384, 384)
# manip.initialConfig.setKeepAspectRatio(False) #squash the image to not lose FOV
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manip.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)
manip.inputImage.setBlocking(True)

# setting node configs
detectionNetwork.setBlobPath(args.nnPath)
detectionNetwork.setConfidenceThreshold(0.5)
detectionNetwork.input.setBlocking(True)

objectTracker.inputTrackerFrame.setBlocking(True)
objectTracker.inputDetectionFrame.setBlocking(True)
objectTracker.inputDetections.setBlocking(True)
objectTracker.setDetectionLabelsToTrack([1])  # track only person
# possible tracking types: ZERO_TERM_COLOR_HISTOGRAM, ZERO_TERM_IMAGELESS, SHORT_TERM_IMAGELESS, SHORT_TERM_KCF
objectTracker.setTrackerType(dai.TrackerType.ZERO_TERM_COLOR_HISTOGRAM)
# take the smallest ID when new object is tracked, possible options: SMALLEST_ID, UNIQUE_ID
objectTracker.setTrackerIdAssignmentPolicy(dai.TrackerIdAssignmentPolicy.SMALLEST_ID)

# Linking
manip.out.link(manipOut.input)
manip.out.link(detectionNetwork.input)
xinFrame.out.link(manip.inputImage)
xinFrame.out.link(objectTracker.inputTrackerFrame)
detectionNetwork.out.link(nnOut.input)
detectionNetwork.out.link(objectTracker.inputDetections)
detectionNetwork.passthrough.link(objectTracker.inputDetectionFrame)
objectTracker.out.link(trackerOut.input)
objectTracker.passthroughTrackerFrame.link(xlinkOut.input)

# Connect and start the pipeline
with dai.Device(pipeline) as device:

    qIn = device.getInputQueue(name="inFrame")
    trackerFrameQ = device.getOutputQueue(name="trackerFrame", maxSize=4)
    tracklets = device.getOutputQueue(name="tracklets", maxSize=4)
    qManip = device.getOutputQueue(name="manip", maxSize=4)
    qDet = device.getOutputQueue(name="nn", maxSize=4)

    startTime = time.monotonic()
    counter = 0
    fps = 0
    detections = []
    frame = None

    def to_planar(arr: np.ndarray, shape: tuple) -> np.ndarray:
        return cv2.resize(arr, shape).transpose(2, 0, 1).flatten()

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

    def displayFrame(name, frame):
        for detection in detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
            cv2.putText(frame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
        cv2.imshow(name, frame)

    cap = cv2.VideoCapture(args.videoPath)
    baseTs = time.monotonic()
    simulatedFps = 30
    inputFrameShape = (1920, 1080)

    while cap.isOpened():
        read_correctly, frame = cap.read()
        if not read_correctly:
            break

        img = dai.ImgFrame()
        img.setType(dai.ImgFrame.Type.BGR888p)
        img.setData(to_planar(frame, inputFrameShape))
        img.setTimestamp(baseTs)
        baseTs += 1/simulatedFps

        img.setWidth(inputFrameShape[0])
        img.setHeight(inputFrameShape[1])
        qIn.send(img)

        trackFrame = trackerFrameQ.tryGet()
        if trackFrame is None:
            continue

        track = tracklets.get()
        manip = qManip.get()
        inDet = qDet.get()

        counter+=1
        current_time = time.monotonic()
        if (current_time - startTime) > 1 :
            fps = counter / (current_time - startTime)
            counter = 0
            startTime = current_time

        detections = inDet.detections
        manipFrame = manip.getCvFrame()
        displayFrame("nn", manipFrame)

        color = (255, 0, 0)
        trackerFrame = trackFrame.getCvFrame()
        trackletsData = track.tracklets
        for t in trackletsData:
            roi = t.roi.denormalize(trackerFrame.shape[1], trackerFrame.shape[0])
            x1 = int(roi.topLeft().x)
            y1 = int(roi.topLeft().y)
            x2 = int(roi.bottomRight().x)
            y2 = int(roi.bottomRight().y)

            try:
                label = labelMap[t.label]
            except:
                label = t.label

            cv2.putText(trackerFrame, str(label), (x1 + 10, y1 + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(trackerFrame, f"ID: {[t.id]}", (x1 + 10, y1 + 35), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(trackerFrame, t.status.name, (x1 + 10, y1 + 50), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.rectangle(trackerFrame, (x1, y1), (x2, y2), color, cv2.FONT_HERSHEY_SIMPLEX)

        cv2.putText(trackerFrame, "Fps: {:.2f}".format(fps), (2, trackerFrame.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color)

        cv2.imshow("tracker", trackerFrame)

        if cv2.waitKey(1) == ord('q'):
            break
