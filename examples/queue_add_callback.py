#!/usr/bin/env python3
import cv2
import depthai as dai
import queue

# Start defining a pipeline
pipeline = dai.Pipeline()

# Add all three cameras
camRgb = pipeline.createColorCamera()
camRgb.setPreviewSize(300, 300)

left = pipeline.createMonoCamera()
left.setBoardSocket(dai.CameraBoardSocket.LEFT)
left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)

right = pipeline.createMonoCamera()
right.setBoardSocket(dai.CameraBoardSocket.RIGHT)
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)

# Create XLink output
xout = pipeline.createXLinkOut()
xout.setStreamName("frames")

# Stream all the camera streams through the same XLink node
camRgb.preview.link(xout.input)
left.out.link(xout.input)
right.out.link(xout.input)

q = queue.Queue()


def newFrame(inFrame):
    global q
    # Get "stream name" from the instance number
    num = inFrame.getInstanceNum()
    name = "color" if num == 0 else "left" if num == 1 else "right"
    frame = inFrame.getCvFrame()
    # This is a different thread and you could use it to
    # run image processing algorithms here
    q.put({"name": name, "frame": frame})


# Pipeline is defined, now we can connect to the device
with dai.Device(pipeline) as device:
    device.startPipeline()

    # Add callback to the output queue "frames" for all newly arrived frames (color, left, right)
    device.getOutputQueue(name="frames", maxSize=4, blocking=False).addCallback(newFrame)

    while True:
        # You could also get the data as non-blocking (block=False)
        data = q.get(block=True)
        cv2.imshow(data["name"], data["frame"])

        if cv2.waitKey(1) == ord('q'):
            break
