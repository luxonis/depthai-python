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
    py::class_<AbsoluteVisionHealthMetric> _AbsoluteVisionHealthMetric(m, "AbsoluteVisionHealthMetric", DOC(dai, AbsoluteVisionHealthMetric));
    py::class_<RelativeVisionHealthMetric> _RelativeVisionHealthMetric(m, "RelativeVisionHealthMetric", DOC(dai, RelativeVisionHealthMetric));
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
        .def_readwrite("absoluteVisionHealthMetrics", &RawVisionHealthMetrics::absoluteVisionHealthMetrics)
        .def_readwrite("relativeVisionHealthMetrics", &RawVisionHealthMetrics::relativeVisionHealthMetrics)
        ;

    _AbsoluteVisionHealthMetric
        .def(py::init<>())
        .def_readwrite("value", &AbsoluteVisionHealthMetric::value, DOC(dai, AbsoluteVisionHealthMetric, value))
        ;

    _RelativeVisionHealthMetric
        .def(py::init<>())
        .def_readwrite("value", &RelativeVisionHealthMetric::value, DOC(dai, RelativeVisionHealthMetric, value))
        .def_readwrite("mean", &RelativeVisionHealthMetric::mean, DOC(dai, RelativeVisionHealthMetric, mean))
        .def_readwrite("variance", &RelativeVisionHealthMetric::variance, DOC(dai, RelativeVisionHealthMetric, variance))
        ;


    // Message
    _VisionHealthMetrics
        .def(py::init<>())
        .def_property("absoluteVisionHealthMetrics", [](VisionHealthMetrics& feat) { return &feat.absoluteVisionHealthMetrics; }, [](VisionHealthMetrics& feat, std::unordered_map<VisionHealthMetricTypes, AbsoluteVisionHealthMetric> val) { feat.absoluteVisionHealthMetrics = val; }, DOC(dai, VisionHealthMetrics, absoluteVisionHealthMetrics))
        .def_property("relativeVisionHealthMetrics", [](VisionHealthMetrics& feat) { return &feat.relativeVisionHealthMetrics; }, [](VisionHealthMetrics& feat, std::unordered_map<VisionHealthMetricTypes, RelativeVisionHealthMetric> val) { feat.relativeVisionHealthMetrics = val; }, DOC(dai, VisionHealthMetrics, relativeVisionHealthMetrics))
        .def("getTimestamp", &VisionHealthMetrics::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &VisionHealthMetrics::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &VisionHealthMetrics::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &VisionHealthMetrics::setTimestamp, DOC(dai, VisionHealthMetrics, setTimestamp))
        .def("setTimestampDevice", &VisionHealthMetrics::setTimestampDevice, DOC(dai, VisionHealthMetrics, setTimestampDevice))
        .def("setSequenceNum", &VisionHealthMetrics::setSequenceNum, DOC(dai, VisionHealthMetrics, setSequenceNum))
        ;

}
