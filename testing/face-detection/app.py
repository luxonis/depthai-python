import logging
from typing import Tuple, List

import depthai as dai
import cv2
import numpy as np
import blobconverter


class FaceDetector:
    """
    The FaceDetector class is designed for detecting and annotating faces in image frames using OpenCV.
    It provides functionality to draw bounding boxes and labels around detected faces,
    with customizable colors, font scales, and thicknesses for annotations.

    Attributes:
        color (Tuple[int, int, int]): The color of the bounding box.
        text_color (Tuple[int, int, int]): The color of the label text.
        font_scale (float): The font scale of the label text.
        thickness (int): The thickness of the bounding box and label text.

    Methods:
        annotate_frame(frame, detections):
            Annotates a frame with bounding boxes and labels around detected faces.
        _convert_to_absolute_bbox(frame, bbox) -> np.ndarray:
            Converts relative bounding box coordinates to absolute pixel values.
        _draw_bbox(frame, bbox):
            Draws a bounding box around a detected face.
        _put_label(frame, bbox):
            Draws a label above a detected face.
    """

    def __init__(
        self,
        color: Tuple[int, int, int] = (255, 0, 0),
        text_color: Tuple[int, int, int] = (255, 0, 0),
        font_scale: float = 0.5,
        thickness: int = 2,
    ):
        self.color: Tuple[int, int, int] = color
        self.text_color: Tuple[int, int, int] = text_color
        self.font_scale: float = font_scale
        self.thickness: int = thickness

    def annotate_frame(self, frame: np.ndarray, detections: List[Tuple[float, float, float, float]]):
        """
        Draws rectangles and labels around detected faces on the image.
        :param frame: The image on which to highlight faces.
        :param detections: A list of detected face coordinates in the format (xmin, ymin, xmax, ymax).
        :return: None
        """
        for detection in detections:
            bbox = self._convert_to_absolute_bbox(frame, detection)
            self._draw_bbox(frame, bbox)
            self._put_label(frame, bbox)

    @staticmethod
    def _convert_to_absolute_bbox(frame: np.ndarray, bbox: Tuple[float, float, float, float]) -> np.ndarray:
        """
        Converts face bounding box coordinates from relative to absolute pixel values.
        'bbox' is a tuple representing the coordinates of the face (xmin, ymin, xmax, ymax),
        where 'xmin' and 'ymin' are the coordinates of the top-left corner, and 'xmax' and 'ymax'
        are the coordinates of the bottom-right corner.
        :param frame: The image to determine the dimensions for the bounding box.
        :param bbox: Relative coordinates of the face bounding box.
        :return: Absolute pixel coordinates of the bounding box.
        """
        frame_height, frame_width = frame.shape[:2]
        xmin, ymin, xmax, ymax = bbox
        return np.array([xmin * frame_width, ymin * frame_height, xmax * frame_width, ymax * frame_height], dtype=int)

    def _draw_bbox(self, frame: np.ndarray, bbox: np.ndarray):
        """
        Draws a bounding box around a detected face.
        :param frame: The image frame to annotate.
        :param bbox: The bounding box coordinates to convert.
        """
        cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), self.color, self.thickness)

    def _put_label(self, frame: np.ndarray, bbox: np.ndarray):
        """
        Draws a label above a detected face.
        :param frame: The image frame to annotate.
        :param bbox: The bounding box coordinates to convert.
        """
        cv2.putText(
            frame,
            "Face",
            (bbox[0], bbox[1] - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            self.font_scale,
            self.text_color,
            self.thickness,
        )


class FaceDetectionNode(dai.Node):
    """
    This class integrates with a DepthAI pipeline to process live camera feeds and detect faces.
    Detected faces are highlighted with rectangles in the output display. The display loop
    can be terminated by pressing the 'q' key.

    Attributes:
        KEY_QUIT (int): ASCII code for the 'quit' key.

    Methods:
        __run__(self, image, detections):
            Annotates and displays the camera feed with face detections and labels using special FaceDetector class.

    TODO: Add visualization enhancements
    """

    KEY_QUIT = ord("q")

    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.ImgDetections):
        """
        Processes an image frame and detections from a neural network to perform face detection.
        Draws bounding boxes around detected faces.

        Args:
            image (dai.message.ImgFrame): The image frame to process with type np.ndarray
            detections (dai.message.ImgDetections): The detections from the neural network
        """

        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        frame = image.getCvFrame()
        face_detector = FaceDetector()
        face_detector.annotate_frame(
            frame=frame, detections=[(det.xmin, det.ymin, det.xmax, det.ymax) for det in detections.detections]
        )
        cv2.imshow("Face Detection", frame)


# Initialize logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Create pipeline with camera and neural network
pipeline = dai.Pipeline()

with pipeline:
    preview = dai.node.ColorCamera().preview
    nn_detections = dai.node.MobileNetDetectionNetwork(
        preview, blob_path=blobconverter.from_zoo(name="face-detection-retail-0004", shaves=6)
    )
    FaceDetectionNode(image=preview, detections=nn_detections)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline)
