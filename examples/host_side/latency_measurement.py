import depthai as dai
import numpy as np
# Create pipeline
pipeline = dai.Pipeline()
# This might improve reducing the latency on some systems
pipeline.setXLinkChunkSize(0)

# Define source and output
camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setFps(60)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("out")
camRgb.isp.link(xout.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    print(device.getUsbSpeed())
    q = device.getOutputQueue(name="out")
    diffs = np.array([])
    while True:
        imgFrame = q.get()
        # Latency in miliseconds 
        latencyMs = (dai.Clock.now() - imgFrame.getTimestamp()).total_seconds() * 1000
        diffs = np.append(diffs, latencyMs)
        print('Latency: {:.2f} ms, Average latency: {:.2f} ms, Std: {:.2f}'.format(latencyMs, np.average(diffs), np.std(diffs)))
        
        # Not relevant for this example
        # cv2.imshow('frame', imgFrame.getCvFrame())