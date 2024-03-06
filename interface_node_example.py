import depthai as dai
import cv2
import os
import blobconverter
import typing
import numpy as np

class VisualizeDetectionsNode(dai.Node):
    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.ImgDetections):
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

class Haulier(dai.Node):
    def __node_init__(self):
        self.img_out = os.open("fifo/img.out", os.O_RDWR | os.O_NONBLOCK)
        self.img_in = os.open("fifo/img.in", os.O_RDWR | os.O_NONBLOCK)
        self.detections_out = os.open("fifo/detections.out", os.O_RDWR | os.O_NONBLOCK)
        self.detections_in = os.open("fifo/detections.in", os.O_RDWR | os.O_NONBLOCK)

    def __run__(self):
        for input, output in [
                (self.img_in, self.img_out),
                (self.detections_in, self.detections_out)]:
            try: buffer = os.read(output, 10000000)
            except OSError: return
            try: os.write(input, buffer)
            except OSError: print("Error")

with (pipeline := dai.Pipeline()):
    image = dai.node.ColorCamera().preview
    dai.node.OutNode(image, name="img")
    detections = dai.node.MobileNetDetectionNetwork(image,
                      blob_path = blobconverter.from_zoo(name='mobilenet-ssd', shaves=6))
    dai.node.OutNode(detections.out, name="detections")
    VisualizeDetectionsNode(
            dai.node.InNode(name="img"),
            dai.node.InNode(name="detections"),
            )
    Haulier()

dai.run(pipeline)


