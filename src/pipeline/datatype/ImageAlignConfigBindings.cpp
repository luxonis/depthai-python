#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ImageAlignConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_imagealignconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawImageAlignConfig, RawBuffer, std::shared_ptr<RawImageAlignConfig>> rawImageAlignConfig(m, "RawImageAlignConfig", DOC(dai, RawImageAlignConfig));
    py::class_<ImageAlignConfig, Buffer, std::shared_ptr<ImageAlignConfig>> imageAlignConfig(m, "ImageAlignConfig", DOC(dai, ImageAlignConfig));

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

    rawImageAlignConfig
        .def(py::init<>())
        .def_readwrite("staticDepthPlane", &RawImageAlignConfig::staticDepthPlane, DOC(dai, RawImageAlignConfig, staticDepthPlane))
        ;

    // Message
    imageAlignConfig
        .def(py::init<>())
        .def("get",         &ImageAlignConfig::get, DOC(dai, ImageAlignConfig, get))
        .def("set",         &ImageAlignConfig::set, py::arg("config"), DOC(dai, ImageAlignConfig, set))
        ;

}
