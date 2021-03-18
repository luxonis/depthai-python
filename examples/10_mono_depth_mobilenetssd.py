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
detection_nn = pipeline.createMobileNetDetectionNetwork()
detection_nn.setConfidenceThreshold(0.5)
detection_nn.setBlobPath(mobilenet_path)
detection_nn.setNumInferenceThreads(2)
detection_nn.input.setBlocking(False)
manip.out.link(detection_nn.input)

# Create outputs
xout_depth = pipeline.createXLinkOut()
xout_depth.setStreamName("depth")

stereo.depth.link(xout_depth.input)

xout_right = pipeline.createXLinkOut()
xout_right.setStreamName("rectifiedRight")
manip.out.link(xout_right.input)

xout_nn = pipeline.createXLinkOut()
xout_nn.setStreamName("nn")
detection_nn.out.link(xout_nn.input)

# MobilenetSSD label nnLabels
nn_labels = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow",
             "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

# Pipeline defined, now the device is connected to
with dai.Device(pipeline) as device:
    # Start pipeline
    device.startPipeline()

    # Output queues will be used to get the grayscale / depth frames and nn data from the outputs defined above
    q_right = device.getOutputQueue("rectifiedRight", maxSize=4, blocking=False)
    q_depth = device.getOutputQueue("depth", maxSize=4, blocking=False)
    q_nn = device.getOutputQueue("nn", maxSize=4, blocking=False)

    right_frame = None
    depth_frame = None
    detections = []

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frame_norm(frame, bbox):
        norm_vals = np.full(len(bbox), frame.shape[0])
        norm_vals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * norm_vals).astype(int)

    def display_frame(name, frame):
        for detection in detections:
            bbox = frame_norm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
            cv2.putText(frame, nn_labels[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
        cv2.imshow(name, frame)


    while True:
        # instead of get (blocking) used tryGet (nonblocking) which will return the available data or None otherwise
        in_right = q_right.tryGet()
        in_nn = q_nn.tryGet()
        in_depth = q_depth.tryGet()

        if in_right is not None:
            right_frame = in_right.getCvFrame()

        if in_nn is not None:
            detections = in_nn.detections

        if in_depth is not None:
            depth_frame = in_depth.getFrame()
            depth_frame = cv2.normalize(depth_frame, None, 255, 0, cv2.NORM_INF, cv2.CV_8UC1)
            depth_frame = cv2.equalizeHist(depth_frame)
            depth_frame = cv2.applyColorMap(depth_frame, cv2.COLORMAP_HOT)

        if right_frame is not None:
            display_frame("rectified right", right_frame)

        if depth_frame is not None:
            display_frame("depth", depth_frame)

        if cv2.waitKey(1) == ord('q'):
            break
