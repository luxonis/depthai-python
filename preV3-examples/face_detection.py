import depthai as dai
import cv2
import numpy as np
import blobconverter

class Viewer(dai.Node):
    def __run__(self, 
                image: dai.message.ImgFrame, 
                detections: dai.message.ImgDetections):
        if cv2.waitKey(1) == ord("q"): raise KeyboardInterrupt()

        def frameNorm(frame, bbox):
            normVals = np.full(len(bbox), frame.shape[0])
            normVals[::2] = frame.shape[1]
            return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

        frame = image.getCvFrame()
        for detection in detections.detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
        cv2.imshow("Face detection", frame)

pipeline = dai.Pipeline()
with pipeline:
    image = dai.node.ColorCamera().preview
    detections = dai.node.MobileNetDetectionNetwork(image,
        blob_path=blobconverter.from_zoo(name="face-detection-retail-0004", shaves=6)
    )
    Viewer(image, detections)
dai.run(pipeline)
