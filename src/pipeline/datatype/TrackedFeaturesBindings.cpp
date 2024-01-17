#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/TrackedFeatures.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_trackedfeatures(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawTrackedFeatures, RawBuffer, std::shared_ptr<RawTrackedFeatures>> rawTrackedFeatures(m, "RawTrackedFeatures", DOC(dai, RawTrackedFeatures));
    py::class_<TrackedFeature> trackedFeature(m, "TrackedFeature", DOC(dai, TrackedFeature));
    py::class_<TrackedFeatures, Buffer, std::shared_ptr<TrackedFeatures>> trackedFeatures(m, "TrackedFeatures", DOC(dai, TrackedFeatures));

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
    // rawTrackedFeatures
    //     .def(py::init<>())
    //     .def_readwrite("trackedFeatures", &RawTrackedFeatures::trackedFeatures)
    //     ;

    trackedFeature
        .def(py::init<>())
        .def_readwrite("position", &TrackedFeature::position, DOC(dai, TrackedFeature, position))
        .def_readwrite("id", &TrackedFeature::id, DOC(dai, TrackedFeature, id))
        .def_readwrite("age", &TrackedFeature::age, DOC(dai, TrackedFeature, age))
        .def_readwrite("harrisScore", &TrackedFeature::harrisScore, DOC(dai, TrackedFeature, harrisScore))
        .def_readwrite("trackingError", &TrackedFeature::trackingError, DOC(dai, TrackedFeature, trackingError))
        ;


    // Message
    trackedFeatures
        .def(py::init<>())
        .def_property("trackedFeatures", [](TrackedFeatures& feat) { return &feat.trackedFeatures; }, [](TrackedFeatures& feat, std::vector<TrackedFeature> val) { feat.trackedFeatures = val; }, DOC(dai, TrackedFeatures, trackedFeatures))
        .def("getTimestamp", &TrackedFeatures::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &TrackedFeatures::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &TrackedFeatures::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &TrackedFeatures::setTimestamp, DOC(dai, TrackedFeatures, setTimestamp))
        .def("setTimestampDevice", &TrackedFeatures::setTimestampDevice, DOC(dai, TrackedFeatures, setTimestampDevice))
        .def("setSequenceNum", &TrackedFeatures::setSequenceNum, DOC(dai, TrackedFeatures, setSequenceNum))
        ;

}
