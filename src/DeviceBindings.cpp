#include "DeviceBindings.hpp"

// depthai
#include "depthai/device/Device.hpp"

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
                // Check if pathToCmd supplied
                if(pathToCmd.empty()){
                    return std::unique_ptr<dai::Device>(new dai::Device(pipeline, deviceInfo, usb2Mode));
                } else {
                    return std::unique_ptr<dai::Device>(new dai::Device(pipeline, deviceInfo, pathToCmd));
                }
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

    if(!found) throw std::runtime_error("No available devices");
    return nullptr;
} 


void DeviceBindings::bind(pybind11::module& m){

    using namespace dai;


    // Bind global properties
    py::class_<Device>(m, "Device")
        .def_static("getAnyAvailableDevice", [](std::chrono::microseconds us){ return Device::getAnyAvailableDevice(us); }, py::arg("timeout"))
        .def_static("getAnyAvailableDevice", [](){ return Device::getAnyAvailableDevice(); })
        .def_static("getFirstAvailableDevice", &Device::getFirstAvailableDevice)
        .def_static("getAllAvailableDevices", &Device::getAllAvailableDevices)
        .def_static("getEmbeddedDeviceBinary", &Device::getEmbeddedDeviceBinary, py::arg("usb2Mode"), py::arg("version") = Pipeline::DEFAULT_OPENVINO_VERSION)
        .def_static("getDeviceByMxId", &Device::getDeviceByMxId, py::arg("mxId"))
        .def(py::init([](const Pipeline& pipeline){ return deviceConstructorHelper(pipeline); }), py::arg("pipeline"))
        .def(py::init([](const Pipeline& pipeline, bool usb2Mode){ 
            // Blocking constructor
            return deviceConstructorHelper(pipeline, std::string(""), usb2Mode); 
        }), py::arg("pipeline"), py::arg("usb2Mode"))
        .def(py::init([](const Pipeline& pipeline, const std::string& pathToCmd){
            // Blocking constructor
            return deviceConstructorHelper(pipeline, pathToCmd); 
        }), py::arg("pipeline"), py::arg("pathToCmd"))
        .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, bool usb2Mode){
            // Non blocking constructor
            return std::unique_ptr<Device>(new Device(pipeline, deviceInfo, usb2Mode));
        }), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("usb2Mode") = false)
        .def(py::init([](const Pipeline& pipeline, const DeviceInfo& deviceInfo, std::string pathToCmd){
            // Non blocking constructor
            return std::unique_ptr<Device>(new Device(pipeline, deviceInfo, pathToCmd));
        }), py::arg("pipeline"), py::arg("deviceDesc"), py::arg("pathToCmd"))
        .def("isPipelineRunning", &Device::isPipelineRunning)
        .def("startPipeline", &Device::startPipeline)
        .def("getOutputQueue", &Device::getOutputQueue, py::arg("name"), py::arg("maxSize") = 16, py::arg("blocking") = true )
        .def("getInputQueue", &Device::getInputQueue,  py::arg("name"), py::arg("maxSize") = 16, py::arg("blocking") = true )
        .def("setCallback", &Device::setCallback, py::arg("name"), py::arg("callback"))
        .def("setLogLevel", &Device::setLogLevel, py::arg("level"))
        .def("getLogLevel", &Device::getLogLevel)
        .def("getDdrMemoryUsage", &Device::getDdrMemoryUsage)
        .def("getLeonOsHeapUsage", &Device::getLeonOsHeapUsage)
        .def("getLeonRtHeapUsage", &Device::getLeonRtHeapUsage)
        .def("getChipTemperature", &Device::getChipTemperature)
        ;

}