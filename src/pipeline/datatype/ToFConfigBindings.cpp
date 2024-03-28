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

    py::class_<RawToFConfig, RawBuffer, std::shared_ptr<RawToFConfig>> rawToFConfig(m, "RawToFConfig", DOC(dai, RawToFConfig));
    py::class_<RawToFConfig::DepthParams> depthParams(rawToFConfig, "DepthParams", DOC(dai, RawToFConfig, DepthParams));
    py::enum_<RawToFConfig::DepthParams::TypeFMod> depthParamsTypeFMod(depthParams, "TypeFMod", DOC(dai, RawToFConfig, DepthParams, TypeFMod));
    py::class_<ToFConfig, Buffer, std::shared_ptr<ToFConfig>> toFConfig(m, "ToFConfig", DOC(dai, ToFConfig));

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
    rawToFConfig
        .def(py::init<>())
        .def_readwrite("depthParams", &RawToFConfig::depthParams, DOC(dai, RawToFConfig, depthParams))
        .def_readwrite("enableFPPNCorrection", &RawToFConfig::enableFPPNCorrection, DOC(dai, RawToFConfig, enableFPPNCorrection))
        .def_readwrite("enableOpticalCorrection", &RawToFConfig::enableOpticalCorrection, DOC(dai, RawToFConfig, enableOpticalCorrection))
        .def_readwrite("enableTemperatureCorrection", &RawToFConfig::enableTemperatureCorrection, DOC(dai, RawToFConfig, enableTemperatureCorrection))
        .def_readwrite("enableWiggleCorrection", &RawToFConfig::enableWiggleCorrection, DOC(dai, RawToFConfig, enableWiggleCorrection))
        ;

    depthParamsTypeFMod
        .value("ALL", RawToFConfig::DepthParams::TypeFMod::F_MOD_ALL)
        .value("MIN", RawToFConfig::DepthParams::TypeFMod::F_MOD_MIN)
        .value("MAX", RawToFConfig::DepthParams::TypeFMod::F_MOD_MAX)
    ;

    depthParams
        .def(py::init<>())
        .def_readwrite("freqModUsed", &RawToFConfig::DepthParams::freqModUsed, DOC(dai, RawToFConfig, DepthParams, freqModUsed))
        .def_readwrite("avgPhaseShuffle", &RawToFConfig::DepthParams::avgPhaseShuffle, DOC(dai, RawToFConfig, DepthParams, avgPhaseShuffle))
        .def_readwrite("minimumAmplitude", &RawToFConfig::DepthParams::minimumAmplitude, DOC(dai, RawToFConfig, DepthParams, minimumAmplitude))
        .def_readwrite("median", &RawToFConfig::DepthParams::median, DOC(dai, RawToFConfig, DepthParams, median))
        ;

    // Message
    toFConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawToFConfig>>())
        
        .def("setDepthParams", static_cast<ToFConfig&(ToFConfig::*)(dai::ToFConfig::DepthParams)>(&ToFConfig::setDepthParams), py::arg("config"), DOC(dai, ToFConfig, setDepthParams))
        .def("setFreqModUsed", static_cast<ToFConfig&(ToFConfig::*)(dai::ToFConfig::DepthParams::TypeFMod)>(&ToFConfig::setFreqModUsed), DOC(dai, ToFConfig, setFreqModUsed))
        .def("setAvgPhaseShuffle", &ToFConfig::setAvgPhaseShuffle, DOC(dai, ToFConfig, setAvgPhaseShuffle))
        .def("setMinAmplitude", &ToFConfig::setMinAmplitude, DOC(dai, ToFConfig, setMinAmplitude))
        .def("setMedianFilter", &ToFConfig::setMedianFilter, DOC(dai, ToFConfig, setMedianFilter))

        .def("set", &ToFConfig::set, py::arg("config"), DOC(dai, ToFConfig, set))
        .def("get", &ToFConfig::get, DOC(dai, ToFConfig, get))
        ;

    // add aliases
    m.attr("ToFConfig").attr("DepthParams") = m.attr("RawToFConfig").attr("DepthParams");

}
