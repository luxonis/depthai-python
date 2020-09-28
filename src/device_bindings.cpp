#include "device_bindings.hpp"

//std
#include <iostream>

//depthai-core
#include "depthai/host_capture_command.hpp"

//depthai-shared
#include "depthai-shared/metadata/capture_metadata.hpp"

//project
#include "pybind11_common.hpp"
#include <boost/algorithm/string/replace.hpp>




// Binding for HostDataPacket
namespace py = pybind11;

void init_binding_device(pybind11::module& m){
      
    py::class_<Device>(m, "Device")
        .def(py::init<>())
        .def(py::init<std::string, bool>())
        .def(py::init<std::string, std::string>())
        .def(
            "create_pipeline",
            [](Device& device, py::dict config)
            {
                

                // str(dict) for string representation uses ['] , but JSON requires ["]
                // fast & dirty solution:
                std::string str = py::str(config);
                boost::replace_all(str, "\'", "\"");
                boost::replace_all(str, "None", "null");
                boost::replace_all(str, "True", "true");
                boost::replace_all(str, "False", "false");
                // TODO: make better json serialization

                return device.create_pipeline(str);
            },
            "Function for pipeline creation",
            py::arg("config") = py::dict()
        )
        .def(
            "get_available_streams",
            &Device::get_available_streams,
            "Returns available streams, that possible to retreive from the device."
        )
        .def(
            "request_jpeg",
            &Device::request_jpeg,
            "Function to request a still JPEG encoded image ('jpeg' stream must be enabled)"
        )
        .def(
            "request_af_trigger",
            &Device::request_af_trigger,
            "Function to request autofocus trigger"
        )
        .def(
            "request_af_mode",
            &Device::request_af_mode,
            "Function to request a certain autofocus mode (Check 'AutofocusMode.__members__')"
        )
        .def(
            "send_disparity_confidence_threshold",
            &Device::send_disparity_confidence_threshold,
            "Function to send disparity confidence threshold for SGBM"
        )

        .def(
            "send_camera_control",
            &Device::send_camera_control,
            "Function to send an ISP 3A camera control command",
            py::arg("camera_id"),
            py::arg("command_id"),
            py::arg("extra_args")
        )

        .def(
            "get_nn_to_depth_bbox_mapping",
            &Device::get_nn_to_depth_bbox_mapping,
            "Returns NN bounding-box to depth mapping as a dict of coords: off_x, off_y, max_w, max_h."
        )

        // calibration data bindings
        .def(
            "get_left_intrinsic",
            &Device::get_left_intrinsic,
            "Returns 3x3 matrix defining the intrinsic parameters of the left camera of the stereo setup."
        )

        .def(
            "get_left_homography",
            &Device::get_left_homography,
            "Returns 3x3 matrix defining the homography to rectify the left camera of the stereo setup."
        )

        .def(
            "get_right_intrinsic",
            &Device::get_right_intrinsic,
            "Returns 3x3 matrix defining the intrinsic parameters of the right camera of the stereo setup."
        )

        .def(
            "get_right_homography",
            &Device::get_right_homography,
            "Returns 3x3 matrix defining the homography to rectify the right camera of the stereo setup."
        )

        .def(
            "get_rotation",
            &Device::get_rotation,
            "Returns 3x3 matrix defining how much the right camera is rotated w.r.t left camera."
        )

        .def(
            "get_translation",
            &Device::get_translation,
            "Returns a vector defining how much the right camera is translated w.r.t left camera."
        )


        
        ;


    py::enum_<CaptureMetadata::AutofocusMode>(m, "AutofocusMode")
        .value("AF_MODE_AUTO", CaptureMetadata::AutofocusMode::AF_MODE_AUTO)
        .value("AF_MODE_MACRO", CaptureMetadata::AutofocusMode::AF_MODE_MACRO)
        .value("AF_MODE_CONTINUOUS_VIDEO", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_VIDEO)
        .value("AF_MODE_CONTINUOUS_PICTURE", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_PICTURE)
        .value("AF_MODE_EDOF", CaptureMetadata::AutofocusMode::AF_MODE_EDOF)
    ;

    py::enum_<Isp3A_CamId>(m, "Cam3A")
        .value("RGB",   Isp3A_CamId::CAM_RGB)
        .value("Left",  Isp3A_CamId::CAM_LEFT)
        .value("Right", Isp3A_CamId::CAM_RIGHT)
    ;

    py::enum_<Isp3A_Commands>(m, "Isp3A")
        .value("CMD_START_STREAM",             Isp3A_Commands::CMD_START_STREAM)
        .value("CMD_STOP_STREAM",              Isp3A_Commands::CMD_STOP_STREAM)
        .value("CMD_REQ_STILL",                Isp3A_Commands::CMD_REQ_STILL)
        .value("CMD_MOV_LENS",                 Isp3A_Commands::CMD_MOV_LENS)
        .value("CMD_FOCUS_TRIGGER",            Isp3A_Commands::CMD_FOCUS_TRIGGER)
        .value("CMD_AE_MANUAL",                Isp3A_Commands::CMD_AE_MANUAL)
        .value("CMD_AE_AUTO",                  Isp3A_Commands::CMD_AE_AUTO)
        .value("CMD_SET_AWB_MODE",             Isp3A_Commands::CMD_SET_AWB_MODE)
        .value("CMD_SCENE_MODES",              Isp3A_Commands::CMD_SCENE_MODES)
        .value("CMD_ANTIBANDING_MODES",        Isp3A_Commands::CMD_ANTIBANDING_MODES)
        .value("CMD_EXPOSURE_COMPENSATION",    Isp3A_Commands::CMD_EXPOSURE_COMPENSATION)
        .value("CMD_AE_LOCK",                  Isp3A_Commands::CMD_AE_LOCK)
        .value("CMD_AE_TARGET_FPS_RANGE",      Isp3A_Commands::CMD_AE_TARGET_FPS_RANGE)
        .value("CMD_AWB_LOCK",                 Isp3A_Commands::CMD_AWB_LOCK)
        .value("CMD_CAPTURE_INTENT",           Isp3A_Commands::CMD_CAPTURE_INTENT)
        .value("CMD_CONTROL_MODE",             Isp3A_Commands::CMD_CONTROL_MODE)
        .value("CMD_FRAME_DURATION",           Isp3A_Commands::CMD_FRAME_DURATION)
        .value("CMD_SENSITIVITY",              Isp3A_Commands::CMD_SENSITIVITY)
        .value("CMD_EFFECT_MODE",              Isp3A_Commands::CMD_EFFECT_MODE)
        .value("CMD_AF_MODE",                  Isp3A_Commands::CMD_AF_MODE)
        .value("CMD_NOISE_REDUCTION_STRENGTH", Isp3A_Commands::CMD_NOISE_REDUCTION_STRENGTH)
        .value("CMD_SATURATION",               Isp3A_Commands::CMD_SATURATION)
        .value("CMD_BRIGHTNESS",               Isp3A_Commands::CMD_BRIGHTNESS)
        .value("CMD_STREAM_FORMAT",            Isp3A_Commands::CMD_STREAM_FORMAT)
        .value("CMD_CAM_RESOLUTION",           Isp3A_Commands::CMD_CAM_RESOLUTION)
        .value("CMD_SHARPNESS",                Isp3A_Commands::CMD_SHARPNESS)
        .value("CMD_CUST_USECASE",             Isp3A_Commands::CMD_CUST_USECASE)
        .value("CMD_CUST_CAPT_MODE",           Isp3A_Commands::CMD_CUST_CAPT_MODE)
        .value("CMD_CUST_EXP_BRACKETS",        Isp3A_Commands::CMD_CUST_EXP_BRACKETS)
        .value("CMD_CUST_CAPTURE",             Isp3A_Commands::CMD_CUST_CAPTURE)
        .value("CMD_CONTRAST",                 Isp3A_Commands::CMD_CONTRAST)
        .value("CMD_AE_REGION",                Isp3A_Commands::CMD_AE_REGION)
        .value("CMD_AF_REGION",                Isp3A_Commands::CMD_AF_REGION)
        .value("CMD_LUMA_DENOISE",             Isp3A_Commands::CMD_LUMA_DENOISE)
        .value("CMD_CHROMA_DENOISE",           Isp3A_Commands::CMD_CHROMA_DENOISE)
    ;

}

