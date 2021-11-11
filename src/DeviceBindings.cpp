#include "DeviceBindings.hpp"

// depthai
#include "depthai/device/Device.hpp"
#include "depthai/pipeline/Pipeline.hpp"

// std::chrono bindings
#include <pybind11/chrono.h>
// py::detail
#include <pybind11/detail/common.h>
// hedley
#include <hedley/hedley.h>


// Searches for available devices (as Device constructor)
// but pooling, to check for python interrupts, and releases GIL in between

template<typename DEVICE, class... Args>
static auto deviceSearchHelper(Args&&... args){

    auto startTime = std::chrono::steady_clock::now();
    bool found;
    dai::DeviceInfo deviceInfo = {};
    do {
        {
            // releases python GIL
            py::gil_scoped_release release;
            std::tie(found, deviceInfo) = DEVICE::getFirstAvailableDevice();
            // Check if found
            if(found){
                break;
            } else {
                // block for 100ms
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        // reacquires python GIL for PyErr_CheckSignals call
        // check if interrupt triggered in between
        if (PyErr_CheckSignals() != 0) throw py::error_already_set();
    } while(std::chrono::steady_clock::now() - startTime < DEVICE::DEFAULT_SEARCH_TIME);

    // If neither UNBOOTED nor BOOTLOADER were found (after 'DEFAULT_SEARCH_TIME'), try BOOTED
    if(!found) std::tie(found, deviceInfo) = dai::XLinkConnection::getFirstDevice(X_LINK_BOOTED);

    // if no devices found, then throw
    if(!found) throw std::runtime_error("No available devices");

    return deviceInfo;
}


static std::vector<std::string> deviceGetQueueEventsHelper(dai::Device& d, const std::vector<std::string>& queueNames, std::size_t maxNumEvents, std::chrono::microseconds timeout){
    using namespace std::chrono;

    // if timeout < 0, unlimited timeout
    bool unlimitedTimeout = timeout < microseconds(0);
    auto startTime = steady_clock::now();
    do {
        {
            // releases python GIL
            py::gil_scoped_release release;
            // block for 100ms
            auto events = d.getQueueEvents(queueNames, maxNumEvents, std::chrono::milliseconds(100));
            if(!events.empty()) return events;
        }
        // reacquires python GIL for PyErr_CheckSignals call
        // check if interrupt triggered in between
        if (PyErr_CheckSignals() != 0) throw py::error_already_set();
    } while(unlimitedTimeout || steady_clock::now() - startTime < timeout);

    return std::vector<std::string>();
}


template<typename D, typename ARG>
static void bindConstructors(ARG& arg){
    using namespace dai;

    arg
    .def(py::init([](const Pipeline& pipeline){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, dev);
    }), py::arg("pipeline"), DOC(dai, DeviceBase, DeviceBase))
    .def(py::init([](const Pipeline& pipeline, bool usb2Mode){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, dev, usb2Mode);
    }), py::arg("pipeline"), py::arg("usb2Mode"), DOC(dai, DeviceBase, DeviceBase, 2))
    .def(py::init([](const Pipeline& pipeline, UsbSpeed maxUsbSpeed){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, dev, maxUsbSpeed);
    }), py::arg("pipeline"), py::arg("maxUsbSpeed"), DOC(dai, DeviceBase, DeviceBase, 3))
    .def(py::init([](const Pipeline& pipeline, const std::string& pathToCmd){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, dev, pathToCmd);
    }), py::arg("pipeline"), py::arg("pathToCmd"), DOC(dai, DeviceBase, DeviceBase, 4))
    .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, bool usb2Mode){
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, deviceInfo, usb2Mode);
    }), py::arg("pipeline"), py::arg("devInfo"), py::arg("usb2Mode") = false, DOC(dai, DeviceBase, DeviceBase, 7))
    .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, UsbSpeed maxUsbSpeed){
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, deviceInfo, maxUsbSpeed);
    }), py::arg("pipeline"), py::arg("deviceInfo"), py::arg("maxUsbSpeed"), DOC(dai, DeviceBase, DeviceBase, 8))
    .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, std::string pathToCmd){
        py::gil_scoped_release release;
        return std::make_unique<D>(pipeline, deviceInfo, pathToCmd);
    }), py::arg("pipeline"), py::arg("devInfo"), py::arg("pathToCmd"), DOC(dai, DeviceBase, DeviceBase, 9))

    // DeviceBase constructor - OpenVINO version
    .def(py::init([](OpenVINO::Version version){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(version, dev);
    }), py::arg("version") = OpenVINO::DEFAULT_VERSION, DOC(dai, DeviceBase, DeviceBase, 11))
    .def(py::init([](OpenVINO::Version version, bool usb2Mode){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(version, dev, usb2Mode);
    }), py::arg("version"), py::arg("usb2Mode") = false, DOC(dai, DeviceBase, DeviceBase, 13))
    .def(py::init([](OpenVINO::Version version, UsbSpeed maxUsbSpeed){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(version, dev, maxUsbSpeed);
    }), py::arg("version"), py::arg("maxUsbSpeed"), DOC(dai, DeviceBase, DeviceBase, 14))
    .def(py::init([](OpenVINO::Version version, const std::string& pathToCmd){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(version, dev, pathToCmd);
    }), py::arg("version"), py::arg("pathToCmd"), DOC(dai, DeviceBase, DeviceBase, 15))
    .def(py::init([](OpenVINO::Version version, const DeviceInfo& deviceInfo, bool usb2Mode){
        py::gil_scoped_release release;
        return std::make_unique<D>(version, deviceInfo, usb2Mode);
    }), py::arg("version"), py::arg("deviceDesc"), py::arg("usb2Mode") = false, DOC(dai, DeviceBase, DeviceBase, 18))
    .def(py::init([](OpenVINO::Version version, const DeviceInfo& deviceInfo, UsbSpeed maxUsbSpeed){
        py::gil_scoped_release release;
        return std::make_unique<D>(version, deviceInfo, maxUsbSpeed);
    }), py::arg("version"), py::arg("deviceInfo"), py::arg("maxUsbSpeed"), DOC(dai, DeviceBase, DeviceBase, 19))
    .def(py::init([](OpenVINO::Version version, const DeviceInfo& deviceInfo, std::string pathToCmd){
        py::gil_scoped_release release;
        return std::make_unique<D>(version, deviceInfo, pathToCmd);
    }), py::arg("version"), py::arg("deviceDesc"), py::arg("pathToCmd"), DOC(dai, DeviceBase, DeviceBase, 20))
    .def(py::init([](typename D::Config config){
        auto dev = deviceSearchHelper<D>();
        py::gil_scoped_release release;
        return std::make_unique<D>(config, dev);
    }), py::arg("config"), DOC(dai, DeviceBase, DeviceBase, 22))
    .def(py::init([](typename D::Config config, const DeviceInfo& deviceInfo){
        py::gil_scoped_release release;
        return std::make_unique<D>(config, deviceInfo);
    }), py::arg("config"), py::arg("deviceInfo"), DOC(dai, DeviceBase, DeviceBase, 23))
    ;

}



void DeviceBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Type definitions
    py::class_<DeviceBase> deviceBase(m, "DeviceBase", DOC(dai, DeviceBase));
    py::class_<Device, DeviceBase> device(m, "Device", DOC(dai, Device));
    py::class_<Device::Config> deviceConfig(device, "Config", DOC(dai, DeviceBase, Config));
    py::class_<PrebootConfig> prebootConfig(m, "PrebootConfig", DOC(dai, PrebootConfig));
    py::class_<PrebootConfig::USB> prebootConfigUsb(prebootConfig, "USB", DOC(dai, PrebootConfig, USB));


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


    // Bind PrebootConfig::USB
    prebootConfigUsb
        .def(py::init<>())
        .def_readwrite("vid", &PrebootConfig::USB::vid)
        .def_readwrite("pid", &PrebootConfig::USB::pid)
        .def_readwrite("flashBootedVid", &PrebootConfig::USB::flashBootedVid)
        .def_readwrite("flashBootedPid", &PrebootConfig::USB::flashBootedPid)
        .def_readwrite("maxSpeed", &PrebootConfig::USB::maxSpeed)
    ;

    // Bind PrebootConfig
    prebootConfig
        .def(py::init<>())
        .def_readwrite("usb", &PrebootConfig::usb)
        .def_readwrite("watchdogTimeoutMs", &PrebootConfig::watchdogTimeoutMs)
    ;

    // Bind Device::Config
    deviceConfig
        .def(py::init<>())
        .def_readwrite("version", &Device::Config::version)
        .def_readwrite("preboot", &Device::Config::preboot)
    ;

    // Bind constructors
    bindConstructors<DeviceBase>(deviceBase);
    // Bind the rest
    deviceBase
        // Python only methods
        .def("__enter__", [](DeviceBase& d) -> DeviceBase& { return d; })
        .def("__exit__", [](DeviceBase& d, py::object type, py::object value, py::object traceback) {
            py::gil_scoped_release release;
            d.close();
        })
        .def("close", [](DeviceBase& d) { py::gil_scoped_release release; d.close(); }, "Closes the connection to device. Better alternative is the usage of context manager: `with depthai.Device(pipeline) as device:`")
        .def("isClosed", &DeviceBase::isClosed, "Check if the device is still connected`")

        //dai::Device methods
        //static
        .def_static("getAnyAvailableDevice", [](std::chrono::microseconds us){ return Device::getAnyAvailableDevice(us); }, py::arg("timeout"), DOC(dai, DeviceBase, getAnyAvailableDevice))
        .def_static("getAnyAvailableDevice", [](){ return DeviceBase::getAnyAvailableDevice(); }, DOC(dai, DeviceBase, getAnyAvailableDevice, 2))
        .def_static("getFirstAvailableDevice", &DeviceBase::getFirstAvailableDevice, DOC(dai, DeviceBase, getFirstAvailableDevice))
        .def_static("getAllAvailableDevices", &DeviceBase::getAllAvailableDevices, DOC(dai, DeviceBase, getAllAvailableDevices))
        .def_static("getEmbeddedDeviceBinary", py::overload_cast<bool, OpenVINO::Version>(&DeviceBase::getEmbeddedDeviceBinary), py::arg("usb2Mode"), py::arg("version") = OpenVINO::DEFAULT_VERSION, DOC(dai, DeviceBase, getEmbeddedDeviceBinary))
        .def_static("getEmbeddedDeviceBinary", py::overload_cast<DeviceBase::Config>(&DeviceBase::getEmbeddedDeviceBinary), py::arg("config"), DOC(dai, DeviceBase, getEmbeddedDeviceBinary, 2))
        .def_static("getDeviceByMxId", &DeviceBase::getDeviceByMxId, py::arg("mxId"), DOC(dai, DeviceBase, getDeviceByMxId))

        // methods
        .def("isPipelineRunning", [](DeviceBase& d) { py::gil_scoped_release release; return d.isPipelineRunning(); }, DOC(dai, DeviceBase, isPipelineRunning))
        .def("startPipeline", [](DeviceBase& d){
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Device(pipeline) starts the pipeline automatically. Use Device() and startPipeline(pipeline) otherwise", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            py::gil_scoped_release release;
            d.startPipeline();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, DeviceBase, startPipeline))
        .def("startPipeline", [](DeviceBase& d, const Pipeline& pipeline) { py::gil_scoped_release release; return d.startPipeline(pipeline); }, DOC(dai, DeviceBase, startPipeline, 2))

        // Doesn't require GIL release (eg, don't do RPC or long blocking things in background)
        .def("setLogOutputLevel", &DeviceBase::setLogOutputLevel, py::arg("level"), DOC(dai, DeviceBase, setLogOutputLevel))
        .def("getLogOutputLevel", &DeviceBase::getLogOutputLevel, DOC(dai, DeviceBase, getLogOutputLevel))

        // Requires GIL release
        .def("setLogLevel", [](DeviceBase& d, LogLevel l) { py::gil_scoped_release release; d.setLogLevel(l); }, py::arg("level"), DOC(dai, DeviceBase, setLogLevel))
        .def("getLogLevel", [](DeviceBase& d) { py::gil_scoped_release release; return d.getLogLevel(); }, DOC(dai, DeviceBase, getLogLevel))
        .def("setSystemInformationLoggingRate", [](DeviceBase& d, float hz) { py::gil_scoped_release release; d.setSystemInformationLoggingRate(hz); }, py::arg("rateHz"), DOC(dai, DeviceBase, setSystemInformationLoggingRate))
        .def("getSystemInformationLoggingRate", [](DeviceBase& d) { py::gil_scoped_release release; return d.getSystemInformationLoggingRate(); }, DOC(dai, DeviceBase, getSystemInformationLoggingRate))
        .def("getConnectedCameras", [](DeviceBase& d) { py::gil_scoped_release release; return d.getConnectedCameras(); }, DOC(dai, DeviceBase, getConnectedCameras))
        .def("getDdrMemoryUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getDdrMemoryUsage(); }, DOC(dai, DeviceBase, getDdrMemoryUsage))
        .def("getCmxMemoryUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getCmxMemoryUsage(); }, DOC(dai, DeviceBase, getCmxMemoryUsage))
        .def("getLeonCssHeapUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getLeonCssHeapUsage(); }, DOC(dai, DeviceBase, getLeonCssHeapUsage))
        .def("getLeonMssHeapUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getLeonMssHeapUsage(); }, DOC(dai, DeviceBase, getLeonMssHeapUsage))
        .def("getChipTemperature", [](DeviceBase& d) { py::gil_scoped_release release; return d.getChipTemperature(); }, DOC(dai, DeviceBase, getChipTemperature))
        .def("getLeonCssCpuUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getLeonCssCpuUsage(); }, DOC(dai, DeviceBase, getLeonCssCpuUsage))
        .def("getLeonMssCpuUsage", [](DeviceBase& d) { py::gil_scoped_release release; return d.getLeonMssCpuUsage(); }, DOC(dai, DeviceBase, getLeonMssCpuUsage))
        .def("addLogCallback", [](DeviceBase& d, std::function<void(LogMessage)> callback) { py::gil_scoped_release release; return d.addLogCallback(callback); }, py::arg("callback"), DOC(dai, DeviceBase, addLogCallback))
        .def("removeLogCallback", [](DeviceBase& d, int cbId) { py::gil_scoped_release release; return d.removeLogCallback(cbId); }, py::arg("callbackId"), DOC(dai, DeviceBase, removeLogCallback))
        .def("getUsbSpeed", [](DeviceBase& d) { py::gil_scoped_release release; return d.getUsbSpeed(); }, DOC(dai, DeviceBase, getUsbSpeed))
        .def("getDeviceInfo", [](DeviceBase& d) { py::gil_scoped_release release; return d.getDeviceInfo(); }, DOC(dai, DeviceBase, getDeviceInfo))
        .def("getMxId", [](DeviceBase& d) { py::gil_scoped_release release; return d.getMxId(); }, DOC(dai, DeviceBase, getMxId))
        .def("readCalibration", [](DeviceBase& d) { py::gil_scoped_release release; return d.readCalibration(); }, DOC(dai, DeviceBase, readCalibration))
        .def("flashCalibration", [](DeviceBase& d, CalibrationHandler calibrationDataHandler) { py::gil_scoped_release release; return d.flashCalibration(calibrationDataHandler); }, py::arg("calibrationDataHandler"), DOC(dai, DeviceBase, flashCalibration))
        .def("setXLinkChunkSize", [](DeviceBase& d, int s) { py::gil_scoped_release release; d.setXLinkChunkSize(s); }, py::arg("sizeBytes"), DOC(dai, DeviceBase, setXLinkChunkSize))
        .def("getXLinkChunkSize", [](DeviceBase& d) { py::gil_scoped_release release; return d.getXLinkChunkSize(); }, DOC(dai, DeviceBase, getXLinkChunkSize))
    ;


    // Bind constructors
    bindConstructors<Device>(device);
    // Bind the rest
    device
        .def("getOutputQueue", static_cast<std::shared_ptr<DataOutputQueue>(Device::*)(const std::string&)>(&Device::getOutputQueue), py::arg("name"), DOC(dai, Device, getOutputQueue))
        .def("getOutputQueue", static_cast<std::shared_ptr<DataOutputQueue>(Device::*)(const std::string&, unsigned int, bool)>(&Device::getOutputQueue), py::arg("name"), py::arg("maxSize"), py::arg("blocking") = true, DOC(dai, Device, getOutputQueue, 2))
        .def("getOutputQueueNames", &Device::getOutputQueueNames, DOC(dai, Device, getOutputQueueNames))

        .def("getInputQueue", static_cast<std::shared_ptr<DataInputQueue>(Device::*)(const std::string&)>(&Device::getInputQueue), py::arg("name"), DOC(dai, Device, getInputQueue))
        .def("getInputQueue", static_cast<std::shared_ptr<DataInputQueue>(Device::*)(const std::string&, unsigned int, bool)>(&Device::getInputQueue), py::arg("name"), py::arg("maxSize"), py::arg("blocking") = true, DOC(dai, Device, getInputQueue, 2))
        .def("getInputQueueNames", &Device::getInputQueueNames, DOC(dai, Device, getInputQueueNames))

        .def("getQueueEvents", [](Device& d, const std::vector<std::string>& queueNames, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            return deviceGetQueueEventsHelper(d, queueNames, maxNumEvents, timeout);
        }, py::arg("queueNames"), py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvents))
        .def("getQueueEvents", [](Device& d, std::string queueName, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            return deviceGetQueueEventsHelper(d, std::vector<std::string>{queueName}, maxNumEvents, timeout);
        }, py::arg("queueName"), py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvents, 3))
        .def("getQueueEvents", [](Device& d, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            return deviceGetQueueEventsHelper(d, d.getOutputQueueNames(), maxNumEvents, timeout);
        }, py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvents, 4))

        .def("getQueueEvent", [](Device& d, const std::vector<std::string>& queueNames, std::chrono::microseconds timeout) {
            auto events = deviceGetQueueEventsHelper(d, queueNames, std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("queueNames"), py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvent))

        .def("getQueueEvent", [](Device& d, std::string queueName, std::chrono::microseconds timeout) {
            auto events = deviceGetQueueEventsHelper(d, std::vector<std::string>{queueName}, std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("queueName"), py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvent, 3))

        .def("getQueueEvent", [](Device& d, std::chrono::microseconds timeout) {
            auto events = deviceGetQueueEventsHelper(d, d.getOutputQueueNames(), std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("timeout") = std::chrono::microseconds(-1), DOC(dai, Device, getQueueEvent, 4))

        //.def("setCallback", DeviceWrapper::wrap(&Device::setCallback), py::arg("name"), py::arg("callback"))

    ;

}