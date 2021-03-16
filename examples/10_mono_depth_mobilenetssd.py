#!/usr/bin/env python3

from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np

# Get argument first
mobilenet_path = str((Path(__file__).parent / Path('models/mobilenet.blob')).resolve().absolute())
if len(sys.argv) > 1:
    mobilenet_path = sys.argv[1]


# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - mono (grayscale) cameras
left = pipeline.createMonoCamera()
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
left.setBoardSocket(dai.CameraBoardSocket.LEFT)

right = pipeline.createMonoCamera()
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create a node that will produce the depth map (using disparity output as it's easier to visualize depth this way)
stereo = pipeline.createStereoDepth()
stereo.setOutputRectified(True) # The rectified streams are horizontally mirrored by default
stereo.setOutputDepth(True)
stereo.setConfidenceThreshold(255)
stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout from rectification (black stripe on the edges)

left.out.link(stereo.left)
right.out.link(stereo.right)

# Create a node to convert the grayscale frame into the nn-acceptable form
manip = pipeline.createImageManip()
manip.initialConfig.setResize(300, 300)
# The NN model expects BGR input. By default ImageManip output type would be same as input (gray in this case)
manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)
stereo.rectifiedRight.link(manip.inputImage)

# Define a neural network that will make predictions based on the source frames
detectionNN = pipeline.createNeuralNetwork()
detectionNN.setBlobPath(mobilenet_path)
manip.out.link(detectionNN.input)

# Create outputs
xout_depth = pipeline.createXLinkOut()
xout_depth.setStreamName("depth")

stereo.depth.link(xout_depth.input)

xout_right = pipeline.createXLinkOut()
xout_right.setStreamName("rectifiedRight")
manip.out.link(xout_right.input)

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("nn")
detectionNN.out.link(xout_nn.input)

# MobilenetSSD label nnLabels
nnLabels = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
         "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale / depth frames and nn data from the outputs defined above
    qRight = device.getOutputQueue("rectifiedRight", maxSize=4, blocking=False)
    qDepth = device.getOutputQueue("depth", maxSize=4, blocking=False)
    qNN = device.getOutputQueue("nn", maxSize=4, blocking=False)

    rightFrame = None
    depthFrameColor = None
    bboxes = []
    labels = []
    confidences = []


    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frame_norm(frame, bbox):
        norm_vals = np.full(len(bbox), frame.shape[0])
        norm_vals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * norm_vals).astype(int)


    while True:
        # instead of get (blocking) used tryGet (nonblocking) which will return the available data or None otherwise
        inRight = qRight.tryGet()
        inNN = qNN.tryGet()
        inDepth = qDepth.tryGet()

        if inRight is not None:
            # if the grayscale frame data is available, transform the 1D data into a HxWxC frame
            rightFrame = inRight.getCvFrame()

        if inNN is not None:
            # one detection has 7 numbers, and the last detection is followed by -1 digit, which later is filled with 0
            bboxes = np.array(inNN.getFirstLayerFp16())
            # take only the results before -1 digit
            bboxes = bboxes[:np.where(bboxes == -1)[0][0]]
            # transform the 1D array into Nx7 matrix
            bboxes = bboxes.reshape((bboxes.size // 7, 7))
            # filter out the results which confidence less than a defined threshold
            bboxes = bboxes[bboxes[:, 2] > 0.5]
            # Cut bboxes and labels
            labels = bboxes[:, 1].astype(int)
            confidences = bboxes[:, 2]
            bboxes = bboxes[:, 3:7]

        if inDepth is not None:
            # data is originally represented as a flat 1D array, it needs to be converted into HxW form

            depthFrame = inDepth.getFrame()

            depthFrameColor = cv2.normalize(depthFrame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
            depthFrameColor = cv2.equalizeHist(depthFrameColor)
            depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_HOT)

        if rightFrame is not None:
            # if the frame is available, draw bounding boxes on it and show the frame
            for rawBbox, label, conf in zip(bboxes, labels, confidences):
                bbox = frame_norm(rightFrame, rawBbox)
                cv2.rectangle(rightFrame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
                cv2.putText(rightFrame, nnLabels[label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
                cv2.putText(depthFrameColor, f"{int(conf * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.imshow("rectified right", rightFrame)

        if depthFrameColor is not None:
            for rawBbox, label, conf in zip(bboxes, labels, confidences):
                bbox = frame_norm(depthFrameColor, rawBbox)
                cv2.rectangle(depthFrameColor, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (0, 0, 255), 2)
                cv2.putText(depthFrameColor, nnLabels[label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, (0, 0, 255))
                cv2.putText(depthFrameColor, f"{int(conf * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, (0, 0, 255))
            cv2.imshow("depth", depthFrameColor)

        if cv2.waitKey(1) == ord('q'):
            break
