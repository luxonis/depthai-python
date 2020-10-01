#include "device_bindings.hpp"

//std
#include <iostream>

//depthai-core
#include "depthai/device.hpp"
//#include "depthai/host_capture_command.hpp"

//depthai-shared
//#include "depthai-shared/metadata/capture_metadata.hpp"

//project
#include "pybind11_common.hpp"




// Binding for HostDataPacket
namespace py = pybind11;

void init_binding_device(pybind11::module& m){
      
    using namespace dai;

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

}

