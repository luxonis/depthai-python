from depthai.core_node import CoreNode
from typing import Any, TypeVar
from depthai.messages import *
import depthai.misc as misc

# TODO Set defaults

T = TypeVar("T")

common_camera_param_desc = {
        "board_socket": misc.CameraBoardSocket,
        "camera": str,
        "fps": float,
        "image_orientation": misc.CameraImageOrientation,
        "isp_3a_fps": int,
        "raw_output_packed": bool,
        }

mono_color_camera_param_desc = common_camera_param_desc | {
        "cam_id": int,
        "frame_event_filter": list[misc.FrameEvent],
        "raw_num_frames_pool": int,
        }

class Camera(CoreNode):
    input_desc = {
        "input_config": ImageManipConfig,
        "input_control": CameraControl,
        }
    output_desc = {
        "raw": ImgFrame,
        "isp": ImgFrame,
        "still": ImgFrame,
        "preview": ImgFrame,
        "video": ImgFrame,
        }
    param_desc = common_camera_param_desc | {
        "calibration_alpha": float,
        "mesh_source": misc.CameraProperties.WarpMeshSource,
        "mesh_step": tuple[int, int],
        "preview_size": (tuple[int, int], (300, 300)),
        "size": tuple[int, int],
        "still_size": tuple[int, int],
        "video_size": tuple[int, int],
        }


class ColorCamera(CoreNode):
    input_desc = {
        "input_config": ImageManipConfig,
        "input_control": CameraControl,
        }
    output_desc = {
        "raw": ImgFrame,
        "isp": ImgFrame,
        "still": ImgFrame,
        "preview": ImgFrame,
        "video": ImgFrame,
        }
    param_desc = mono_color_camera_param_desc | {
        "color_order": misc.ColorCameraProperties.ColorOrder,
        "fp16": bool,
        "interleaved": (bool, False),
        "isp_num_frames_pool": int,
        "isp_scale": tuple[int, int] | tuple[tuple[int, int], tuple[int, int]],
        "num_frames_pool": tuple[int, int, int, int, int],
        "preview_keep_aspect_ratio": bool,
        "preview_num_frames_pool": int,
        "preview_size": (tuple[int, int], (300, 300)),
        "resolution": misc.ColorCameraProperties.SensorResolution,
        "sensor_crop": tuple[int, int],
        "still_num_frames_pool": int,
        "still_size": tuple[int, int],
        "video_num_frames_pool": int,
        "video_size": tuple[int, int],
        "wait_for_config_input": bool,
        }


class MonoCamera(CoreNode):
    default_output = "out"
    input_desc = {
        "input_control": CameraControl,
        }
    output_desc = {
        "raw": ImgFrame,
        "out": ImgFrame,
        }
    param_desc = mono_color_camera_param_desc | {
        "num_frames_pool": int,
        "resolution": misc.MonoCameraProperties.SensorResolution,
        }

class EdgeDetector(CoreNode):
    input_desc = {
        "input_image": ImgFrame,
        "input_config": EdgeDetectorConfig,
        }
    output_desc = {
        "output_image": ImgFrame,
        }
    param_desc = {
        "max_output_frame_size": int,
        "num_frames_pool": int,
        "wait_for_config_input": bool,
        }

class FeatureTracker(CoreNode):
    default_output = "output_features"
    input_desc = {
        "input_image": ImgFrame,
        "input_config": FeatureTrackerConfig,
        }
    output_desc = {
        "output_features": TrackedFeatures,
        "passthrough_input_image": ImgFrame,
        }
    param_desc = {
        "hardware_resources": tuple[int, int],
        "wait_for_config_input": bool,
        }

class ImageManip(CoreNode):
    input_desc = {
        "input_image": ImgFrame,
        "input_config": ImageManipConfig,
        }
    output_desc = {
        "out": ImgFrame,
        }
    param_desc = {
        "center_crop": tuple[float, float],
        "crop_rect": tuple[float, float, float, float],
        "frame_type": misc.RawImgFrame.Type,
        "horizontal_flip": bool,
        "keep_aspect_ratio": bool,
        "max_output_frame_size": int,
        "num_frames_pool": int,
        "resize": tuple[int, int],
        "resize_thumbnail": tuple[int, int, int, int, int],
        "wait_for_config_input": bool,
        "warp_mesh": tuple[list[tuple[float, float] | misc.Point2f], int, int],
        }

