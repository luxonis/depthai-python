import logging
from typing import Tuple, List

import depthai as dai
import cv2
import numpy as np
import blobconverter
from handler import decode, ImgLandmarks, ALL_COLORS


class PoseEstimator:
    def __init__(
            self,
            thickness: int = 2,
    ):
        self.label = "Pose"
        self.thickness: int = thickness

    def annotate_frame(self, frame: np.ndarray, detections: ImgLandmarks) -> None:
        if detections.landmarks:
            pose_points_pairs = self._convert_to_absolute_points(frame, detections.landmarks[0])
            self._draw_detected_pose(frame, pose_points_pairs, detections.landmarks_indices[0], detections.pairs)

    @staticmethod
    def _convert_to_absolute_points(frame: np.ndarray,
                                    landmarks: List[Tuple[Tuple[float, float], Tuple[float, float]]]) -> np.ndarray:
        frame_h, frame_w = frame.shape[:2]
        return np.array(
            [(
             (landmark[0][0] * frame_w, landmark[0][1] * frame_h), (landmark[1][0] * frame_w, landmark[1][1] * frame_h))
             for landmark in landmarks],
            dtype=int
        )

    def _draw_detected_pose(self, frame: np.ndarray, pose_points_pairs: np.ndarray, indices: List[int],
                            pairs: List[Tuple[int, int]]) -> None:
        keypoints = set()
        for i in range(len(pose_points_pairs)):
            cv2.line(frame, pose_points_pairs[i][0], pose_points_pairs[i][1], color=ALL_COLORS[indices[i]],
                     thickness=self.thickness)
            for j in range(2):
                if pairs[indices[i]][j] not in keypoints:
                    cv2.circle(frame, (pose_points_pairs[i][j]), radius=1, color=ALL_COLORS[indices[i]],
                               thickness=self.thickness)
                    keypoints.add(pairs[indices[i]][j])


class HumanPoseEstimationNode(dai.Node):
    """
    Custom node for visualizing the results of human pose detection within the DepthAI pipeline.
    This node takes in image frames and detection data from a neural network, and uses
    the PoseEstimator class to annotate the frames with lines on detected limbs.
    The annotated frames are then displayed on the screen.

    """

    KEY_QUIT = ord("q")

    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.NNData):
        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        frame = image.getCvFrame()
        pose_estimator = PoseEstimator()
        pose_estimator.annotate_frame(
            frame=frame, detections=decode(detections)
        )
        cv2.imshow("Pose Estimation", frame)


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    color_camera = dai.node.ColorCamera()
    color_camera.param_desc["preview_size"] = (tuple[int, int], (456, 256))
    nn_data = dai.node.NeuralNetwork(
        input=color_camera.preview,
        blob_path=blobconverter.from_zoo(name="human-pose-estimation-0001", shaves=6)
    )
    HumanPoseEstimationNode(image=color_camera.preview, detections=nn_data)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline, compilation=dai.compilation.default_compilation[1:])
