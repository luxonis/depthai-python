#!/usr/bin/env python3

import cv2
import depthai as dai
import numpy as np

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - two mono (grayscale) cameras
left = pipeline.createMonoCamera()
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
left.setBoardSocket(dai.CameraBoardSocket.LEFT)

right = pipeline.createMonoCamera()
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create a node that will produce the depth map (using disparity output as it's easier to visualize depth this way)
stereo = pipeline.createStereoDepth()
stereo.setConfidenceThreshold(200)
stereo.setOutputDepth(True)
median   = dai.StereoDepthProperties.MedianFilter.KERNEL_7x7
stereo.setMedianFilter(median) # KERNEL_7x7 default

left.out.link(stereo.left)
right.out.link(stereo.right)

depthcalculator = pipeline.createDepthCalculator()
config = dai.DepthCalculatorConfig()
config.lower_threshold = 100
config.upper_threshold = 5000
rect = dai.Rect()
rect.xmin = 0.4
rect.ymin = 0.4
rect.xmax = 0.5
rect.ymax = 0.5
config.roi = rect
depthcalculator.addROI(config)

stereo.depth.link(depthcalculator.depthInput)

# Create output
xout_depth = pipeline.createXLinkOut()
xout_depth.setStreamName("depth")
stereo.depth.link(xout_depth.input)

xout_depth_average = pipeline.createXLinkOut()
xout_depth_average.setStreamName("depth_avg")
depthcalculator.out.link(xout_depth_average.input)

# Pipeline defined, now the device is assigned and pipeline is started
device = dai.Device(pipeline)
device.startPipeline()

# Output queue will be used to get the depth frames from the outputs defined above
q = device.getOutputQueue(name="depth", maxSize=4, blocking=False)
q2 = device.getOutputQueue(name="depth_avg", maxSize=4, blocking=False)

while True:
    in_depth = q.get() # blocking call, will wait until a new data has arrived
    in_depth_avg = q2.get() # blocking call, will wait until a new data has arrived
    
    depth_avg = in_depth_avg.getDepthData()

    for avg in depth_avg:
        roi = avg.config.roi
        average = avg.depth_avg
        print(f"Average depth {average}")


    # data is originally represented as a flat 1D array, it needs to be converted into HxW form
    frame = np.array(in_depth.getData()).astype(np.uint8).view(np.uint16).reshape((in_depth.getHeight(), in_depth.getWidth()))

    frame = np.ascontiguousarray(frame)
    # frame is transformed, the color map will be applied to highlight the depth info
    # frame is ready to be shown
    cv2.imshow("depth", frame)

    if cv2.waitKey(1) == ord('q'):
        break
