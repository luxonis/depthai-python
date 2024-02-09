#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/SpatialImgDetections.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_spatialimgdetections(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawSpatialImgDetections, RawBuffer, std::shared_ptr<RawSpatialImgDetections>> rawSpatialImgDetections(m, "RawSpatialImgDetections", DOC(dai, RawSpatialImgDetections));
    py::class_<SpatialImgDetection, ImgDetection> spatialImgDetection(m, "SpatialImgDetection", DOC(dai, SpatialImgDetection));
    py::class_<SpatialImgDetections, Buffer, std::shared_ptr<SpatialImgDetections>> spatialImgDetections(m, "SpatialImgDetections", DOC(dai, SpatialImgDetections));

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
    spatialImgDetection
        .def(py::init<>())
        .def_readwrite("spatialCoordinates", &SpatialImgDetection::spatialCoordinates)
        .def_readwrite("boundingBoxMapping", &SpatialImgDetection::boundingBoxMapping)
        ;

    rawSpatialImgDetections
        .def(py::init<>())
        .def_readwrite("detections", &RawSpatialImgDetections::detections)
        .def_property("ts",
            [](const RawSpatialImgDetections& o){
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0;
                return ts;
            },
            [](RawSpatialImgDetections& o, double ts){
                o.ts.sec = ts;
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;
            }
        )
        .def_property("tsDevice",
            [](const RawSpatialImgDetections& o){
                double ts = o.tsDevice.sec + o.tsDevice.nsec / 1000000000.0;
                return ts;
            },
            [](RawSpatialImgDetections& o, double ts){
                o.tsDevice.sec = ts;
                o.tsDevice.nsec = (ts - o.tsDevice.sec) * 1000000000.0;
            }
        )
        .def_readwrite("sequenceNum", &RawSpatialImgDetections::sequenceNum)
        ;

    // Message
    spatialImgDetections
        .def(py::init<>())
        .def_property("detections", [](SpatialImgDetections& det) { return &det.detections; }, [](SpatialImgDetections& det, std::vector<SpatialImgDetection> val) { det.detections = val; })
        .def("getTimestamp", &SpatialImgDetections::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &SpatialImgDetections::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &SpatialImgDetections::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &SpatialImgDetections::setTimestamp, DOC(dai, SpatialImgDetections, setTimestamp))
        .def("setTimestampDevice", &SpatialImgDetections::setTimestampDevice, DOC(dai, SpatialImgDetections, setTimestampDevice))
        .def("setSequenceNum", &SpatialImgDetections::setSequenceNum, DOC(dai, SpatialImgDetections, setSequenceNum))
        ;



}
