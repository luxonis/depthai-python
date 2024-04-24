#include "DeviceBootloaderBindings.hpp"

// depthai
#include "depthai/device/DeviceBootloader.hpp"

void DeviceBootloaderBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Type definitions
    py::class_<DeviceBootloader> deviceBootloader(m, "DeviceBootloader", DOC(dai, DeviceBootloader));
    py::enum_<DeviceBootloader::Type> deviceBootloaderType(deviceBootloader, "Type");
    py::enum_<DeviceBootloader::Memory> deviceBootloaderMemory(deviceBootloader, "Memory");
    py::enum_<DeviceBootloader::Section> deviceBootloaderSection(deviceBootloader, "Section");
    py::class_<DeviceBootloader::UsbConfig> deviceBootlaoderUsbConfig(deviceBootloader, "UsbConfig");
    py::class_<DeviceBootloader::NetworkConfig> deviceBootlaoderNetworkConfig(deviceBootloader, "NetworkConfig");
    py::class_<DeviceBootloader::Config> deviceBootloderConfig(deviceBootloader, "Config");
    py::class_<DeviceBootloader::ApplicationInfo> deviceBootloderApplicationInfo(deviceBootloader, "ApplicationInfo");
    py::class_<DeviceBootloader::MemoryInfo> deviceBootloderMemoryInfo(deviceBootloader, "MemoryInfo");


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
        .def("toJson", &DeviceBootloader::Config::toJson)
        .def("fromJson", &DeviceBootloader::Config::fromJson)
    ;

    deviceBootloderApplicationInfo
        .def(py::init<>())
        .def_readwrite("hasApplication", &DeviceBootloader::ApplicationInfo::hasApplication)
        .def_readwrite("firmwareVersion", &DeviceBootloader::ApplicationInfo::firmwareVersion)
        .def_readwrite("applicationName", &DeviceBootloader::ApplicationInfo::applicationName)
    ;

    deviceBootloderMemoryInfo
        .def(py::init<>())
        .def_readwrite("available", &DeviceBootloader::MemoryInfo::available)
        .def_readwrite("size", &DeviceBootloader::MemoryInfo::size)
        .def_readwrite("info", &DeviceBootloader::MemoryInfo::info)
    ;

    deviceBootloader
      // Python only methods
        .def("__enter__", [](DeviceBootloader& d) -> DeviceBootloader& { return d; })
        .def("__exit__", [](DeviceBootloader& bl, py::object type, py::object value, py::object traceback) { bl.close(); })
        .def("close", &DeviceBootloader::close, "Closes the connection to device. Better alternative is the usage of context manager: `with depthai.DeviceBootloader(deviceInfo) as bootloader:`")

        .def_static("getFirstAvailableDevice", &DeviceBootloader::getFirstAvailableDevice, DOC(dai, DeviceBootloader, getFirstAvailableDevice))
        .def_static("getAllAvailableDevices", &DeviceBootloader::getAllAvailableDevices, DOC(dai, DeviceBootloader, getAllAvailableDevices))
        .def_static("saveDepthaiApplicationPackage", py::overload_cast<const Path&, const Pipeline&, const Path&, bool, std::string, bool>(&DeviceBootloader::saveDepthaiApplicationPackage), py::arg("path"), py::arg("pipeline"), py::arg("pathToCmd") = Path{}, py::arg("compress") = false, py::arg("applicationName") = "", py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, saveDepthaiApplicationPackage))
        .def_static("saveDepthaiApplicationPackage", py::overload_cast<const Path&, const Pipeline&, bool, std::string, bool>(&DeviceBootloader::saveDepthaiApplicationPackage), py::arg("path"), py::arg("pipeline"), py::arg("compress"), py::arg("applicationName") = "", py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, saveDepthaiApplicationPackage, 2))
        .def_static("createDepthaiApplicationPackage", py::overload_cast<const Pipeline&, const Path&, bool, std::string, bool>(&DeviceBootloader::createDepthaiApplicationPackage), py::arg("pipeline"), py::arg("pathToCmd") = Path{}, py::arg("compress") = false, py::arg("applicationName") = "", py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, createDepthaiApplicationPackage))
        .def_static("createDepthaiApplicationPackage", py::overload_cast<const Pipeline&, bool, std::string, bool>(&DeviceBootloader::createDepthaiApplicationPackage), py::arg("pipeline"), py::arg("compress"), py::arg("applicationName") = "", py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, createDepthaiApplicationPackage, 2))
        .def_static("getEmbeddedBootloaderVersion", &DeviceBootloader::getEmbeddedBootloaderVersion, DOC(dai, DeviceBootloader, getEmbeddedBootloaderVersion))
        .def_static("getEmbeddedBootloaderBinary", &DeviceBootloader::getEmbeddedBootloaderBinary, DOC(dai, DeviceBootloader, getEmbeddedBootloaderBinary))

        .def(py::init<const DeviceInfo&, bool>(), py::arg("devInfo"), py::arg("allowFlashingBootloader") = false, DOC(dai, DeviceBootloader, DeviceBootloader, 4))
        .def(py::init<const DeviceInfo&, const Path&, bool>(), py::arg("devInfo"), py::arg("pathToCmd"), py::arg("allowFlashingBootloader") = false, DOC(dai, DeviceBootloader, DeviceBootloader, 5))
        .def(py::init<std::string, bool>(), py::arg("nameOrDeviceId"), py::arg("allowFlashingBootloader") = false, DOC(dai, DeviceBootloader, DeviceBootloader, 6))

        .def("flash", [](DeviceBootloader& db, std::function<void(float)> progressCallback, const Pipeline& pipeline, bool compress, std::string applicationName, DeviceBootloader::Memory memory, bool checkChecksum) { py::gil_scoped_release release; return db.flash(progressCallback, pipeline, compress, applicationName, memory, checkChecksum); }, py::arg("progressCallback"), py::arg("pipeline"), py::arg("compress") = false, py::arg("applicationName") = "", py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, flash))
        .def("flash", [](DeviceBootloader& db, const Pipeline& pipeline, bool compress, std::string applicationName, DeviceBootloader::Memory memory, bool checkChecksum) { py::gil_scoped_release release; return db.flash(pipeline, compress, applicationName, memory, checkChecksum); }, py::arg("pipeline"), py::arg("compress") = false, py::arg("applicationName") = "", py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("checkChecksum") = false, DOC(dai, DeviceBootloader, flash, 2))

        .def("readApplicationInfo", [](DeviceBootloader& db, DeviceBootloader::Memory mem) { py::gil_scoped_release release; return db.readApplicationInfo(mem); }, py::arg("memory"), DOC(dai, DeviceBootloader, readApplicationInfo))
        .def("getMemoryInfo", [](DeviceBootloader& db, DeviceBootloader::Memory memory) { py::gil_scoped_release release; return db.getMemoryInfo(memory); }, DOC(dai, DeviceBootloader, getMemoryInfo))
        .def("isUserBootloader", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.isUserBootloader(); }, DOC(dai, DeviceBootloader, isUserBootloader))
        .def("isUserBootloaderSupported", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.isUserBootloaderSupported(); }, DOC(dai, DeviceBootloader, isUserBootloaderSupported))

        .def("flashDepthaiApplicationPackage", [](DeviceBootloader& db, std::function<void(float)> progressCallback, std::vector<uint8_t> package, DeviceBootloader::Memory memory) { py::gil_scoped_release release; return db.flashDepthaiApplicationPackage(progressCallback, package); }, py::arg("progressCallback"), py::arg("package"), py::arg("memory") = DeviceBootloader::Memory::AUTO, DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage))
        .def("flashDepthaiApplicationPackage", [](DeviceBootloader& db, std::vector<uint8_t> package, DeviceBootloader::Memory memory) { py::gil_scoped_release release; return db.flashDepthaiApplicationPackage(package); }, py::arg("package"), py::arg("memory") = DeviceBootloader::Memory::AUTO, DOC(dai, DeviceBootloader, flashDepthaiApplicationPackage, 2))
        .def("flashBootloader", [](DeviceBootloader& db, std::function<void(float)> progressCallback, const Path& path) { py::gil_scoped_release release; return db.flashBootloader(progressCallback, path); }, py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader))
        .def("flashBootloader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type, std::function<void(float)> progressCallback, dai::Path path) { py::gil_scoped_release release; return db.flashBootloader(memory, type, progressCallback, path); }, py::arg("memory"), py::arg("type"), py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashBootloader, 2))
        .def("flashUserBootloader", [](DeviceBootloader& db, std::function<void(float)> progressCallback, const Path& path) { py::gil_scoped_release release; return db.flashUserBootloader(progressCallback, path); }, py::arg("progressCallback"), py::arg("path") = "", DOC(dai, DeviceBootloader, flashUserBootloader))

        .def("readConfigData", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.readConfigData(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, readConfigData))
        .def("flashConfigData", [](DeviceBootloader& db, nlohmann::json configData, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigData(configData, memory, type); }, py::arg("configData"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigData))
        .def("flashConfigFile", [](DeviceBootloader& db, dai::Path configPath, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigFile(configPath, memory, type); }, py::arg("configData"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigFile))
        .def("flashConfigClear", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfigClear(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfigClear))
        .def("readConfig", [](DeviceBootloader& db, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.readConfig(memory, type); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, readConfig))
        .def("flashConfig", [](DeviceBootloader& db, const DeviceBootloader::Config& config, DeviceBootloader::Memory memory, DeviceBootloader::Type type) { py::gil_scoped_release release; return db.flashConfig(config, memory, type); }, py::arg("config"), py::arg("memory") = DeviceBootloader::Memory::AUTO, py::arg("type") = DeviceBootloader::Type::AUTO, DOC(dai, DeviceBootloader, flashConfig))

        .def("bootMemory", [](DeviceBootloader& db, const std::vector<uint8_t>& fw) { py::gil_scoped_release release; return db.bootMemory(fw); }, py::arg("fw"), DOC(dai, DeviceBootloader, bootMemory))
        .def("bootUsbRomBootloader", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.bootUsbRomBootloader(); }, DOC(dai, DeviceBootloader, bootUsbRomBootloader))

        //.def("flashCustom", &DeviceBootloader::flashCustom, py::arg("memory"), py::arg("offset"), py::arg("progressCallback"), py::arg("data"), DOC(dai, DeviceBootloader, flashCustom))
        .def("getVersion", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.getVersion(); }, DOC(dai, DeviceBootloader, getVersion))
        .def("getFlashedVersion", [](DeviceBootloader& db) { py::gil_scoped_release release; return db.getFlashedVersion(); }, DOC(dai, DeviceBootloader, getFlashedVersion))

        .def("isEmbeddedVersion", &DeviceBootloader::isEmbeddedVersion, DOC(dai, DeviceBootloader, isEmbeddedVersion))
        .def("getType", &DeviceBootloader::getType, DOC(dai, DeviceBootloader, getType))
        .def("isAllowedFlashingBootloader", &DeviceBootloader::isAllowedFlashingBootloader, DOC(dai, DeviceBootloader, isAllowedFlashingBootloader))

        .def("flashClear", [](DeviceBootloader& db, DeviceBootloader::Memory memory) { py::gil_scoped_release release; return db.flashClear(memory); }, py::arg("memory") = DeviceBootloader::Memory::AUTO, DOC(dai, DeviceBootloader, flashClear))
        .def("flashGpioModeBootHeader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, int mode) { py::gil_scoped_release release; return db.flashGpioModeBootHeader(memory, mode); }, py::arg("memory"), py::arg("mode"), DOC(dai, DeviceBootloader, flashGpioModeBootHeader))
        .def("flashUsbRecoveryBootHeader", [](DeviceBootloader& db, DeviceBootloader::Memory memory) { py::gil_scoped_release release; return db.flashUsbRecoveryBootHeader(memory); }, py::arg("memory"), DOC(dai, DeviceBootloader, flashUsbRecoveryBootHeader))
        .def("flashBootHeader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, int32_t frequency, int64_t location, int32_t dummyCycles, int64_t offset) { py::gil_scoped_release release; return db.flashBootHeader(memory, frequency, location, dummyCycles, offset); }, py::arg("memory"), py::arg("frequency") = -1, py::arg("location") = -1, py::arg("dummyCycles") = -1, py::arg("offset") = -1, DOC(dai, DeviceBootloader, flashBootHeader))
        .def("flashFastBootHeader", [](DeviceBootloader& db, DeviceBootloader::Memory memory, int32_t frequency, int64_t location, int32_t dummyCycles, int64_t offset) { py::gil_scoped_release release; return db.flashFastBootHeader(memory, frequency, location, dummyCycles, offset); }, py::arg("memory"), py::arg("frequency") = -1, py::arg("location") = -1, py::arg("dummyCycles") = -1, py::arg("offset") = -1, DOC(dai, DeviceBootloader, flashFastBootHeader))
        .def("flashCustom", [](DeviceBootloader& db, DeviceBootloader::Memory memory, size_t offset, const std::vector<uint8_t>& data, std::function<void(float)> progressCb) { py::gil_scoped_release release; return db.flashCustom(memory, offset, data, progressCb); }, py::arg("memory"), py::arg("offset"), py::arg("data"), py::arg("progressCallback") = nullptr, DOC(dai, DeviceBootloader, flashCustom))
        .def("flashCustom", [](DeviceBootloader& db, DeviceBootloader::Memory memory, size_t offset, std::string filename, std::function<void(float)> progressCb) { py::gil_scoped_release release; return db.flashCustom(memory, offset, filename, progressCb); }, py::arg("memory"), py::arg("offset"), py::arg("filename"), py::arg("progressCallback") = nullptr, DOC(dai, DeviceBootloader, flashCustom))
        .def("readCustom", [](DeviceBootloader& db, DeviceBootloader::Memory memory, size_t offset, size_t size, std::string filename, std::function<void(float)> progressCb) { py::gil_scoped_release release; return db.readCustom(memory, offset, size, filename, progressCb); }, py::arg("memory"), py::arg("offset"), py::arg("size"), py::arg("filename"), py::arg("progressCallback") = nullptr, DOC(dai, DeviceBootloader, readCustom))
        .def("readCustom", [](DeviceBootloader& db, DeviceBootloader::Memory memory, size_t offset, size_t size, std::function<void(float)> progressCb) { py::gil_scoped_release release; return db.readCustom(memory, offset, size, progressCb); }, py::arg("memory"), py::arg("offset"), py::arg("size"), py::arg("progressCallback") = nullptr, DOC(dai, DeviceBootloader, readCustom))
        ;

}
