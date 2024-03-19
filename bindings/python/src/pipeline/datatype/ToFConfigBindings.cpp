#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ToFConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_tofconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawToFConfig, RawBuffer, std::shared_ptr<RawToFConfig>> rawToFConfig(m, "RawToFConfig", DOC(dai, RawToFConfig));
    py::class_<ToFConfig, Buffer, std::shared_ptr<ToFConfig>> toFConfig(m, "ToFConfig", DOC(dai, ToFConfig));
    py::class_<ToFConfig::DepthParams> depthParams(toFConfig, "DepthParams", DOC(dai, ToFConfig, DepthParams));
    py::enum_<ToFConfig::DepthParams::TypeFMod> depthParamsTypeFMod(depthParams, "TypeFMod", DOC(dai, ToFConfig, DepthParams, TypeFMod));

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

    // // Metadata / raw
    // toFConfig
    //     .def(py::init<>())
    //     .def_readwrite("depthParams", &ToFConfig::depthParams, DOC(dai, ToFConfig, depthParams))
    //     ;

    depthParamsTypeFMod
        .value("ALL", ToFConfig::DepthParams::TypeFMod::F_MOD_ALL)
        .value("MIN", ToFConfig::DepthParams::TypeFMod::F_MOD_MIN)
        .value("MAX", ToFConfig::DepthParams::TypeFMod::F_MOD_MAX)
    ;

    depthParams
        .def(py::init<>())
        .def_readwrite("freqModUsed", &ToFConfig::DepthParams::freqModUsed, DOC(dai, ToFConfig, DepthParams, freqModUsed))
        .def_readwrite("avgPhaseShuffle", &ToFConfig::DepthParams::avgPhaseShuffle, DOC(dai, ToFConfig, DepthParams, avgPhaseShuffle))
        .def_readwrite("minimumAmplitude", &ToFConfig::DepthParams::minimumAmplitude, DOC(dai, ToFConfig, DepthParams, minimumAmplitude))
        .def_readwrite("median", &ToFConfig::DepthParams::median, DOC(dai, ToFConfig, DepthParams, median))
        ;

    // Message
    toFConfig
        .def(py::init<>())
        // .def(py::init<std::shared_ptr<ToFConfig>>())
        
        .def("setDepthParams", static_cast<ToFConfig&(ToFConfig::*)(dai::ToFConfig::DepthParams)>(&ToFConfig::setDepthParams), py::arg("config"), DOC(dai, ToFConfig, setDepthParams))
        .def("setFreqModUsed", static_cast<ToFConfig&(ToFConfig::*)(dai::ToFConfig::DepthParams::TypeFMod)>(&ToFConfig::setFreqModUsed), DOC(dai, ToFConfig, setFreqModUsed))
        .def("setAvgPhaseShuffle", &ToFConfig::setAvgPhaseShuffle, DOC(dai, ToFConfig, setAvgPhaseShuffle))
        .def("setMinAmplitude", &ToFConfig::setMinAmplitude, DOC(dai, ToFConfig, setMinAmplitude))
        .def("setMedianFilter", &ToFConfig::setMedianFilter, DOC(dai, ToFConfig, setMedianFilter))

        // .def("set", &ToFConfig::set, py::arg("config"), DOC(dai, ToFConfig, set))
        // .def("get", &ToFConfig::get, DOC(dai, ToFConfig, get))
        ;

    // add aliases
    // m.attr("ToFConfig").attr("DepthParams") = m.attr("ToFConfig").attr("DepthParams");

}
