#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/VisionHealthConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_visionhealthconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawVisionHealthConfig, RawBuffer, std::shared_ptr<RawVisionHealthConfig>> _RawVisionHealthConfig(m, "RawVisionHealthConfig", DOC(dai, RawVisionHealthConfig));
    py::class_<VisionHealthConfig, Buffer, std::shared_ptr<VisionHealthConfig>> _VisionHealthConfig(m, "VisionHealthConfig", DOC(dai, VisionHealthConfig));

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
    _RawVisionHealthConfig
        .def(py::init<>())
        .def_readwrite("depthVarianceRelativeWindowSize", &RawVisionHealthConfig::depthVarianceRelativeWindowSize, DOC(dai, RawVisionHealthConfig, depthVarianceRelativeWindowSize))
        .def_readwrite("edgeHoleRateRelativeWindowSize", &RawVisionHealthConfig::edgeHoleRateRelativeWindowSize, DOC(dai, RawVisionHealthConfig, edgeHoleRateRelativeWindowSize))
        .def_readwrite("depthHoleRateThreshold", &RawVisionHealthConfig::depthHoleRateThreshold, DOC(dai, RawVisionHealthConfig, depthHoleRateThreshold))
        .def_readwrite("edgeStrengthThreshold", &RawVisionHealthConfig::edgeStrengthThreshold, DOC(dai, RawVisionHealthConfig, edgeStrengthThreshold))
        .def_readwrite("edgeHoleRateThreshold", &RawVisionHealthConfig::edgeHoleRateThreshold, DOC(dai, RawVisionHealthConfig, edgeHoleRateThreshold))
        .def_readwrite("edgeHoleRateKernelSize", &RawVisionHealthConfig::edgeHoleRateKernelSize, DOC(dai, RawVisionHealthConfig, edgeHoleRateKernelSize))
        .def_readwrite("depthtHoleRateCval", &RawVisionHealthConfig::depthtHoleRateCval, DOC(dai, RawVisionHealthConfig, depthtHoleRateCval))
        .def_readwrite("visionHealthConfigs", &RawVisionHealthConfig::visionHealthConfigs, DOC(dai, RawVisionHealthConfig, visionHealthConfigs))
        ;

    // Message
    _VisionHealthConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawVisionHealthConfig>>())

        .def("set", &VisionHealthConfig::set, py::arg("config"), DOC(dai, VisionHealthConfig, set))
        .def("get", &VisionHealthConfig::get, DOC(dai, VisionHealthConfig, get))
        .def("addVisionHealthConfig", &VisionHealthConfig::addVisionHealthConfig, DOC(dai, VisionHealthConfig, addVisionHealthConfig))
        ;

}
