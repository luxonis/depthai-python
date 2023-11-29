import depthai as dai
import numpy as np
import cv2
from datetime import timedelta

pipeline = dai.Pipeline()

monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
color = pipeline.create(dai.node.ColorCamera)
stereo = pipeline.create(dai.node.StereoDepth)
sync = pipeline.create(dai.node.Sync)

xoutGrp = pipeline.create(dai.node.XLinkOut)

xoutGrp.setStreamName("xout")

monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("left")
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("right")

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_ACCURACY)

color.setCamera("color")

sync.setSyncThreshold(timedelta(milliseconds=50))

monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

stereo.disparity.link(sync.inputs["disparity"])
color.video.link(sync.inputs["video"])

sync.out.link(xoutGrp.input)

disparityMultiplier = 255.0 / stereo.initialConfig.getMaxDisparity()
with dai.Device(pipeline) as device:
    queue = device.getOutputQueue("xout", 10, False)
    while True:
        msgGrp = queue.get()
        for name, msg in msgGrp:
            frame = msg.getCvFrame()
            if name == "disparity":
                frame = (frame * disparityMultiplier).astype(np.uint8)
                frame = cv2.applyColorMap(frame, cv2.COLORMAP_JET)
            cv2.imshow(name, frame)
        if cv2.waitKey(1) == ord("q"):
            break
