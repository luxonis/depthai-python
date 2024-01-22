import cv2
import logging
import depthai as dai


class ControlKey:
    QUIT = ord("q")
    EXPOSURE_DOWN = ord("i")
    EXPOSURE_UP = ord("o")
    SENSITIVITY_DOWN = ord("k")
    SENSITIVITY_UP = ord("l")


class CameraSettingsManager:
    def __init__(self):
        self.exp_time = 20000  # Exposure time in microseconds
        self.sens_iso = 800  # Sensitivity in ISO

        self.exp_time_step = 500
        self.sens_iso_step = 50

        self.exp_time_min = 1
        self.exp_time_max = 33000

        self.sens_iso_min = 100
        self.sens_iso_max = 1600

    def adjust_settings(self, key: int):
        if key == ControlKey.EXPOSURE_DOWN:
            self.exp_time -= self.exp_time_step
        elif key == ControlKey.EXPOSURE_UP:
            self.exp_time += self.exp_time_step
        elif key == ControlKey.SENSITIVITY_DOWN:
            self.sens_iso -= self.sens_iso_step
        elif key == ControlKey.SENSITIVITY_UP:
            self.sens_iso += self.sens_iso_step

        self.exp_time = self._clamp(self.exp_time, self.exp_time_min, self.exp_time_max)
        self.sens_iso = self._clamp(self.sens_iso, self.sens_iso_min, self.sens_iso_max)
        logger.info("Exposure time: %d, Sensitivity: %d", self.exp_time, self.sens_iso)

    @staticmethod
    def _clamp(num: int, v0: int, v1: int) -> int:
        return max(v0, min(num, v1))


class CameraParametersControlNode(dai.Node):
    """
    A custom DepthAI node for real-time control of camera exposure and sensitivity.

    This node captures frames from the camera and allows dynamic adjustment of camera settings
    (exposure time and ISO sensitivity) based on keyboard inputs.
    The adjusted settings are then applied to the camera, influencing the captured image's exposure and brightness.
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.camera_settings_manager = CameraSettingsManager()

    def __run__(self, image: dai.message.ImgFrame) -> dai.message.CameraControl:
        frame = image.getCvFrame()
        cv2.imshow("Camera Control", frame)

        key = cv2.waitKey(1)

        if key == ControlKey.QUIT:
            raise KeyboardInterrupt()

        ctrl = dai.message.CameraControl()
        if key in [
            ControlKey.EXPOSURE_UP,
            ControlKey.EXPOSURE_DOWN,
            ControlKey.SENSITIVITY_DOWN,
            ControlKey.SENSITIVITY_UP,
        ]:
            self.camera_settings_manager.adjust_settings(key=key)
            ctrl.setManualExposure(
                exposureTimeUs=self.camera_settings_manager.exp_time,
                sensitivityIso=self.camera_settings_manager.sens_iso,
            )

        return ctrl


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    feedback = dai.node.Feedback()
    camera_control = CameraParametersControlNode(image=dai.node.ColorCamera(input_control=feedback).video)
    feedback.attach(camera_control)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline)
