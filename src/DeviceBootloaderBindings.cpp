#include "DeviceBootloaderBindings.hpp"

// depthai
#include "depthai/device/DeviceBootloader.hpp"

// pybind for functions
#include "pybind11/functional.h"

void DeviceBootloaderBindings::bind(pybind11::module& m){

    using namespace dai;

    // Bind DeviceBootloader
    py::class_<DeviceBootloader> deviceBootloader(m, "DeviceBootloader");

    py::class_<DeviceBootloader::Version>(deviceBootloader, "Version")
        .def(py::init<const std::string&>())
        .def(py::init<unsigned, unsigned, unsigned>())
        .def("__str__", &DeviceBootloader::Version::toString)
        .def("__eq__", &DeviceBootloader::Version::operator==)
        .def("__lt__", &DeviceBootloader::Version::operator<)
        .def("__gt__", &DeviceBootloader::Version::operator>)
        ;

    deviceBootloader
        // Python only methods
        .def("__enter__", [](py::object obj){ return obj; })
        .def("__exit__", [](DeviceBootloader& bl, py::object type, py::object value, py::object traceback) { bl.close(); })
        .def("close", &DeviceBootloader::close)

        .def_static("getFirstAvailableDevice", &DeviceBootloader::getFirstAvailableDevice)
        .def_static("getAllAvailableDevices", &DeviceBootloader::getAllAvailableDevices)
        .def_static("saveDepthaiApplicationPackage", &DeviceBootloader::saveDepthaiApplicationPackage, py::arg("path"), py::arg("pipeline"), py::arg("pathToCmd") = "")
        .def_static("createDepthaiApplicationPackage", &DeviceBootloader::createDepthaiApplicationPackage, py::arg("pipeline"), py::arg("pathToCmd") = "")
        .def_static("getEmbeddedBootloaderVersion", &DeviceBootloader::getEmbeddedBootloaderVersion)
        .def_static("getEmbeddedBootloaderBinary", &DeviceBootloader::getEmbeddedBootloaderBinary)
        .def(py::init<const DeviceInfo&>(), py::arg("deviceDesc"))
        .def(py::init<const DeviceInfo&, const char*>(), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def(py::init<const DeviceInfo&, std::string>(), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def("flash", &DeviceBootloader::flash, py::arg("progressCallback"), py::arg("pipeline"))
        .def("flashDepthaiApplicationPackage", &DeviceBootloader::flashDepthaiApplicationPackage, py::arg("progressCallback"), py::arg("package"))
        .def("flashBootloader", &DeviceBootloader::flashBootloader, py::arg("progressCallback"), py::arg("path") = "")
        .def("getVersion", &DeviceBootloader::getVersion)
        .def("isEmbeddedVersion", &DeviceBootloader::isEmbeddedVersion)
        ;

}