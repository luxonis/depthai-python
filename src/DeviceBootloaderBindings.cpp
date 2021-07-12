#include "DeviceBootloaderBindings.hpp"

// depthai
#include "depthai/device/DeviceBootloader.hpp"

void DeviceBootloaderBindings::bind(pybind11::module& m){

    using namespace dai;

    // Bind DeviceBootloader
    py::class_<DeviceBootloader> deviceBootloader(m, "DeviceBootloader", DOC(dai, DeviceBootloader));

    py::class_<DeviceBootloader::Version>(deviceBootloader, "Version", DOC(dai, DeviceBootloader, Version))
        .def(py::init<const std::string&>(), py::arg("v"), DOC(dai, DeviceBootloader, Version, Version))
        .def(py::init<unsigned, unsigned, unsigned>(), py::arg("major"), py::arg("minor"), py::arg("patch"), DOC(dai, DeviceBootloader, Version, Version, 2))
        .def("__str__", &DeviceBootloader::Version::toString)
        .def("__eq__", &DeviceBootloader::Version::operator==)
        .def("__lt__", &DeviceBootloader::Version::operator<)
        .def("__gt__", &DeviceBootloader::Version::operator>)
        ;

    py::enum_<DeviceBootloader::Type>(deviceBootloader, "Type")
        .value("USB", DeviceBootloader::Type::USB)
        .value("NETWORK", DeviceBootloader::Type::NETWORK)
        ;
    py::enum_<DeviceBootloader::Memory>(deviceBootloader, "Memory")
        .value("FLASH", DeviceBootloader::Memory::FLASH)
        .value("EMMC", DeviceBootloader::Memory::EMMC)
        ;
    py::enum_<DeviceBootloader::Section>(deviceBootloader, "Section")
        .value("HEADER", DeviceBootloader::Section::HEADER)
        .value("BOOTLOADER", DeviceBootloader::Section::BOOTLOADER)
        .value("BOOTLOADER_CONFIG", DeviceBootloader::Section::BOOTLOADER_CONFIG)
        .value("APPLICATION", DeviceBootloader::Section::APPLICATION)
        ;

    deviceBootloader
      // Python only methods
        .def("__enter__", [](py::object obj){ return obj; })
        .def("__exit__", [](DeviceBootloader& bl, py::object type, py::object value, py::object traceback) { bl.close(); })
        .def("close", &DeviceBootloader::close, "Closes the connection to device. Better alternative is the usage of context manager: `with depthai.DeviceBootloader(deviceInfo) as bootloader:`")

        .def_static("getFirstAvailableDevice", &DeviceBootloader::getFirstAvailableDevice, DOC(dai, DeviceBootloader, getFirstAvailableDevice))
        .def_static("getAllAvailableDevices", &DeviceBootloader::getAllAvailableDevices, DOC(dai, DeviceBootloader, getAllAvailableDevices))
        .def_static("saveDepthaiApplicationPackage", &DeviceBootloader::saveDepthaiApplicationPackage, py::arg("path"), py::arg("pipeline"), py::arg("pathToCmd") = "", DOC(dai, DeviceBootloader, saveDepthaiApplicationPackage))
        .def_static("createDepthaiApplicationPackage", &DeviceBootloader::createDepthaiApplicationPackage, py::arg("pipeline"), py::arg("pathToCmd") = "", DOC(dai, DeviceBootloader, createDepthaiApplicationPackage))
        .def_static("getEmbeddedBootloaderVersion", &DeviceBootloader::getEmbeddedBootloaderVersion, DOC(dai, DeviceBootloader, getEmbeddedBootloaderVersion))
        .def_static("getEmbeddedBootloaderBinary", &DeviceBootloader::getEmbeddedBootloaderBinary, DOC(dai, DeviceBootloader, getEmbeddedBootloaderBinary))

        .def(py::init<const DeviceInfo&>(), py::arg("deviceDesc"), DOC(dai, DeviceBootloader, DeviceBootloader))
        .def(py::init<const DeviceInfo&, std::string>(), py::arg("deviceDesc"), py::arg("pathToCmd"), DOC(dai, DeviceBootloader, DeviceBootloader, 2))
        .def("flash", &DeviceBootloader::flash, py::arg("progressCallback"), py::arg("pipeline"), DOC(dai, DeviceBootloader, flash))
        .def("flashDepthaiApplicationPackage", &DeviceBootloader::flashDepthaiApplicationPackage, py::arg("progressCallback"), py::arg("package"), DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage))
        .def("flashBootloader", py::overload_cast<std::function<void(float)>, std::string>(&DeviceBootloader::flashBootloader), py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader))
        .def("flashBootloader", py::overload_cast<DeviceBootloader::Memory, DeviceBootloader::Type, std::function<void(float)>, std::string>(&DeviceBootloader::flashBootloader), py::arg("memory"), py::arg("type"), py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader, 2))
        //.def("flashCustom", &DeviceBootloader::flashCustom, py::arg("memory"), py::arg("offset"), py::arg("progressCallback"), py::arg("data"), DOC(dai, DeviceBootloader, flashCustom))
        .def("getVersion", &DeviceBootloader::getVersion, DOC(dai, DeviceBootloader, getVersion))
        .def("isEmbeddedVersion", &DeviceBootloader::isEmbeddedVersion, DOC(dai, DeviceBootloader, isEmbeddedVersion))
        ;

}