class IMU(CoreNode):
    input_desc = {}
    output_desc = {
        "out": IMUData,
        }
    param_desc = {
        "batch_report_threshold": int,
        "max_batch_reports": int,
        }

class NeuralNetwork(CoreNode):
    default_output = "out"
    input_desc = {
        "input": T,
        }
    output_desc = {
        "out": NNData,
        "passthrough": T, #Doesn't match doc -- suspected error in doc
        }
    param_desc = {
        "blob": misc.OpenVINO.Blob,
        "blob_path": Any, # TODO FIXME
        "num_inference_threads": int,
        "num_NCE_per_inference_thread": int,
        "num_pool_frames": int,
        }


class MobileNetDetectionNetwork(CoreNode):
    default_output = "out"
    input_desc = {
        "input": ImgFrame,
        }
    output_desc = {
        "out": ImgDetections,
        "passthrough": ImgFrame,
        }
    param_desc = NeuralNetwork.param_desc | {
        "confidence_threshold": (float, 0.5),
        }

spatial_network_params = {
        "bounding_box_scale_factor": float,
        "depth_lower_threshold": int,
        "depth_upper_threshold": int,
        "spatial_calculation_algorithm": 
                misc.SpatialLocationCalculatorAlgorithm,
        "spatial_calculation_step_size": int,
        }

class MobileNetSpatialDetectionNetwork(CoreNode):
    input_desc = {
        "input": ImgFrame,
        "input_depth": ImgFrame,
        }
    output_desc = {
        "out": SpatialImgDetections,
        "bounding_box_mapping": SpatialLocationCalculatorData, #Doesn't match doc -- suspected error in doc
        "passthrough": ImgFrame,
        "passthrough_depth": ImgFrame,
        }
    param_desc = MobileNetDetectionNetwork.param_desc | spatial_network_params

class YoloDetectionNetwork(CoreNode):
    default_output = "out"
    input_desc = {
        "input": ImgFrame,
        }
    output_desc = {
        "out": ImgDetections,
        "passthrough": ImgFrame,
        }
    param_desc = NeuralNetwork.param_desc | {
        "anchor_masks": dict[str, list[int]],
        "anchors": list[float],
        "confidence_threshold": (float, 0.5),
        "coordinate_size": int,
        "iou_threshold": float,
        "num_classes": int,
        }

class YoloSpatialDetectionNetwork(CoreNode):
    default_output="out"
    input_desc = {
        "input": ImgFrame,
        "input_depth": ImgFrame,
        }
    output_desc = {
        "out": SpatialImgDetections,
        "bounding_box_mapping": SpatialLocationCalculatorData, #Doesn't match doc -- suspected error in doc
        "passthrough": ImgFrame,
        "passthrough_depth": ImgFrame,
        }
    param_desc = YoloDetectionNetwork.param_desc | spatial_network_params

class ObjectTracker(CoreNode):
    default_output = "out"
    input_desc = {
        "input_detection_frame": ImgFrame,
        "input_tracker_frame": ImgFrame,
        "input_detections": ImgDetections,
        }
    output_desc = {
        "out": Tracklets,
        "passthrough_detection_frame": ImgFrame,
        "passthrough_tracker_frame": ImgFrame,
        "passthrough_detections": ImgDetections,
        }
    param_desc = {
        "detection_labels_to_track": list[int],
        "max_objects_to_track": int,
        "tracker_id_assignment_policy": misc.TrackerIdAssignmentPolicy,
        "tracker_threshold": float,
        "tracker_type": misc.TrackerType,
        "tracking_per_class": bool,
        }

class SpatialLocationCalculator(CoreNode):
    default_output = "out"
    input_desc = {
        "input_config": SpatialLocationCalculatorConfig,
        "input_depth": ImgFrame,
        }
    output_desc = {
        "out": SpatialLocationCalculatorData,
        "passthrough_depth": ImgFrame,
        }
    param_desc = {
        "wait_for_config_input": bool
        }

class SPIOut(CoreNode):
    input_desc = {
        "input": Buffer,
        }
    output_desc = {}
    param_desc = {
        "bus_id": int,
        "stream_name": str
        }

class SPIIn(CoreNode):
    input_desc = {}
    output_desc = {
        "out": Buffer
        }
    param_desc = SPIOut.param_desc | {
        "max_data_size": int,
        "num_frames": int,
        }

