import depthai as dai
import cv2
import numpy as np
import math

# User-defined constants
WARNING = 500  # 50cm, orange
CRITICAL = 300  # 30cm, red

# Create pipeline
pipeline = dai.Pipeline()

# Color camera
camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setPreviewSize(300, 300)
camRgb.setInterleaved(False)

# Define source - stereo depth cameras
left = pipeline.create(dai.node.MonoCamera)
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
left.setBoardSocket(dai.CameraBoardSocket.LEFT)

right = pipeline.create(dai.node.MonoCamera)
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)

# Create stereo depth node
stereo = pipeline.create(dai.node.StereoDepth)
stereo.setConfidenceThreshold(50)
stereo.setLeftRightCheck(True)
stereo.setExtendedDisparity(True)

# Linking
left.out.link(stereo.left)
right.out.link(stereo.right)

# Spatial location calculator configuration
slc = pipeline.create(dai.node.SpatialLocationCalculator)
for x in range(15):
    for y in range(9):
        config = dai.SpatialLocationCalculatorConfigData()
        config.depthThresholds.lowerThreshold = 200
        config.depthThresholds.upperThreshold = 10000
        config.roi = dai.Rect(dai.Point2f((x+0.5)*0.0625, (y+0.5)*0.1), dai.Point2f((x+1.5)*0.0625, (y+1.5)*0.1))
        config.calculationAlgorithm = dai.SpatialLocationCalculatorAlgorithm.MEDIAN
        slc.initialConfig.addROI(config)

stereo.depth.link(slc.inputDepth)
stereo.setDepthAlign(dai.CameraBoardSocket.RGB)

# Create output
slcOut = pipeline.create(dai.node.XLinkOut)
slcOut.setStreamName('slc')
slc.out.link(slcOut.input)

colorOut = pipeline.create(dai.node.XLinkOut)
colorOut.setStreamName('color')
camRgb.video.link(colorOut.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    # Output queues will be used to get the color mono frames and spatial location data
    qColor = device.getOutputQueue(name="color", maxSize=4, blocking=False)
    qSlc = device.getOutputQueue(name="slc", maxSize=4, blocking=False)

    fontType = cv2.FONT_HERSHEY_TRIPLEX

    while True:
        inColor = qColor.get()  # Try to get a frame from the color camera
        inSlc = qSlc.get()  # Try to get spatial location data

        if inColor is None:
            print("No color camera data")
        if inSlc is None:
            print("No spatial location data")

        colorFrame = None
        if inColor is not None:
            colorFrame = inColor.getCvFrame()  # Fetch the frame from the color mono camera
            

        if inSlc is not None and colorFrame is not None:
            slc_data = inSlc.getSpatialLocations()
            for depthData in slc_data:
                roi = depthData.config.roi
                roi = roi.denormalize(width=colorFrame.shape[1], height=colorFrame.shape[0])

                xmin = int(roi.topLeft().x)
                ymin = int(roi.topLeft().y)
                xmax = int(roi.bottomRight().x)
                ymax = int(roi.bottomRight().y)

                coords = depthData.spatialCoordinates
                distance = math.sqrt(coords.x ** 2 + coords.y ** 2 + coords.z ** 2)

                if distance == 0:  # Invalid
                    continue

                # Determine color based on distance
                if distance < CRITICAL:
                    color = (0, 0, 255)  # Red
                elif distance < WARNING:
                    color = (0, 140, 255)  # Orange
                else:
                    continue  # Skip drawing for non-critical/non-warning distances

                # Draw rectangle and distance text on the color frame
                cv2.rectangle(colorFrame, (xmin, ymin), (xmax, ymax), color, thickness=2)
                cv2.putText(colorFrame, "{:.1f}m".format(distance / 1000), (xmin + 10, ymin + 20), fontType, 0.5, color)

            # Display the color frame
            cv2.imshow('Left Mono Camera', colorFrame)
            if cv2.waitKey(1) == ord('q'):
                break
