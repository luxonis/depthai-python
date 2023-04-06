#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/TofCameraConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_tofcameraconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawTofCameraConfig, RawBuffer, std::shared_ptr<RawTofCameraConfig>> rawTofCameraConfig(m, "RawTofCameraConfig", DOC(dai, RawTofCameraConfig));
    py::class_<TofCameraConfig, Buffer, std::shared_ptr<TofCameraConfig>> tofCameraConfig(m, "TofCameraConfig", DOC(dai, TofCameraConfig));

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
    rawTofCameraConfig
        .def(py::init<>())
        .def_readwrite("dummy", &RawTofCameraConfig::dummy, DOC(dai, RawTofCameraConfig, dummy))
        ;

    // Message
    tofCameraConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawTofCameraConfig>>())

        .def("set", &TofCameraConfig::set, py::arg("config"), DOC(dai, TofCameraConfig, set))
        .def("get", &TofCameraConfig::get, DOC(dai, TofCameraConfig, get))
        ;

}
