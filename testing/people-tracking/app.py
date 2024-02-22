import logging
from collections import deque
from typing import Tuple, List
import sys
from pathlib import Path
from time import monotonic

import depthai_bind
from depthai import misc

import depthai as dai
import cv2
import numpy as np
import blobconverter


class PeopleTrackerDrawer:

    def draw_tracked_objects(self, frame: np.ndarray, tracklets: dai.messages.Tracklets):
        self._track_object_path(frame, tracklets)
        self._draw_tracked_objects(frame)

    @staticmethod
    def _get_object_relative_position(frame: np.ndarray, tracklet: depthai_bind.Tracklet) -> np.array:
        roi = tracklet.roi.denormalize(frame.shape[1], frame.shape[0])
        return np.array([roi.topLeft().x, roi.topLeft().y, roi.bottomRight().x, roi.bottomRight().y], dtype=int)

    @staticmethod
    def _get_middle_of_bbox(bbox: np.ndarray) -> np.ndarray:
        return np.array([(bbox[0] + bbox[2]) / 2, (bbox[1] + bbox[3]) / 2], dtype=int)

    def _track_object_path(self, frame: np.ndarray, objects: dai.messages.Tracklets):

        new_tracked_ids = set()
        for object in objects.tracklets:
            current_id = object.id
            new_tracked_ids.add(current_id)

            if current_id not in self.tracked_objects_path:
                self.tracked_objects_path[current_id] = deque(maxlen=self.tracked_objects_path_length)

            self.tracked_objects_path[current_id].append(self._get_object_relative_position(frame, object))

        features_to_remove = set()
        for oldId in self.tracked_ids:
            if oldId not in new_tracked_ids:
                features_to_remove.add(oldId)

        for id in features_to_remove:
            self.tracked_objects_path.pop(id)

        self.tracked_ids = new_tracked_ids

    def _draw_tracked_objects(self, frame: np.ndarray):
        for tracked_object_path in self.tracked_objects_path.values():
            for j in range(len(tracked_object_path) - 1):
                self._draw_tracking_line(frame, tracked_object_path[j], tracked_object_path[j + 1])
            j = len(tracked_object_path) - 1
            self._draw_dot(frame, tracked_object_path[j])
            self._draw_filled_bbox(frame, tracked_object_path[j])

    def _draw_tracking_line(self, frame: np.ndarray, src_bbox: np.ndarray, dst_bbox: np.ndarray) -> None:
        cv2.line(frame, self._get_middle_of_bbox(src_bbox), self._get_middle_of_bbox(dst_bbox), color=(0, 0, 255),
                 thickness=self.thickness, shift=0)

    def _draw_dot(self, frame: np.ndarray, bbox: np.ndarray) -> None:
        cv2.circle(frame, self._get_middle_of_bbox(bbox), radius=1, color=(0, 255, 0), thickness=self.thickness)

    def _draw_filled_bbox(self, frame: np.ndarray, bbox: np.ndarray) -> None:
        alpha = 0.3
        overlay = frame.copy()
        overlay = cv2.rectangle(overlay, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color=self.color, thickness=-1)

        cv2.addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame)
        cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color=self.color, thickness=self.thickness)

    def __init__(
            self,
            color: Tuple[int, int, int] = (255, 0, 0),
            thickness: int = 1,
            tracked_objects_path_length: int = 10
    ):
        self.color = color
        self.thickness = thickness
        self.tracked_ids = set()
        self.tracked_objects_path = dict()
        self.tracked_objects_path_length = tracked_objects_path_length


class PeopleTrackingNode(dai.Node):
    """
    Custom node for visualizing the results of people detection within the DepthAI pipeline.
    This node takes in image frames and detection data from a neural network, and uses
    the PeopleTracker class to annotate the frames with bouding boxes on detected people.
    The annotated frames are then displayed on the screen.

    """

    KEY_QUIT = ord("q")

    people_tracker_drawer = PeopleTrackerDrawer()

    def __run__(self, image: dai.messages.ImgFrame, tracklets: dai.messages.Tracklets):
        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        frame = image.getCvFrame()
        self.people_tracker_drawer.draw_tracked_objects(frame, tracklets)
        cv2.imshow("People Tracker", frame)


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    color_camera = dai.node.ColorCamera(preview_size=(544, 320))
    nn_detections = dai.node.MobileNetDetectionNetwork(
        input=color_camera.preview,
        blob_path=blobconverter.from_zoo(name="person-detection-retail-0013", shaves=6)
    )
    object_tracker = dai.node.ObjectTracker(
        input_detection_frame=nn_detections.passthrough,
        input_tracker_frame=nn_detections.passthrough,
        input_detections=nn_detections.out,
        detection_labels_to_track=[1],
        max_objects_to_track=20,
        tracker_id_assignment_policy=dai.TrackerIdAssignmentPolicy.SMALLEST_ID,
        tracker_threshold=0.5,
        tracker_type=dai.TrackerType.SHORT_TERM_IMAGELESS
    )
    PeopleTrackingNode(image=color_camera.preview, tracklets=object_tracker.out)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline, compilation=dai.compilation.default_compilation[1:])
