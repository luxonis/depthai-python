import cv2
import depthai as dai
import numpy as np
import time


from pathlib import Path

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
nn = pipeline.create(dai.node.NeuralNetwork)
xoutRgb = pipeline.create(dai.node.XLinkOut)
nnOut = pipeline.create(dai.node.XLinkOut)
nnNetworkOut = pipeline.create(dai.node.XLinkOut)

xoutRgb.setStreamName("rgb")
nnOut.setStreamName("nn")
nnNetworkOut.setStreamName("nnNetwork")

# Properties
camRgb.setPreviewSize(224, 224)
camRgb.setInterleaved(False)
camRgb.setFps(40)
# Define a neural network that will make predictions based on the source frames
nn.setBlobPath("${Path to resnet blob}")
nn.setNumInferenceThreads(2)
nn.input.setBlocking(False)

# Linking
nn.passthrough.link(xoutRgb.input)
camRgb.preview.link(nn.input)
nn.out.link(nnOut.input)

labels = []

# Parse
with open(str((Path(__file__).parent / Path("resnet_labels.txt")).resolve().absolute())) as f:
    for line in f:
        splitLine = line.split("\'")
        labels.append(splitLine[1])

with dai.Device(pipeline) as device:

    # Output queues will be used to get the rgb frames and nn data from the outputs defined above
    qRgb = device.getOutputQueue(name="rgb", maxSize=4, blocking=False)
    qDet = device.getOutputQueue(name="nn", maxSize=4, blocking=False)

    frame = None
    detections = []
    startTime = time.monotonic()
    counter = 0
    color2 = (255, 255, 255)

    while True:
        inRgb: dai.ImgFrame = qRgb.get()
        inDet: dai.NNData = qDet.get()

        deviceTimestamp = inRgb.getTimestamp()
        if inRgb is not None:
            frame = inRgb.getCvFrame()
            cv2.putText(frame, "NN fps: {:.2f}".format(counter / (time.monotonic() - startTime)),
                        (2, frame.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color2)

        if inDet is not None:
            detData = inDet.getFirstLayerFp16()
            npDetData = np.array(detData)
            softmaxOutput = np.exp(npDetData)/sum(np.exp(npDetData))
            labelIndex = np.argmax(softmaxOutput)
            # print(softmaxOutput[labelIndex])
            # print("Detection:")
            # print(labels[labelIndex])
            if frame is not None:
                cv2.putText(frame, f"Detection: {labels[labelIndex]}", (5, 15), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color2)
            counter += 1

        # If the frame is available, draw bounding boxes on it and show the frame
        if frame is not None:
            cv2.imshow("preview", frame)

        if cv2.waitKey(1) == ord('q'):
            break
