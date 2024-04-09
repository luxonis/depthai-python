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
        .def_readwrite("median", &RawToFConfig::median, DOC(dai, RawToFConfig, median))
        .def_readwrite("maxPhaseUnwrappingLevel", &RawToFConfig::maxPhaseUnwrappingLevel, DOC(dai, RawToFConfig, maxPhaseUnwrappingLevel))
        .def_readwrite("enableFPPNCorrection", &RawToFConfig::enableFPPNCorrection, DOC(dai, RawToFConfig, enableFPPNCorrection))
        .def_readwrite("enableOpticalCorrection", &RawToFConfig::enableOpticalCorrection, DOC(dai, RawToFConfig, enableOpticalCorrection))
        .def_readwrite("enableTemperatureCorrection", &RawToFConfig::enableTemperatureCorrection, DOC(dai, RawToFConfig, enableTemperatureCorrection))
        .def_readwrite("enableWiggleCorrection", &RawToFConfig::enableWiggleCorrection, DOC(dai, RawToFConfig, enableWiggleCorrection))
        .def_readwrite("enablePhaseUnwrapping", &RawToFConfig::enablePhaseUnwrapping, DOC(dai, RawToFConfig, enablePhaseUnwrapping))
        ;

    // Message
    toFConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawToFConfig>>())
        
        .def("setMedianFilter", &ToFConfig::setMedianFilter, DOC(dai, ToFConfig, setMedianFilter))

        .def("set", &ToFConfig::set, py::arg("config"), DOC(dai, ToFConfig, set))
        .def("get", &ToFConfig::get, DOC(dai, ToFConfig, get))
        ;

    // add aliases

}
