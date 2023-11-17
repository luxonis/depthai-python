import depthai as dai
import numpy as np
import cv2

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

sync.setSyncThresholdMs(100)

monoLeft.out.link(stereo.left)
monoRight.out.link(stereo.right)

stereo.depth.link(sync.inputs["depth"])
color.video.link(sync.inputs["video"])

sync.out.link(xoutGrp.input)

with dai.Device(pipeline) as device:
    queue = device.getOutputQueue("xout", 10, False)
    while True:
        msgGrp = queue.get()
        for name, msg in msgGrp:
            frame = msg.getCvFrame()
            if name == "depth":
                frame = frame.astype(np.uint16)
            cv2.imshow(name, frame)
        if cv2.waitKey(100) == ord("q"):
            break

