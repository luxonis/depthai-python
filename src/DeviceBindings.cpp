#include "DeviceBindings.hpp"

// depthai
#include "depthai/Device.hpp"

void DeviceBindings::bind(pybind11::module& m){

    using namespace dai;

    // Bind global properties
    py::class_<Device>(m, "Device")
        .def(py::init<>())
        .def(py::init<const DeviceInfo&, bool>(), py::arg("deviceDesc"), py::arg("usb2Mode") = false)
        .def(py::init<const DeviceInfo&, std::string>(), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def("isPipelineRunning", &Device::isPipelineRunning)
        .def("startPipeline", &Device::startPipeline)
        .def("getOutputQueue", &Device::getOutputQueue, py::arg("name"), py::arg("maxSize") = 120, py::arg("overwrite") = false )
        .def("getInputQueue", &Device::getInputQueue,  py::arg("name"), py::arg("maxSize") = 120, py::arg("overwrite") = false )
        .def("setCallback", &Device::setCallback)
        ;

}