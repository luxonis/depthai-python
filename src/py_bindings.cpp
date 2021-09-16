
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

// pybind11
#include "pybind11_common.hpp"

// depthai-core
#include "depthai/build/version.hpp"

// project
#include "depthai/depthai.hpp"
#include "pipeline/AssetManagerBindings.hpp"
#include "pipeline/PipelineBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include "pipeline/NodeBindings.hpp"
#include "XLinkBindings.hpp"
#include "DeviceBindings.hpp"
#include "CalibrationHandlerBindings.hpp"
#include "DeviceBootloaderBindings.hpp"
#include "DatatypeBindings.hpp"
#include "DataQueueBindings.hpp"
#include "openvino/OpenVINOBindings.hpp"
#include "log/LogBindings.hpp"

PYBIND11_MODULE(depthai, m)
{

    // Depthai python version consists of: (depthai-core).(bindings revision)[+bindings hash]
    m.attr("__version__") = DEPTHAI_PYTHON_VERSION;
    m.attr("__commit__") = DEPTHAI_PYTHON_COMMIT_HASH;
    m.attr("__commit_datetime__") = DEPTHAI_PYTHON_COMMIT_DATETIME;
    m.attr("__build_datetime__") = DEPTHAI_PYTHON_BUILD_DATETIME;

    // Add bindings
    std::deque<StackFunction> callstack;
    callstack.push_front(&DatatypeBindings::bind);
    callstack.push_front(&LogBindings::bind);
    callstack.push_front(&DataQueueBindings::bind);
    callstack.push_front(&OpenVINOBindings::bind);
    callstack.push_front(&AssetManagerBindings::bind);
    callstack.push_front(&NodeBindings::bind);
    callstack.push_front(&PipelineBindings::bind);
    callstack.push_front(&XLinkBindings::bind);
    callstack.push_front(&DeviceBindings::bind);
    callstack.push_front(&DeviceBootloaderBindings::bind);
    callstack.push_front(&CalibrationHandlerBindings::bind);
    // end of the callstack
    callstack.push_front([](py::module &, void *) {});

    Callstack callstackAdapter(callstack);

    // Initial call
    CommonBindings::bind(m, &callstackAdapter);

    // Install signal handler option
    bool installSignalHandler = true;
    constexpr static const char* signalHandlerKey = "DEPTHAI_INSTALL_SIGNAL_HANDLER";
    try {
        auto sysModule = py::module_::import("sys");
        if(py::hasattr(sysModule, signalHandlerKey)){
            installSignalHandler = installSignalHandler && sysModule.attr(signalHandlerKey).cast<bool>();
        }
    } catch (...) {
        // ignore
    }
    try {
        auto builtinsModule = py::module_::import("builtins");
        if(py::hasattr(builtinsModule, signalHandlerKey)){
            installSignalHandler = installSignalHandler && builtinsModule.attr(signalHandlerKey).cast<bool>();
        }
    } catch (...){
        // ignore
    }

    // Call dai::initialize on 'import depthai' to initialize asap with additional information to print
    dai::initialize(std::string("Python bindings - version: ") + DEPTHAI_PYTHON_VERSION + " from " + DEPTHAI_PYTHON_COMMIT_DATETIME + " build: " + DEPTHAI_PYTHON_BUILD_DATETIME, installSignalHandler);

}
