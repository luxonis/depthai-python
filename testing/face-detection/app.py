import logging
from typing import Tuple, List

import depthai as dai
import cv2
import numpy as np
import blobconverter


class FaceDetector:
    def __init__(
        self,
        color: Tuple[int, int, int] = (255, 0, 0),
        text_color: Tuple[int, int, int] = (255, 0, 0),
        font_scale: float = 0.5,
        thickness: int = 2,
    ):
        self.label = "Face"
        self.color: Tuple[int, int, int] = color
        self.text_color: Tuple[int, int, int] = text_color
        self.font_scale: float = font_scale
        self.thickness: int = thickness

    def annotate_frame(self, frame: np.ndarray, detections: List[Tuple[float, float, float, float]]):
        for detection in detections:
            bbox = self._convert_to_absolute_bbox(frame, detection)
            self._draw_bbox_around_detected_face(frame, bbox)
            self._put_face_label(frame, bbox)

    @staticmethod
    def _convert_to_absolute_bbox(frame: np.ndarray, bbox: Tuple[float, float, float, float]) -> np.ndarray:
        frame_height, frame_width = frame.shape[:2]
        xmin, ymin, xmax, ymax = bbox
        return np.array([xmin * frame_width, ymin * frame_height, xmax * frame_width, ymax * frame_height], dtype=int)

    def _draw_bbox_around_detected_face(self, frame: np.ndarray, bbox: np.ndarray):
        cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), self.color, self.thickness)

    def _put_face_label(self, frame: np.ndarray, bbox: np.ndarray):
        cv2.putText(
            frame,
            self.label,
            (bbox[0], bbox[1] - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            self.font_scale,
            self.text_color,
            self.thickness,
        )


class FaceDetectionNode(dai.Node):
    """
    Custom node for visualizing the results of face detection within the DepthAI pipeline.
    This node takes in image frames and detection data from a neural network, and uses
    the FaceDetector class to annotate the frames with bounding boxes around detected faces.
    The annotated frames are then displayed on the screen.

    # TODO: Add visualization enhancements
    """

    KEY_QUIT = ord("q")

    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.ImgDetections):
        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        frame = image.getCvFrame()
        face_detector = FaceDetector()
        face_detector.annotate_frame(
            frame=frame, detections=[(det.xmin, det.ymin, det.xmax, det.ymax) for det in detections.detections]
        )
        cv2.imshow("Face Detection", frame)


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    color_camera = dai.node.ColorCamera()
    nn_detections = dai.node.MobileNetDetectionNetwork(
        input=color_camera.preview, blob_path=blobconverter.from_zoo(name="face-detection-retail-0004", shaves=6)
    )
    FaceDetectionNode(image=color_camera.preview, detections=nn_detections)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline)
