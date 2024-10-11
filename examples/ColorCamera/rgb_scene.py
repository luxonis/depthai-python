#!/usr/bin/env python3

import cv2
import depthai as dai
from itertools import cycle

scenes = cycle([item for name, item in vars(dai.RawCameraControl.SceneMode).items() if name != "UNSUPPORTED" and name.isupper()])
effects = cycle([item for name, item in vars(dai.RawCameraControl.EffectMode).items() if name.isupper()])
curr_scene = "OFF"
curr_effect = "OFF"

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setIspScale(1,3)

xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutRgb.setStreamName("video")
camRgb.video.link(xoutRgb.input)

camControlIn = pipeline.create(dai.node.XLinkIn)
camControlIn.setStreamName("camControl")
camControlIn.out.link(camRgb.inputControl)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    videoQ = device.getOutputQueue(name="video", maxSize=4, blocking=False)
    ctrlQ = device.getInputQueue(name="camControl")

    def putText(frame, text, coords):
        cv2.putText(frame, text, coords, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 4)
        cv2.putText(frame, text, coords, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

    while True:
        videoIn = videoQ.tryGet()
        if videoIn is not None:
            frame = videoIn.getCvFrame()
            putText(frame, f"[E] Effect: {curr_effect}", (10, 20))
            putText(frame, f"[S] Scene: {curr_scene}", (10, 40))
            cv2.imshow("video", frame)

        key = cv2.waitKey(1)
        if key == ord('e') or key == ord('E'):
            effect = next(effects)
            print("Switching colorCamera effect:", str(effect))
            curr_effect = str(effect).lstrip("EffectMode.")
            cfg = dai.CameraControl()
            cfg.setEffectMode(effect)
            ctrlQ.send(cfg)
        # Scene currently doesn't work
        elif key == ord('s') or key == ord('S'):
            scene = next(scenes)
            print("Currently doesn't work! Switching colorCamera Scene:", str(scene))
            curr_scene = str(scene).lstrip("SceneMode.")
            cfg = dai.CameraControl()
            cfg.setSceneMode(scene)
            ctrlQ.send(cfg)
        elif key == ord('q'):
            break
