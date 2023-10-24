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
        .def_readwrite("minDist", &RawPointCloudConfig::minDist, DOC(dai, RawPointCloudConfig, minDist))
        .def_readwrite("maxDist", &RawPointCloudConfig::maxDist, DOC(dai, RawPointCloudConfig, maxDist))
        .def_readwrite("minAngle", &RawPointCloudConfig::minAngle, DOC(dai, RawPointCloudConfig, minAngle))
        .def_readwrite("maxAngle", &RawPointCloudConfig::maxAngle, DOC(dai, RawPointCloudConfig, maxAngle))
        .def_readwrite("minHeight", &RawPointCloudConfig::minHeight, DOC(dai, RawPointCloudConfig, minHeight))
        .def_readwrite("maxHeight", &RawPointCloudConfig::maxHeight, DOC(dai, RawPointCloudConfig, maxHeight))
        .def_readwrite("numRings", &RawPointCloudConfig::numRings, DOC(dai, RawPointCloudConfig, numRings))
        .def_readwrite("numSectors", &RawPointCloudConfig::numSectors, DOC(dai, RawPointCloudConfig, numSectors))
        .def_readwrite("numVoxels", &RawPointCloudConfig::numVoxels, DOC(dai, RawPointCloudConfig, numVoxels))
        .def_readwrite("occupancyThreshold", &RawPointCloudConfig::occupancyThreshold, DOC(dai, RawPointCloudConfig, occupancyThreshold))
        .def_readwrite("transformationMatrix", &RawPointCloudConfig::transformationMatrix, DOC(dai, RawPointCloudConfig, transformationMatrix))
        .def_readwrite("scale", &RawPointCloudConfig::scale, DOC(dai, RawPointCloudConfig, scale))
        .def_readwrite("useFastMath", &RawPointCloudConfig::useFastMath, DOC(dai, RawPointCloudConfig, useFastMath))
        .def_readwrite("occupancy3d", &RawPointCloudConfig::occupancy3d, DOC(dai, RawPointCloudConfig, occupancy3d))
        ;

    // Message
    config
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawPointCloudConfig>>())
        
        .def("set", &PointCloudConfig::set, py::arg("config"), DOC(dai, PointCloudConfig, set))
        .def("get", &PointCloudConfig::get, DOC(dai, PointCloudConfig, get))
        ;

    // add aliases

}
