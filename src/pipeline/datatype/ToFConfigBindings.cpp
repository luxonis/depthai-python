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
    py::class_<ToFConfig, Buffer, std::shared_ptr<ToFConfig>> tofConfig(m, "ToFConfig", DOC(dai, ToFConfig));

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
        .def_readwrite("dummy", &RawToFConfig::dummy, DOC(dai, RawToFConfig, dummy))
        ;

    // Message
    tofConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawToFConfig>>())

        .def("set", &ToFConfig::set, py::arg("config"), DOC(dai, ToFConfig, set))
        .def("get", &ToFConfig::get, DOC(dai, ToFConfig, get))
        ;

}
