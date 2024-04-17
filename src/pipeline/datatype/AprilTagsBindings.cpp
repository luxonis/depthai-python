#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/AprilTags.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_apriltags(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<AprilTag> aprilTag(m, "AprilTag", DOC(dai, AprilTag));
    py::class_<AprilTags, Buffer, std::shared_ptr<AprilTags>> aprilTags(m, "AprilTags", DOC(dai, AprilTags));

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
    aprilTag
        .def(py::init<>())
        .def_readwrite("id", &AprilTag::id, DOC(dai, AprilTag, id))
        .def_readwrite("hamming", &AprilTag::hamming, DOC(dai, AprilTag, hamming))
        .def_readwrite("decisionMargin", &AprilTag::decisionMargin, DOC(dai, AprilTag, decisionMargin))
        .def_readwrite("topLeft", &AprilTag::topLeft, DOC(dai, AprilTag, topLeft))
        .def_readwrite("topRight", &AprilTag::topRight, DOC(dai, AprilTag, topRight))
        .def_readwrite("bottomRight", &AprilTag::bottomRight, DOC(dai, AprilTag, bottomRight))
        .def_readwrite("bottomLeft", &AprilTag::bottomLeft, DOC(dai, AprilTag, bottomLeft))
        ;

    // Message
    aprilTags
        .def(py::init<>())
        .def_property("aprilTags", [](AprilTags& det) { return &det.aprilTags; }, [](AprilTags& det, std::vector<AprilTag> val) { det.aprilTags = val; })
        .def("getTimestamp", &AprilTags::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &AprilTags::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &AprilTags::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &AprilTags::setTimestamp, DOC(dai, AprilTags, setTimestamp))
        .def("setTimestampDevice", &AprilTags::setTimestampDevice, DOC(dai, AprilTags, setTimestampDevice))
        .def("setSequenceNum", &AprilTags::setSequenceNum, DOC(dai, AprilTags, setSequenceNum))
        ;

}