class StereoDepth(CoreNode):
    input_desc = {
        "left": ImgFrame,
        "right": ImgFrame,
        "input_config": StereoDepthConfig,
        }
    output_desc = {
        "confidence_map": ImgFrame,
        "rectified_left": ImgFrame,
        "synced_left": ImgFrame,
        "depth": ImgFrame,
        "disparity": ImgFrame,
        "rectified_right": ImgFrame,
        "synced_right": ImgFrame,
        "out_config": StereoDepthConfig,
        }
    param_desc = {
        "alpha_scaling": float,
        "baseline": float,
        "confidence_threshold": int,
        "default_profile_preset": misc.PresetMode,
        "depth_align": misc.RawStereoDepthConfig.AlgorithmControl.DepthAlign
                     | misc.CameraBoardSocket, 
        "depth_alignment_use_spec_translation": bool,
        "disparity_to_depth_use_spec_translation": bool,
        "empty_calibration": tuple[()], # Empty tuple
        "extended_disparity": bool,
        "focal_length": float,
        "focal_length_from_calibration": bool,
        "input_resolution": tuple[int, int],
        "left_right_check": bool,
        "median_filter": misc.MedianFilter,
        "mesh_step": tuple[int, int],
        "num_frames_pool": int,
        "output_depth": bool,
        "output_keep_aspect_ratio": bool,
        "output_rectified": bool,
        "output_size": tuple[int, int],
        "post_processing_hardware_resources": tuple[int, int],
        "rectification": bool,
        "rectification_use_spec_translation": bool,
        "rectify_edge_fill_color": int,
        "rectify_mirror_frame": bool,
        "runtime_mode_switch": bool,
        "subpixel": bool,
        "subpixel_fractional_bits": int,
        }

class SystemLogger(CoreNode):
    input_desc = {}
    output_desc = {
        "out": SystemInformation,
        }
    param_desc = {
        "rate": float
        }

class ToF(CoreNode):
    input_desc = {
        "input_config": ToFConfig,
        "input": ImgFrame,
        }
    output_desc = {
        "depth": ImgFrame,
        "amplitude": ImgFrame,
        "error": ImgFrame,
        }
    param_desc = {}


class UVC(CoreNode):
    input_desc = {
        "input": ImgFrame
        }
    ouput_desc = {}
    param_desc = {
        "gpios_on_init": dict[int, int],
        "gpios_on_stream_off": dict[int, int],
        "gpios_on_stream_on": dict[int, int],
        }

class VideoEncoder(CoreNode):
    input_desc = {
        "input": ImgFrame
        }
    output_desc = {
        "bitstream": ImgFrame
        }
    param_desc = {
        "bitrate": int,
        "bitrate_kbps": int,
        "default_profile_preset": 
                tuple[float, misc.VideoEncoderProperties.Profile]
                | tuple[int, int, float, misc.VideoEncoderProperties.Profile]
                | tuple[tuple[int, int], 
                        float, 
                        misc.VideoEncoderProperties.Profile],
        "frame_rate": float,
        "keyframe_frequency": int,
        "lossless": bool,
        "max_output_frame_size": int,
        "num_b_frames": int,
        "num_frames_pool": int,
        "profile": misc.VideoEncoderProperties.Profile,
        "quality": int,
        "rate_control_mode": misc.VideoEncoderProperties.RateControlMode,
        }

class Warp(CoreNode): 
    input_desc = {
        "input_image": ImgFrame
        }
    output_desc = {
        "out": ImgFrame
        }
    param_desc = {
        "hw_ids": list[int],
        "interpolation": misc.Interpolation,
        "max_output_frame_size": int,
        "num_frames_pool": int,
        "output_size": tuple[int, int],
        "warp_mesh": tuple[list[tuple[float, float] | misc.Point2f], int, int],
        }


class XLinkOut(CoreNode):
    input_desc = {
        "input": Buffer,
        }
    output_desc = {}
    param_desc = {
        "stream_name": str,
        "fps_limit": float,
        "metadata_only": bool,
        }
XLinkDevOut = XLinkOut

class XLinkIn(CoreNode):
    input_desc = {}
    output_desc = {
        "out": Buffer,
        }
    param_desc = {
        "stream_name": str,
        "max_data_size": int,
        "num_frames": int,
        }
XLinkDevIn = XLinkIn

