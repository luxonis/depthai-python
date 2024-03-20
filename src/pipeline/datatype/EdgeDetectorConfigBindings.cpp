#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/EdgeDetectorConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_edgedetectorconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<EdgeDetectorConfigData> edgeDetectorConfigData(m, "EdgeDetectorConfigData", DOC(dai, EdgeDetectorConfigData));
    py::class_<RawEdgeDetectorConfig, RawBuffer, std::shared_ptr<RawEdgeDetectorConfig>> rawEdgeDetectorConfig(m, "RawEdgeDetectorConfig", DOC(dai, RawEdgeDetectorConfig));
    py::class_<EdgeDetectorConfig, Buffer, std::shared_ptr<EdgeDetectorConfig>> edgeDetectorConfig(m, "EdgeDetectorConfig", DOC(dai, EdgeDetectorConfig));

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Metadata / raw
    edgeDetectorConfigData
        .def(py::init<>())
        .def_readwrite("sobelFilterHorizontalKernel", &EdgeDetectorConfigData::sobelFilterHorizontalKernel, DOC(dai, EdgeDetectorConfigData, sobelFilterHorizontalKernel))
        .def_readwrite("sobelFilterVerticalKernel", &EdgeDetectorConfigData::sobelFilterVerticalKernel, DOC(dai, EdgeDetectorConfigData, sobelFilterVerticalKernel))
        ;

    rawEdgeDetectorConfig
        .def(py::init<>())
        .def_readwrite("config", &RawEdgeDetectorConfig::config)
        ;

    // Message
    edgeDetectorConfig
        .def(py::init<>())
        .def("setSobelFilterKernels",  &EdgeDetectorConfig::setSobelFilterKernels, py::arg("horizontalKernel"), py::arg("verticalKernel"), DOC(dai, EdgeDetectorConfig, setSobelFilterKernels))
        .def("getConfigData",         &EdgeDetectorConfig::getConfigData, DOC(dai, EdgeDetectorConfig, getConfigData))
        .def("get",         &EdgeDetectorConfig::get, DOC(dai, EdgeDetectorConfig, get))
        .def("set",         &EdgeDetectorConfig::set, py::arg("config"), DOC(dai, EdgeDetectorConfig, set))
        ;

}
