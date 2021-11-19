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
    py::class_<DeviceBootloader::UsbConfig> deviceBootlaoderUsbConfig(deviceBootloader, "UsbConfig");
    py::class_<DeviceBootloader::NetworkConfig> deviceBootlaoderNetworkConfig(deviceBootloader, "NetworkConfig");
    py::class_<DeviceBootloader::Config> deviceBootloderConfig(deviceBootloader, "Config");


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
        .value("AUTO", DeviceBootloader::Type::AUTO)
        .value("USB", DeviceBootloader::Type::USB)
        .value("NETWORK", DeviceBootloader::Type::NETWORK)
        ;
    deviceBootloaderMemory
        .value("AUTO", DeviceBootloader::Memory::AUTO)
        .value("FLASH", DeviceBootloader::Memory::FLASH)
        .value("EMMC", DeviceBootloader::Memory::EMMC)
        ;
    deviceBootloaderSection
        .value("AUTO", DeviceBootloader::Section::AUTO)
        .value("HEADER", DeviceBootloader::Section::HEADER)
        .value("BOOTLOADER", DeviceBootloader::Section::BOOTLOADER)
        .value("BOOTLOADER_CONFIG", DeviceBootloader::Section::BOOTLOADER_CONFIG)
        .value("APPLICATION", DeviceBootloader::Section::APPLICATION)
        ;

    deviceBootlaoderUsbConfig
        .def(py::init<>())
        .def_readwrite("timeoutMs", &DeviceBootloader::UsbConfig::timeoutMs)
        .def_readwrite("maxUsbSpeed", &DeviceBootloader::UsbConfig::maxUsbSpeed)
        .def_readwrite("vid", &DeviceBootloader::UsbConfig::vid)
        .def_readwrite("pid", &DeviceBootloader::UsbConfig::pid)
    ;
    deviceBootlaoderNetworkConfig
        .def(py::init<>())
        .def_readwrite("timeoutMs", &DeviceBootloader::NetworkConfig::timeoutMs)
        .def_readwrite("ipv4", &DeviceBootloader::NetworkConfig::ipv4)
        .def_readwrite("ipv4Mask", &DeviceBootloader::NetworkConfig::ipv4Mask)
        .def_readwrite("ipv4Gateway", &DeviceBootloader::NetworkConfig::ipv4Gateway)
        .def_readwrite("ipv4Dns", &DeviceBootloader::NetworkConfig::ipv4Dns)
        .def_readwrite("ipv4DnsAlt", &DeviceBootloader::NetworkConfig::ipv4DnsAlt)
        .def_readwrite("staticIpv4", &DeviceBootloader::NetworkConfig::staticIpv4)
        .def_readwrite("ipv6", &DeviceBootloader::NetworkConfig::ipv6)
        .def_readwrite("ipv6Prefix", &DeviceBootloader::NetworkConfig::ipv6Prefix)
        .def_readwrite("ipv6Gateway", &DeviceBootloader::NetworkConfig::ipv6Gateway)
        .def_readwrite("ipv6Dns", &DeviceBootloader::NetworkConfig::ipv6Dns)
        .def_readwrite("ipv6DnsAlt", &DeviceBootloader::NetworkConfig::ipv6DnsAlt)
        .def_readwrite("staticIpv6", &DeviceBootloader::NetworkConfig::staticIpv6)
        .def_readwrite("mac", &DeviceBootloader::NetworkConfig::mac)
    ;

    deviceBootloderConfig
        .def(py::init<>())
        .def_readwrite("appMem", &DeviceBootloader::Config::appMem)
        .def_readwrite("usb", &DeviceBootloader::Config::usb)
        .def_readwrite("network", &DeviceBootloader::Config::network)
        .def("setStaticIPv4", &DeviceBootloader::Config::setStaticIPv4)
        .def("setDynamicIPv4", &DeviceBootloader::Config::setDynamicIPv4)
        .def("isStaticIPV4", &DeviceBootloader::Config::isStaticIPV4)
        .def("getIPv4", &DeviceBootloader::Config::getIPv4)
        .def("getIPv4Mask", &DeviceBootloader::Config::getIPv4Mask)
        .def("getIPv4Gateway", &DeviceBootloader::Config::getIPv4Gateway)
        .def("setDnsIPv4", &DeviceBootloader::Config::setDnsIPv4)
        .def("getDnsIPv4", &DeviceBootloader::Config::getDnsIPv4)
        .def("getDnsAltIPv4", &DeviceBootloader::Config::getDnsAltIPv4)
        .def("setUsbTimeout", &DeviceBootloader::Config::setUsbTimeout)
        .def("getUsbTimeout", &DeviceBootloader::Config::getUsbTimeout)
        .def("setNetworkTimeout", &DeviceBootloader::Config::setNetworkTimeout)
        .def("getNetworkTimeout", &DeviceBootloader::Config::getNetworkTimeout)
        .def("setMacAddress", &DeviceBootloader::Config::setMacAddress)
        .def("getMacAddress", &DeviceBootloader::Config::getMacAddress)
        .def("setUsbMaxSpeed", &DeviceBootloader::Config::setUsbMaxSpeed)
        .def("getUsbMaxSpeed", &DeviceBootloader::Config::getUsbMaxSpeed)
    ;

    deviceBootloader
      // Python only methods
        .def("__enter__", [](DeviceBootloader& d) -> DeviceBootloader& { return d; })
        .def("__exit__", [](DeviceBootloader& bl, py::object type, py::object value, py::object traceback) { bl.close(); })
        .def("close", &DeviceBootloader::close, "Closes the connection to device. Better alternative is the usage of context manager: `with depthai.DeviceBootloader(deviceInfo) as bootloader:`")

        .def_static("getFirstAvailableDevice", &DeviceBootloader::getFirstAvailableDevice, DOC(dai, DeviceBootloader, getFirstAvailableDevice))
        .def_static("getAllAvailableDevices", &DeviceBootloader::getAllAvailableDevices, DOC(dai, DeviceBootloader, getAllAvailableDevices))
        .def_static("saveDepthaiApplicationPackage", py::overload_cast<std::string, const Pipeline&, std::string, bool>(&DeviceBootloader::saveDepthaiApplicationPackage), py::arg("path"), py::arg("pipeline"), py::arg("pathToCmd") = "", py::arg("compress") = false, DOC(dai, DeviceBootloader, saveDepthaiApplicationPackage))
        .def_static("saveDepthaiApplicationPackage", py::overload_cast<std::string, const Pipeline&, bool>(&DeviceBootloader::saveDepthaiApplicationPackage), py::arg("path"), py::arg("pipeline"), py::arg("compress"), DOC(dai, DeviceBootloader, saveDepthaiApplicationPackage, 2))
        .def_static("createDepthaiApplicationPackage", py::overload_cast<const Pipeline&, std::string, bool>(&DeviceBootloader::createDepthaiApplicationPackage), py::arg("pipeline"), py::arg("pathToCmd") = "", py::arg("compress") = false, DOC(dai, DeviceBootloader, createDepthaiApplicationPackage))
        .def_static("createDepthaiApplicationPackage", py::overload_cast<const Pipeline&, bool>(&DeviceBootloader::createDepthaiApplicationPackage), py::arg("pipeline"), py::arg("compress"), DOC(dai, DeviceBootloader, createDepthaiApplicationPackage, 2))
        .def_static("getEmbeddedBootloaderVersion", &DeviceBootloader::getEmbeddedBootloaderVersion, DOC(dai, DeviceBootloader, getEmbeddedBootloaderVersion))
        .def_static("getEmbeddedBootloaderBinary", &DeviceBootloader::getEmbeddedBootloaderBinary, DOC(dai, DeviceBootloader, getEmbeddedBootloaderBinary))

        .def(py::init<const DeviceInfo&, bool>(), py::arg("devInfo"), py::arg("allowFlashingBootloader") = false, DOC(dai, DeviceBootloader, DeviceBootloader))
        .def(py::init<const DeviceInfo&, std::string, bool>(), py::arg("devInfo"), py::arg("pathToCmd"), py::arg("allowFlashingBootloader") = false, DOC(dai, DeviceBootloader, DeviceBootloader, 2))
        .def("flash", [](DeviceBootloader& db, std::function<void(float)> progressCallback, const Pipeline& pipeline, bool compress) { py::gil_scoped_release release; return db.flash(progressCallback, pipeline, compress); }, py::arg("progressCallback"), py::arg("pipeline"), py::arg("compress") = false, DOC(dai, DeviceBootloader, flash))
        .def("flash", [](DeviceBootloader& db, const Pipeline& pipeline, bool compress) { py::gil_scoped_release release; return db.flash(pipeline, compress); }, py::arg("pipeline"), py::arg("compress") = false, DOC(dai, DeviceBootloader, flash, 2))
        .def("flashDepthaiApplicationPackage", [](DeviceBootloader& db, std::function<void(float)> progressCallback, std::vector<uint8_t> package) { py::gil_scoped_release release; return db.flashDepthaiApplicationPackage(progressCallback, package); }, py::arg("progressCallback"), py::arg("package"), DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage))
        .def("flashDepthaiApplicationPackage", [](DeviceBootloader& db, std::vector<uint8_t> package) { py::gil_scoped_release release; return db.flashDepthaiApplicationPackage(package); }, py::arg("package"), DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage, 2))
        .def("flashBootloader", [](DeviceBootloader& db, std::function<void(float)> progressCallback, std::string path) { py::gil_scoped_release release; return db.flashBootloader(progressCallback, path); }, py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader))
        .def("flashBootloader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type, std::function<void(float)> progressCallback, std::string path) { py::gil_scoped_release release; return db.flashBootloader(memory, type, progressCallback, path); }, py::arg("memory"), py::arg("type"), py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader, 2))

        .def("readConfigData", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.readConfigData(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, readConfigData))
        .def("flashConfigData", [](DeviceBootloader& db, nlohmann::json configData, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigData(configData, memory, type); }, py::arg("configData"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigData))
        .def("flashConfigFile", [](DeviceBootloader& db, std::string configPath, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigFile(configPath, memory, type); }, py::arg("configData"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigFile))
        .def("flashConfigClear", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigClear(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigClear))
        .def("readConfig", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.readConfig(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, readConfig))
        .def("flashConfig", [](DeviceBootloader& db, const DeviceBootloader::Config& config, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfig(config, memory, type); }, py::arg("config"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfig))

        .def("bootMemory", [](DeviceBootloader& db, const std::vector<uint8_t>& fw) { py::gil_scoped_release release; return db.bootMemory(fw); }, py::arg("fw"), DOC(dai, DeviceBootloader, bootMemory))
        .def("bootUsbRomBootloader", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.bootUsbRomBootloader(); }, DOC(dai, DeviceBootloader, bootUsbRomBootloader))

        //.def("flashCustom", &DeviceBootloader::flashCustom, py::arg("memory"), py::arg("offset"), py::arg("progressCallback"), py::arg("data"), DOC(dai, DeviceBootloader, flashCustom))
        .def("getVersion", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.getVersion(); }, DOC(dai, DeviceBootloader, getVersion))

        .def("isEmbeddedVersion", &DeviceBootloader::isEmbeddedVersion, DOC(dai, DeviceBootloader, isEmbeddedVersion))
        .def("getType", &DeviceBootloader::getType, DOC(dai, DeviceBootloader, getType))
        .def("isAllowedFlashingBootloader", &DeviceBootloader::isAllowedFlashingBootloader, DOC(dai, DeviceBootloader, isAllowedFlashingBootloader))
        ;

}
