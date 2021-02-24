#include "DeviceBindings.hpp"

// depthai
#include "depthai/device/Device.hpp"

// std::chrono bindings
#include <pybind11/chrono.h>
// py::detail
#include <pybind11/detail/common.h>

// Searches for available devices (as Device constructor)
// but pooling, to check for python interrupts, and releases GIL in between
static std::unique_ptr<dai::Device> deviceConstructorHelper(const dai::Pipeline& pipeline, const std::string& pathToCmd = "", bool usb2Mode = false){
    auto startTime = std::chrono::steady_clock::now();
    bool found;
    dai::DeviceInfo deviceInfo = {};
    do {
        {          
            // releases python GIL
            py::gil_scoped_release release;
            std::tie(found, deviceInfo) = dai::Device::getFirstAvailableDevice();
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
    } while(std::chrono::steady_clock::now() - startTime < dai::Device::DEFAULT_SEARCH_TIME);

    // If neither UNBOOTED nor BOOTLOADER were found (after 'DEFAULT_SEARCH_TIME'), try BOOTED
    if(!found) std::tie(found, deviceInfo) = dai::XLinkConnection::getFirstDevice(X_LINK_BOOTED);

    // if no devices found, then throw
    if(!found) throw std::runtime_error("No available devices");

    // Check if pathToCmd supplied
    if(pathToCmd.empty()){
        return std::unique_ptr<dai::Device>(new dai::Device(pipeline, deviceInfo, usb2Mode));
    } else {
        return std::unique_ptr<dai::Device>(new dai::Device(pipeline, deviceInfo, pathToCmd));
    }
    return nullptr;
} 


std::vector<std::string> deviceGetQueueEventsHelper(dai::Device& d, const std::vector<std::string>& queueNames, std::size_t maxNumEvents, std::chrono::microseconds timeout){
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



// Device wrapper. This wrapper keeps an unique_ptr and has the ability to destruct the underlying object
class DeviceWrapper {
    std::unique_ptr<dai::Device> pDevice;
public:
    // Wrapping function (methods)
    template <typename Return, typename Class, typename... Args>
    static auto wrap(Return (Class::*f)(Args...)) {
        return [f](DeviceWrapper& dw, Args... args){ 
            dw.check();
            return (dw.pDevice.get()->*f)(args...);
        };
    }

    // Wrapping function (const methods)
    template <typename Return, typename Class, typename... Args>
    static auto wrap(Return (Class::*f)(Args...) const) {
        return [f](DeviceWrapper& dw, Args... args){ 
            dw.check();
            return (dw.pDevice.get()->*f)(args...);
        };
    }

    // Wrapping function (functions)
    template <typename Return, typename... Args>
    static auto wrap(Return (*f)(Args...)) {
        return [f](DeviceWrapper& dw, Args... args){ 
            dw.check();
            return (dw.pDevice.get()->*f)(args...);
        };
    }
    
    // Constructor wrapper - overload
    static std::unique_ptr<DeviceWrapper> wrap(std::unique_ptr<dai::Device> ptr){
        return std::make_unique<DeviceWrapper>(std::move(ptr));
    }

    // Constructor
    DeviceWrapper(std::unique_ptr<dai::Device> ptr){
        pDevice = std::move(std::move(ptr));
    }

    // Close function, destructs device
    void close(){
        pDevice = nullptr;
    }

    // check
    void check(){
        if(!pDevice){
            throw std::invalid_argument("Device was already closed");
        }
    }

    // Device reference
    dai::Device& get(){
        check();
        return *pDevice;
    }
};


void DeviceBindings::bind(pybind11::module& m){

    using namespace dai;


    // Bind Device, using DeviceWrapper to be able to destruct the object by calling close()
    py::class_<DeviceWrapper>(m, "Device")
        // Python only methods
        .def("__enter__", [](py::object obj){ return obj; })
        .def("__exit__", [](DeviceWrapper& dw, py::object type, py::object value, py::object traceback) { dw.close(); })
        .def("close", &DeviceWrapper::close)

        //dai::Device methods
        //static (no need for the wrapper)
        .def_static("getAnyAvailableDevice", [](std::chrono::microseconds us){ return Device::getAnyAvailableDevice(us); }, py::arg("timeout"))
        .def_static("getAnyAvailableDevice", [](){ return Device::getAnyAvailableDevice(); })
        .def_static("getFirstAvailableDevice", &Device::getFirstAvailableDevice)
        .def_static("getAllAvailableDevices", &Device::getAllAvailableDevices)
        .def_static("getEmbeddedDeviceBinary", &Device::getEmbeddedDeviceBinary, py::arg("usb2Mode"), py::arg("version") = Pipeline::DEFAULT_OPENVINO_VERSION)
        .def_static("getDeviceByMxId", &Device::getDeviceByMxId, py::arg("mxId"))
        
        // methods
        .def(py::init([](const Pipeline& pipeline){ return DeviceWrapper::wrap(deviceConstructorHelper(pipeline)); }), py::arg("pipeline"))
        .def(py::init([](const Pipeline& pipeline, bool usb2Mode){ 
            // Blocking constructor
            return DeviceWrapper::wrap(deviceConstructorHelper(pipeline, std::string(""), usb2Mode)); 
        }), py::arg("pipeline"), py::arg("usb2Mode"))
        .def(py::init([](const Pipeline& pipeline, const std::string& pathToCmd){
            // Blocking constructor
            return DeviceWrapper::wrap(deviceConstructorHelper(pipeline, pathToCmd)); 
        }), py::arg("pipeline"), py::arg("pathToCmd"))
        .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, bool usb2Mode){
            // Non blocking constructor
            return DeviceWrapper::wrap(std::unique_ptr<Device>(new Device(pipeline, deviceInfo, usb2Mode)));
        }), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("usb2Mode") = false)
        .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, std::string pathToCmd){
            // Non blocking constructor
            return DeviceWrapper::wrap(std::unique_ptr<Device>(new Device(pipeline, deviceInfo, pathToCmd)));
        }), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def("isPipelineRunning", DeviceWrapper::wrap(&Device::isPipelineRunning))
        .def("startPipeline", DeviceWrapper::wrap(&Device::startPipeline))

        .def("getOutputQueue", DeviceWrapper::wrap(static_cast<std::shared_ptr<DataOutputQueue>(Device::*)(const std::string&)>(&Device::getOutputQueue)), py::arg("name"))
        .def("getOutputQueue", DeviceWrapper::wrap(static_cast<std::shared_ptr<DataOutputQueue>(Device::*)(const std::string&, unsigned int, bool)>(&Device::getOutputQueue)), py::arg("name"), py::arg("maxSize"), py::arg("blocking") = true)
        .def("getOutputQueueNames", DeviceWrapper::wrap(&Device::getOutputQueueNames))

        .def("getInputQueue", DeviceWrapper::wrap(static_cast<std::shared_ptr<DataInputQueue>(Device::*)(const std::string&)>(&Device::getInputQueue)), py::arg("name"))
        .def("getInputQueue", DeviceWrapper::wrap(static_cast<std::shared_ptr<DataInputQueue>(Device::*)(const std::string&, unsigned int, bool)>(&Device::getInputQueue)), py::arg("name"), py::arg("maxSize"), py::arg("blocking") = true)
        .def("getInputQueueNames", DeviceWrapper::wrap(&Device::getInputQueueNames))

        .def("getQueueEvents", [](DeviceWrapper& dw, const std::vector<std::string>& queueNames, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            dw.check();
            return deviceGetQueueEventsHelper(dw.get(), queueNames, maxNumEvents, timeout);
        }, py::arg("queueNames"), py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1))
        .def("getQueueEvents", [](DeviceWrapper& dw, std::string queueName, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            dw.check();
            return deviceGetQueueEventsHelper(dw.get(), std::vector<std::string>{queueName}, maxNumEvents, timeout);
        }, py::arg("queueName"), py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1))
        .def("getQueueEvents", [](DeviceWrapper& dw, std::size_t maxNumEvents, std::chrono::microseconds timeout) {
            dw.check();
            return deviceGetQueueEventsHelper(dw.get(), dw.get().getOutputQueueNames(), maxNumEvents, timeout);
        }, py::arg("maxNumEvents") = std::numeric_limits<std::size_t>::max(), py::arg("timeout") = std::chrono::microseconds(-1))

        .def("getQueueEvent", [](DeviceWrapper& dw, const std::vector<std::string>& queueNames, std::chrono::microseconds timeout) {
            dw.check();
            auto events = deviceGetQueueEventsHelper(dw.get(), queueNames, std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("queueNames"), py::arg("timeout") = std::chrono::microseconds(-1))
        .def("getQueueEvent", [](DeviceWrapper& dw, std::string queueName, std::chrono::microseconds timeout) {
            dw.check();
            auto events = deviceGetQueueEventsHelper(dw.get(), std::vector<std::string>{queueName}, std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("queueName"), py::arg("timeout") = std::chrono::microseconds(-1))
        .def("getQueueEvent", [](DeviceWrapper& dw, std::chrono::microseconds timeout) {
            dw.check();
            auto events = deviceGetQueueEventsHelper(dw.get(), dw.get().getOutputQueueNames(), std::numeric_limits<std::size_t>::max(), timeout);
            if(events.empty()) return std::string("");
            return events[0];
        }, py::arg("timeout") = std::chrono::microseconds(-1))

        .def("setCallback", DeviceWrapper::wrap(&Device::setCallback), py::arg("name"), py::arg("callback"))
        .def("setLogLevel", DeviceWrapper::wrap(&Device::setLogLevel), py::arg("level"))
        .def("getLogLevel", DeviceWrapper::wrap(&Device::getLogLevel))
        .def("setSystemInformationLoggingRate", DeviceWrapper::wrap(&Device::setSystemInformationLoggingRate), py::arg("rateHz"))
        .def("getSystemInformationLoggingRate", DeviceWrapper::wrap(&Device::getSystemInformationLoggingRate))
        .def("getDdrMemoryUsage", DeviceWrapper::wrap(&Device::getDdrMemoryUsage))
        .def("getCmxMemoryUsage", DeviceWrapper::wrap(&Device::getCmxMemoryUsage))
        .def("getLeonCssHeapUsage", DeviceWrapper::wrap(&Device::getLeonCssHeapUsage))
        .def("getLeonMssHeapUsage", DeviceWrapper::wrap(&Device::getLeonMssHeapUsage))
        .def("getChipTemperature", DeviceWrapper::wrap(&Device::getChipTemperature))
        .def("getLeonCssCpuUsage", DeviceWrapper::wrap(&Device::getLeonCssCpuUsage))
        .def("getLeonMssCpuUsage", DeviceWrapper::wrap(&Device::getLeonMssCpuUsage))
        .def("setLogOutputLevel", DeviceWrapper::wrap(&Device::setLogOutputLevel))
        .def("getLogOutputLevel", DeviceWrapper::wrap(&Device::getLogOutputLevel))
        .def("addLogCallback", DeviceWrapper::wrap(&Device::addLogCallback))
        .def("removeLogCallback", DeviceWrapper::wrap(&Device::removeLogCallback))
        ;

}