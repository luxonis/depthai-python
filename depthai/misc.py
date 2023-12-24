from depthai_bind import (
        CameraBoardSocket,
        CameraImageOrientation,
        CameraProperties,
        ColorCameraProperties,
        FrameEvent,
        MonoCameraProperties,
        RawImgFrame,
        Point2f,
        OpenVINO,
        SpatialLocationCalculatorAlgorithm,
        TrackerIdAssignmentPolicy,
        TrackerType,
        RawStereoDepthConfig,
        CameraBoardSocket,
        MedianFilter,
        Interpolation,
        VideoEncoderProperties,
        )

# Irregularity which comes from core
from depthai_bind.node import StereoDepth as _StereoDepth
PresetMode = _StereoDepth.PresetMode
