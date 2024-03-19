#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/Tracklets.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_tracklets(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawTracklets, RawBuffer, std::shared_ptr<RawTracklets>> rawTacklets(m, "RawTracklets", DOC(dai, RawTracklets));
    py::class_<Tracklet> tracklet(m, "Tracklet", DOC(dai, Tracklet));
    py::enum_<Tracklet::TrackingStatus> trackletTrackingStatus(tracklet, "TrackingStatus", DOC(dai, Tracklet, TrackingStatus));
    py::class_<Tracklets, Buffer, std::shared_ptr<Tracklets>> tracklets(m, "Tracklets", DOC(dai, Tracklets));

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
    tracklet
        .def(py::init<>())
        .def_readwrite("roi", &Tracklet::roi)
        .def_readwrite("id", &Tracklet::id)
        .def_readwrite("label", &Tracklet::label)
        .def_readwrite("age", &Tracklet::age)
        .def_readwrite("status", &Tracklet::status)
        .def_readwrite("srcImgDetection", &Tracklet::srcImgDetection)
        .def_readwrite("spatialCoordinates", &Tracklet::spatialCoordinates)
        ;

    trackletTrackingStatus
        .value("NEW", Tracklet::TrackingStatus::NEW)
        .value("TRACKED", Tracklet::TrackingStatus::TRACKED)
        .value("LOST", Tracklet::TrackingStatus::LOST)
        .value("REMOVED", Tracklet::TrackingStatus::REMOVED)
        ;

    // rawTacklets
    //     .def(py::init<>())
    //     .def_readwrite("tracklets", &RawTracklets::tracklets)
    //     ;

    // Message
    tracklets
        .def(py::init<>())
        .def_property("tracklets", [](Tracklets& track) { return &track.tracklets; }, [](Tracklets& track, std::vector<Tracklet> val) { track.tracklets = val; }, DOC(dai, Tracklets, tracklets))
        .def("getTimestamp", &Tracklets::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &Tracklets::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &Tracklets::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &Tracklets::setTimestamp, DOC(dai, Tracklets, setTimestamp))
        .def("setTimestampDevice", &Tracklets::setTimestampDevice, DOC(dai, Tracklets, setTimestampDevice))
        .def("setSequenceNum", &Tracklets::setSequenceNum, DOC(dai, Tracklets, setSequenceNum))
        ;

}
