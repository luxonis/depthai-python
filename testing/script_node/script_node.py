import cv2
import blobconverter
import numpy as np
import depthai as dai

class ControlKey:
    QUIT = ord("q")
    DRAW_DETECTIONS = ord("t")

class PipelineFlowChangerNode(dai.Node):
    draw_color = (255, 127, 0)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.toggle = False

    def __run__(self, image: dai.message.ImgFrame, detections: dai.message.ImgDetections) -> None:
        key = cv2.waitKey(1)
        if key == ControlKey.QUIT:raise KeyboardInterrupt()

        frame = image.getCvFrame()
        ctrl = dai.message.CameraControl()

        # Check for the key press to toggle the neural network detection
        if key == ControlKey.DRAW_DETECTIONS:
            self.toggle = not self.toggle

        # Draw detections only if the toggle is True
        if self.toggle:
            self.draw_detections(frame, detections.detections)

        # Display the frame
        cv2.putText(frame, f"NN inferencing: {self.toggle}", (20, 20), cv2.FONT_HERSHEY_TRIPLEX, 0.7, self.draw_color)
        cv2.imshow('Frame', frame)
        return ctrl

    @staticmethod
    def normalize_frame(frame, bbox) -> np.ndarray:
        normalized_values = np.full(len(bbox), frame.shape[0])
        normalized_values[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normalized_values).astype(int)

    def draw_detections(self, frame, detections):
        for detection in detections:
            bbox = self.normalize_frame(frame=frame,bbox=(detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 20),
                        cv2.FONT_HERSHEY_TRIPLEX, 0.5, self.draw_color)
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), self.draw_color, 2)

pipeline = dai.Pipeline()
with pipeline:
    feedback = dai.node.Feedback()
    camera_control = dai.node.ColorCamera(input_control=feedback, default_output="preview")
    nn_detections = dai.node.MobileNetDetectionNetwork(
        input=camera_control.preview,
        blob_path=blobconverter.from_zoo(name="mobilenet-ssd", shaves=6)
    )
    flow = PipelineFlowChangerNode(image=camera_control.preview, detections=nn_detections)
    feedback.attach(camera_control)
    feedback.attach(flow)
dai.run(pipeline)
