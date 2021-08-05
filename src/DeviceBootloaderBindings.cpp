#include "DeviceBootloaderBindings.hpp"

// depthai
#include "depthai/device/DeviceBootloader.hpp"

void DeviceBootloaderBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Type definitions
    py::class_<DeviceBootloader> deviceBootloader(m, "DeviceBootloader", DOC(dai, DeviceBootloader));
    py::class_<DeviceBootloader::Version> deviceBootloaderVersion(deviceBootloader, "Version", DOC(dai, DeviceBootloader, Version));
    py::enum_<DeviceBootloader::Type> deviceBootloaderType(deviceBootloader, "Type");
    py::enum_<DeviceBootloader::Memory> deviceBootloaderMemory(deviceBootloader, "Memory");
    py::enum_<DeviceBootloader::Section> deviceBootloaderSection(deviceBootloader, "Section");


    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////


    // Bind DeviceBootloader
    deviceBootloaderVersion
        .def(py::init<const std::string&>(), py::arg("v"), DOC(dai, DeviceBootloader, Version, Version))
        .def(py::init<unsigned, unsigned, unsigned>(), py::arg("major"), py::arg("minor"), py::arg("patch"), DOC(dai, DeviceBootloader, Version, Version, 2))
        .def("__str__", &DeviceBootloader::Version::toString)
        .def("__eq__", &DeviceBootloader::Version::operator==)
        .def("__lt__", &DeviceBootloader::Version::operator<)
        .def("__gt__", &DeviceBootloader::Version::operator>)
        ;

    deviceBootloaderType
        .value("USB", DeviceBootloader::Type::USB)
        .value("NETWORK", DeviceBootloader::Type::NETWORK)
        ;
    deviceBootloaderMemory
        .value("FLASH", DeviceBootloader::Memory::FLASH)
        .value("EMMC", DeviceBootloader::Memory::EMMC)
        ;
    deviceBootloaderSection
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
        .def("flash", [](DeviceBootloader& db, std::function<void(float)> progressCallback, Pipeline& pipeline) { py::gil_scoped_release release; return db.flash(progressCallback, pipeline); }, py::arg("progressCallback"), py::arg("pipeline"), DOC(dai, DeviceBootloader, flash))
        .def("flashDepthaiApplicationPackage", [](DeviceBootloader& db, std::function<void(float)> progressCallback, std::vector<uint8_t> package) { py::gil_scoped_release release; return db.flashDepthaiApplicationPackage(progressCallback, package); }, py::arg("progressCallback"), py::arg("package"), DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage))
        .def("flashBootloader", [](DeviceBootloader& db, std::function<void(float)> progressCallback, std::string path) { py::gil_scoped_release release; return db.flashBootloader(progressCallback, path); }, py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader))
        .def("flashBootloader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type, std::function<void(float)> progressCallback, std::string path) { py::gil_scoped_release release; return db.flashBootloader(memory, type, progressCallback, path); }, py::arg("memory"), py::arg("type"), py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader, 2))
        //.def("flashCustom", &DeviceBootloader::flashCustom, py::arg("memory"), py::arg("offset"), py::arg("progressCallback"), py::arg("data"), DOC(dai, DeviceBootloader, flashCustom))
        .def("getVersion", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.getVersion(); }, DOC(dai, DeviceBootloader, getVersion))

        .def("isEmbeddedVersion", &DeviceBootloader::isEmbeddedVersion, DOC(dai, DeviceBootloader, isEmbeddedVersion))
        ;

}
