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
            "Function to send an ISP 3A camera control command. "
            "args: 'Cam3A.__members__', 'Isp3A.__members__', extra_args: 'string, space separated values https://git.io/JUpnJ '",
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

        .def(
            "is_usb3",
            &Device::is_usb3,
            "Return true if connected over usb3 or else false."
        )

        .def(
            "get_mx_id",
            &Device::get_mx_id,
            "Return the Myraid X serial number of the device."
        )
        
        .def(
            "is_eeprom_loaded",
            &Device::is_eeprom_loaded,
            "Return true if EEPROM has both intrinsic matrixes."
        )

        .def(
            "is_device_changed",
            &Device::is_device_changed,
            "Return true if device is swapped while running over watchdog thread."
        )
        
        .def(
            "reset_device_changed",
            &Device::reset_device_changed,
            "Sets device_changed var to false to detect the next swap while running over watchdog thread."
        )
        
        .def(
            "is_rgb_connected",
            &Device::is_rgb_connected,
            "Returns true if RGB camera is connected."
        )
        
        .def(
            "is_left_connected",
            &Device::is_left_connected,
            "Returns true if left stereo camera is connected."
        )
        
        .def(
            "is_right_connected",
            &Device::is_right_connected,
            "Returns true if right stereo camera is connected."
        )
        .def(
            "write_eeprom_data",
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

                return device.write_eeprom_data(str);
            },
            "Takes board config and calibration data as input and writes to eeprom",
            py::arg("config") = py::dict()
        )
        .def(
            "get_pipeline",
            &Device::get_pipeline,
            "Returns shared ptr of CNNHostPipeline created using cerate_pipeline."
        );


    py::enum_<CaptureMetadata::AutofocusMode>(m, "AutofocusMode")
        .value("AF_MODE_AUTO", CaptureMetadata::AutofocusMode::AF_MODE_AUTO)
        .value("AF_MODE_MACRO", CaptureMetadata::AutofocusMode::AF_MODE_MACRO)
        .value("AF_MODE_CONTINUOUS_VIDEO", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_VIDEO)
        .value("AF_MODE_CONTINUOUS_PICTURE", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_PICTURE)
        .value("AF_MODE_EDOF", CaptureMetadata::AutofocusMode::AF_MODE_EDOF)
    ;

    py::class_<CameraControl> cameraControl(m, "CameraControl");

    py::enum_<CameraControl::CamId>(cameraControl, "CamId")
        .value("RGB",   CameraControl::CamId::RGB)
        .value("LEFT",  CameraControl::CamId::LEFT)
        .value("RIGHT", CameraControl::CamId::RIGHT)
    ;

    py::enum_<CameraControl::Command>(cameraControl, "Command")
        .value("START_STREAM",             CameraControl::Command::START_STREAM)
        .value("STOP_STREAM",              CameraControl::Command::STOP_STREAM)
        .value("REQUEST_STILL",            CameraControl::Command::REQUEST_STILL)
        .value("MOVE_LENS",                CameraControl::Command::MOVE_LENS)
        .value("FOCUS_TRIGGER",            CameraControl::Command::FOCUS_TRIGGER)
        .value("AE_MANUAL",                CameraControl::Command::AE_MANUAL)
        .value("AE_AUTO",                  CameraControl::Command::AE_AUTO)
        .value("SET_AWB_MODE",             CameraControl::Command::SET_AWB_MODE)
        .value("SCENE_MODES",              CameraControl::Command::SCENE_MODES)
        .value("ANTIBANDING_MODES",        CameraControl::Command::ANTIBANDING_MODES)
        .value("EXPOSURE_COMPENSATION",    CameraControl::Command::EXPOSURE_COMPENSATION)
        .value("AE_LOCK",                  CameraControl::Command::AE_LOCK)
        .value("AE_TARGET_FPS_RANGE",      CameraControl::Command::AE_TARGET_FPS_RANGE)
        .value("AWB_LOCK",                 CameraControl::Command::AWB_LOCK)
        .value("CAPTURE_INTENT",           CameraControl::Command::CAPTURE_INTENT)
        .value("CONTROL_MODE",             CameraControl::Command::CONTROL_MODE)
        .value("FRAME_DURATION",           CameraControl::Command::FRAME_DURATION)
        .value("SENSITIVITY",              CameraControl::Command::SENSITIVITY)
        .value("EFFECT_MODE",              CameraControl::Command::EFFECT_MODE)
        .value("AF_MODE",                  CameraControl::Command::AF_MODE)
        .value("NOISE_REDUCTION_STRENGTH", CameraControl::Command::NOISE_REDUCTION_STRENGTH)
        .value("SATURATION",               CameraControl::Command::SATURATION)
        .value("BRIGHTNESS",               CameraControl::Command::BRIGHTNESS)
        .value("STREAM_FORMAT",            CameraControl::Command::STREAM_FORMAT)
        .value("CAM_RESOLUTION",           CameraControl::Command::CAM_RESOLUTION)
        .value("SHARPNESS",                CameraControl::Command::SHARPNESS)
        .value("CUSTOM_USECASE",           CameraControl::Command::CUSTOM_USECASE)
        .value("CUSTOM_CAPT_MODE",         CameraControl::Command::CUSTOM_CAPT_MODE)
        .value("CUSTOM_EXP_BRACKETS",      CameraControl::Command::CUSTOM_EXP_BRACKETS)
        .value("CUSTOM_CAPTURE",           CameraControl::Command::CUSTOM_CAPTURE)
        .value("CONTRAST",                 CameraControl::Command::CONTRAST)
        .value("AE_REGION",                CameraControl::Command::AE_REGION)
        .value("AF_REGION",                CameraControl::Command::AF_REGION)
        .value("LUMA_DENOISE",             CameraControl::Command::LUMA_DENOISE)
        .value("CHROMA_DENOISE",           CameraControl::Command::CHROMA_DENOISE)
    ;

}

