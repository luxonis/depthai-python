#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/VisionHealthMetrics.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_visionhealthmetrics(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawVisionHealthMetrics, RawBuffer, std::shared_ptr<RawVisionHealthMetrics>> _RawVisionHealthMetrics(m, "RawVisionHealthMetrics", DOC(dai, RawVisionHealthMetrics));
    py::class_<VisionHealthMetric> _VisionHealthMetric(m, "VisionHealthMetric", DOC(dai, VisionHealthMetric));
    py::class_<VisionHealthMetrics, Buffer, std::shared_ptr<VisionHealthMetrics>> _VisionHealthMetrics(m, "VisionHealthMetrics", DOC(dai, VisionHealthMetrics));

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
    _RawVisionHealthMetrics
        .def(py::init<>())
        .def_readwrite("visionHealthMetrics", &RawVisionHealthMetrics::visionHealthMetrics)
        ;

    _VisionHealthMetric
        .def(py::init<>())
        .def_readwrite("value", &VisionHealthMetric::value, DOC(dai, VisionHealthMetric, value))
        ;

    // Message
    _VisionHealthMetrics
        .def(py::init<>())
        .def_property("visionHealthMetrics", [](VisionHealthMetrics& feat) { return &feat.visionHealthMetrics; }, [](VisionHealthMetrics& feat, std::unordered_map<VisionHealthMetricTypes, VisionHealthMetric> val) { feat.visionHealthMetrics = val; }, DOC(dai, VisionHealthMetrics, visionHealthMetrics))
        .def("getTimestamp", &VisionHealthMetrics::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &VisionHealthMetrics::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &VisionHealthMetrics::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &VisionHealthMetrics::setTimestamp, DOC(dai, VisionHealthMetrics, setTimestamp))
        .def("setTimestampDevice", &VisionHealthMetrics::setTimestampDevice, DOC(dai, VisionHealthMetrics, setTimestampDevice))
        .def("setSequenceNum", &VisionHealthMetrics::setSequenceNum, DOC(dai, VisionHealthMetrics, setSequenceNum))
        ;

}
