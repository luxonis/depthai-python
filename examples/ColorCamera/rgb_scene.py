import cv2
import depthai as dai
from itertools import cycle

scenes = cycle([item for name, item in vars(dai.RawCameraControl.SceneMode).items() if name != "UNSUPPORTED" and name.isupper()])
effects = cycle([item for name, item in vars(dai.RawCameraControl.EffectMode).items() if name.isupper()])

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.createColorCamera()
camRgb.setVideoSize(800,800)

xoutRgb = pipeline.createXLinkOut()
xoutRgb.setStreamName("video")
camRgb.video.link(xoutRgb.input)

camControlIn = pipeline.createXLinkIn()
camControlIn.setStreamName("camControl")
camControlIn.out.link(camRgb.inputControl)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    videoQ = device.getOutputQueue(name="video", maxSize=4, blocking=False)
    ctrlQ = device.getInputQueue(name="camControl")

    while True:
        videoIn = videoQ.tryGet()
        if videoIn is not None:
            # Get BGR frame from NV12 encoded video frame to show with opencv
            cv2.imshow("video", videoIn.getCvFrame())

        key = cv2.waitKey(1)
        if key == ord('e') or key == ord('E'):
            effect = next(effects)
            print("Switching colorCamera effect:", str(effect))
            cfg = dai.CameraControl()
            cfg.setEffectMode(effect)
            ctrlQ.send(cfg)
        # Scene currently doesn't work
        elif key == ord('s') or key == ord('S'):
            scene = next(scenes)
            print("Currently doesn't work! Switching colorCamera Scene:", str(scene))
            cfg = dai.CameraControl()
            cfg.setSceneMode(scene)
            ctrlQ.send(cfg)
        elif key == ord('q'):
            break
