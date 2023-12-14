import depthai as dai
import cv2

pipeline = dai.Pipeline()

# Define sources and outputs
camRgb: dai.node.Camera = pipeline.create(dai.node.Camera)

#Properties
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setSize((1280, 800))

# Linking
videoOut = pipeline.create(dai.node.XLinkOut)
videoOut.setStreamName("video")
camRgb.video.link(videoOut.input)

ispOut = pipeline.create(dai.node.XLinkOut)
ispOut.setStreamName("isp")
camRgb.isp.link(ispOut.input)

with dai.Device(pipeline) as device:
    video = device.getOutputQueue(name="video", maxSize=1, blocking=False)
    isp = device.getOutputQueue(name="isp", maxSize=1, blocking=False)

    while True:
        if video.has():
            cv2.imshow("video", video.get().getCvFrame())
        if isp.has():
            cv2.imshow("isp", isp.get().getCvFrame())
        if cv2.waitKey(1) == ord('q'):
            break
