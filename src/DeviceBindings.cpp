#include "DeviceBindings.hpp"

// depthai
#include "depthai/device/Device.hpp"

void DeviceBindings::bind(pybind11::module& m){

    using namespace dai;


    // Bind global properties
    py::class_<Device>(m, "Device")
        .def_static("getFirstAvailableDevice", &Device::getFirstAvailableDevice)
        .def_static("getAllAvailableDevices", &Device::getAllAvailableDevices)
        .def_static("getEmbeddedDeviceBinary", &Device::getEmbeddedDeviceBinary, py::arg("usb2Mode"), py::arg("version") = Pipeline::DEFAULT_OPENVINO_VERSION)
        .def_static("getDeviceByMxId", &Device::getDeviceByMxId, py::arg("mxId"))
        .def(py::init<const Pipeline&>())
        .def(py::init<const Pipeline&, bool>(), py::arg("pipeline"), py::arg("usb2Mode"))
        .def(py::init<const Pipeline&, const char*>(), py::arg("pipeline"), py::arg("pathToCmd"))
        .def(py::init<const Pipeline&, const std::string&>(), py::arg("pipeline"), py::arg("pathToCmd"))
        .def(py::init<const Pipeline&, const DeviceInfo&, bool>(), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("usb2Mode") = false)
        .def(py::init<const Pipeline&, const DeviceInfo&, std::string>(), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def(py::init<const Pipeline&, const DeviceInfo&, bool>(), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("usb2Mode") = false)
        .def(py::init<const Pipeline&, const DeviceInfo&, std::string>(), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def("isPipelineRunning", &Device::isPipelineRunning)
        .def("startPipeline", &Device::startPipeline)
        .def("getOutputQueue", &Device::getOutputQueue, py::arg("name"), py::arg("maxSize") = 120, py::arg("overwrite") = false )
        .def("getInputQueue", &Device::getInputQueue,  py::arg("name"), py::arg("maxSize") = 120, py::arg("overwrite") = false )
        .def("setCallback", &Device::setCallback)
        ;

}