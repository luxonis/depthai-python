import logging
import depthai as dai
import cv2
import numpy as np


class DepthVisualizer:
    WLSLevel = {"LOW": (1000, 0.8), "MEDIUM": (6000, 1.5), "HIGH": (12000, 2.0)}

    def __init__(self, max_disparity: int):
        self.max_disparity = max_disparity
        self._config_wls()

    def _config_wls(self, wls_level: str = "HIGH", wls_lambda: float = None, wls_sigma: float = None) -> None:
        default_lambda, default_sigma = self.WLSLevel.get(wls_level.upper(), self.WLSLevel["HIGH"])
        self.wls_config = {
            "enabled": True,
            "lambda": wls_lambda or default_lambda,
            "sigma": wls_sigma or default_sigma,
        }

    def _apply_wls_filter(self, left_disp: np.ndarray):
        # TODO: Implement WLS filter for disparity map with visualization - ask @PetrNovota or @Zimamazim
        pass

    def _apply_color_map(self, disparity_frame: np.ndarray) -> np.ndarray:
        depth_frame = (disparity_frame * 255.0 / self.max_disparity).astype(np.uint8)
        return cv2.applyColorMap(depth_frame, cv2.COLORMAP_JET)

    def visualize(self, disparity_frame: np.ndarray) -> np.ndarray:
        return self._apply_color_map(disparity_frame)


class StereoPreviewNode(dai.Node):
    """
    The custom node which processes the disparity map and create a visual representation of depth.
    TODO: Implement visualization
    """

    FRAME_NAME = "disparity_color"
    KEY_QUIT = ord("q")

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.depth_visualizer = None

    def __run__(self, disparity: dai.message.ImgFrame, config: dai.message.StereoDepthConfig):
        if cv2.waitKey(1) == self.KEY_QUIT:
            raise KeyboardInterrupt()

        if not self.depth_visualizer:
            self.depth_visualizer = DepthVisualizer(max_disparity=config.getMaxDisparity())

        disparity_frame = disparity.getCvFrame()
        depth_colored = self.depth_visualizer.visualize(disparity_frame)
        cv2.imshow(self.FRAME_NAME, depth_colored)


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

pipeline = dai.Pipeline()

with pipeline:
    stereo = dai.node.StereoDepth(
        left=dai.node.MonoCamera(resolution=dai.MonoCameraProperties.SensorResolution.THE_400_P, fps=30).out,
        right=dai.node.MonoCamera(resolution=dai.MonoCameraProperties.SensorResolution.THE_400_P, fps=30).out,
        default_profile_preset=dai.misc.PresetMode.HIGH_DENSITY,
        median_filter=dai.misc.MedianFilter.KERNEL_7x7,
        left_right_check=True,
        extended_disparity=False,
        subpixel=False,
    )
    StereoPreviewNode(disparity=stereo.disparity, config=stereo.outConfig)

logger.info("Pipeline created successfully %s", pipeline)

dai.run(pipeline)
