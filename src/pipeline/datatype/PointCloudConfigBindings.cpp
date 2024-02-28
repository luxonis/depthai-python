#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/PointCloudConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_pointcloudconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawPointCloudConfig, RawBuffer, std::shared_ptr<RawPointCloudConfig>> rawConfig(m, "RawPointCloudConfig", DOC(dai, RawPointCloudConfig));
    py::class_<PointCloudConfig, Buffer, std::shared_ptr<PointCloudConfig>> config(m, "PointCloudConfig", DOC(dai, PointCloudConfig));

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
    rawConfig
        .def(py::init<>())
        .def_readwrite("sparse", &RawPointCloudConfig::sparse, DOC(dai, RawPointCloudConfig, sparse))
        .def_readwrite("transformationMatrix", &RawPointCloudConfig::transformationMatrix, DOC(dai, RawPointCloudConfig, transformationMatrix))
        ;

    // Message
    config
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawPointCloudConfig>>())
        
        .def("set", &PointCloudConfig::set, py::arg("config"), DOC(dai, PointCloudConfig, set))
        .def("get", &PointCloudConfig::get, DOC(dai, PointCloudConfig, get))
        .def("getSparse", &PointCloudConfig::getSparse, DOC(dai, PointCloudConfig, getSparse))
        .def("getTransformationMatrix", &PointCloudConfig::getTransformationMatrix, DOC(dai, PointCloudConfig, getTransformationMatrix))
        .def("setSparse", &PointCloudConfig::setSparse, DOC(dai, PointCloudConfig, setSparse))
        .def("setTransformationMatrix", [](PointCloudConfig& cfg, std::array<std::array<float, 3>, 3> mat) {
            return cfg.setTransformationMatrix(mat);
        }, DOC(dai, PointCloudConfig, setTransformationMatrix))

        .def("setTransformationMatrix", [](PointCloudConfig& cfg, std::array<std::array<float, 4>, 4> mat) {
            return cfg.setTransformationMatrix(mat);
        }, DOC(dai, PointCloudConfig, setTransformationMatrix))
        ;

    // add aliases

}
