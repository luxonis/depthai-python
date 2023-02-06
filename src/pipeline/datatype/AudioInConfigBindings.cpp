#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/AudioInConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_audioinconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawAudioInConfig, RawBuffer, std::shared_ptr<RawAudioInConfig>> rawAudioInConfig(m, "RawAudioInConfig", DOC(dai, RawAudioInConfig));
    py::class_<AudioInConfig, Buffer, std::shared_ptr<AudioInConfig>> audioInConfig(m, "AudioInConfig", DOC(dai, AudioInConfig));

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
    audioInConfig
        .def(py::init<>())
        .def("setMicGainTimes", &AudioInConfig::setMicGainTimes, py::arg("times"), DOC(dai, AudioInConfig, setMicGainTimes))
        .def("setMicGainDecibels", &AudioInConfig::setMicGainDecibels, py::arg("dB"), DOC(dai, AudioInConfig, setMicGainDecibels))
        .def("getConfigData", &AudioInConfig::getConfigData, DOC(dai, AudioInConfig, getConfigData))
        ;

    rawAudioInConfig
        .def(py::init<>())
        .def_readwrite("config", &RawAudioInConfig::config, DOC(dai, RawAudioInConfig, config))
        ;

}
