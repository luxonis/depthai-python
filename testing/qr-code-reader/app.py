import logging
from typing import Tuple, List
from pyzbar.pyzbar import decode

import depthai as dai
import cv2
import numpy as np
import blobconverter


class QrCodeDetector:
    def __init__(
            self,
            color: Tuple[int, int, int] = (255, 0, 0),
            text_color: Tuple[int, int, int] = (255, 0, 0),
            font_scale: float = 0.5,
            thickness: int = 2,
    ):
        self.barcode_label = ""
        self.color: Tuple[int, int, int] = color
        self.text_color: Tuple[int, int, int] = text_color
        self.font_scale: float = font_scale
        self.thickness: int = thickness

    def annotate_frame(self, frame: np.ndarray, detections: List[Tuple[float, float, float, float]]):
        for detection in detections:
            bbox = self._convert_to_absolute_bbox(frame, detection)
            self._draw_bbox_around_detected_qr_code(frame, bbox)
            self._put_qr_label(frame, bbox)

    @staticmethod
    def _convert_to_absolute_bbox(frame: np.ndarray, bbox: Tuple[float, float, float, float]) -> np.ndarray:
        frame_height, frame_width = frame.shape[:2]
        xmin, ymin, xmax, ymax = bbox
        start_point_scale_coef = 0.9
        end_point_scale_coef = 1.1
        return np.array([max(0, xmin * frame_width) * start_point_scale_coef,
                         max(0, ymin * frame_height) * start_point_scale_coef,
                         min(frame_width, xmax * frame_width) * end_point_scale_coef,
                         min(frame_height, ymax * frame_height) * end_point_scale_coef],
                        dtype=int)

    def _draw_bbox_around_detected_qr_code(self, frame: np.ndarray, bbox: np.ndarray):
        cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color=self.color, thickness=self.thickness)

    def _put_qr_label(self, frame: np.ndarray, bbox: np.ndarray):
        cropped_qr = frame[bbox[1]:bbox[3], bbox[0]:bbox[2]]  # crop QR code
        cropped_qr = 255 - cropped_qr  # invert colors for revert black and white

        # Decode QR code
        barcodes = decode(cropped_qr)
        for barcode in barcodes:
            self.barcode_label = barcode.data.decode("utf-8")
            cv2.putText(
                frame,
                self.barcode_label,
                (bbox[0], bbox[1] - 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                self.font_scale,
                self.text_color,
                self.thickness,
            )


class QrDetectionNode(dai.Node):
    """
    Custom node for visualizing the results of qr code detection within the DepthAI pipeline.
    This node takes in image frames and detection data from a neural network, and uses
    the QrCodeDetector class to annotate the frames with bounding boxes around detected qr codes.
    The annotated frames are then displayed on the screen.

    """

    KEY_QUIT = ord("q")

    qr_detector = QrCodeDetector()

    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.ImgDetections):
        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        frame = image.getCvFrame()
        self.qr_detector.annotate_frame(
            frame=frame, detections=[(det.xmin, det.ymin, det.xmax, det.ymax) for det in detections.detections]
        )
        cv2.imshow("QR Detection", frame)


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    color_camera = dai.node.ColorCamera()
    color_camera.param_desc["preview_size"] = (tuple[int, int], (384, 384))
    nn_detections = dai.node.MobileNetDetectionNetwork(
        input=color_camera.preview,
        blob_path=blobconverter.from_zoo(name="qr_code_detection_384x384", zoo_type="depthai")
    )
    QrDetectionNode(image=color_camera.preview, detections=nn_detections)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline, compilation=dai.compilation.default_compilation[1:])
