import blobconverter
import cv2
import depthai
from depthai import node
import numpy as np

class HelloWorld(depthai.Node):
    def __run__(self, image, detections):
        def frameNorm(frame, bbox):
            normVals = np.full(len(bbox), frame.shape[0])
            normVals[::2] = frame.shape[1]
            return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

        frame = image.getCvFrame()
        for detection in detections.detections:
            # for each bounding box, we first normalize it to match the frame size
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            # and then draw a rectangle on the frame to show the actual result
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
        # After all the drawing is finished, we show the frame on the screen
        cv2.imshow("preview", frame)

        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

pipeline = depthai.Pipeline()
with pipeline:
    image = node.ColorCamera().preview
    detections = node.MobileNetDetectionNetwork(image,
            blob_path=blobconverter.from_zoo(name='mobilenet-ssd', shaves=6))
    HelloWorld(image, detections)
depthai.run(pipeline)
