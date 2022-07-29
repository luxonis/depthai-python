#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/AprilTagConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_apriltagconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawAprilTagConfig, RawBuffer, std::shared_ptr<RawAprilTagConfig>> rawAprilTagConfig(m, "RawAprilTagConfig", DOC(dai, RawAprilTagConfig));
    py::enum_<RawAprilTagConfig::Family> aprilTagFamily(rawAprilTagConfig, "Family", DOC(dai, RawAprilTagConfig, Family));
    py::class_<RawAprilTagConfig::QuadThresholds> quadThresholds(rawAprilTagConfig, "QuadThresholds", DOC(dai, RawAprilTagConfig, QuadThresholds));
    py::class_<AprilTagConfig, Buffer, std::shared_ptr<AprilTagConfig>> aprilTagConfig(m, "AprilTagConfig", DOC(dai, AprilTagConfig));

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
    aprilTagFamily
        .value("TAG_36H11", RawAprilTagConfig::Family::TAG_36H11)
        .value("TAG_36H10", RawAprilTagConfig::Family::TAG_36H10)
        .value("TAG_25H9", RawAprilTagConfig::Family::TAG_25H9)
        .value("TAG_16H5", RawAprilTagConfig::Family::TAG_16H5)
        .value("TAG_CIR21H7", RawAprilTagConfig::Family::TAG_CIR21H7)
        .value("TAG_STAND41H12", RawAprilTagConfig::Family::TAG_STAND41H12)
        ;

    quadThresholds
        .def(py::init<>())
        .def_readwrite("minClusterPixels", &RawAprilTagConfig::QuadThresholds::minClusterPixels, DOC(dai, RawAprilTagConfig, QuadThresholds, minClusterPixels))
        .def_readwrite("maxNmaxima", &RawAprilTagConfig::QuadThresholds::maxNmaxima, DOC(dai, RawAprilTagConfig, QuadThresholds, maxNmaxima))
        .def_readwrite("criticalDegree", &RawAprilTagConfig::QuadThresholds::criticalDegree, DOC(dai, RawAprilTagConfig, QuadThresholds, criticalDegree))
        .def_readwrite("maxLineFitMse", &RawAprilTagConfig::QuadThresholds::maxLineFitMse, DOC(dai, RawAprilTagConfig, QuadThresholds, maxLineFitMse))
        .def_readwrite("minWhiteBlackDiff", &RawAprilTagConfig::QuadThresholds::minWhiteBlackDiff, DOC(dai, RawAprilTagConfig, QuadThresholds, minWhiteBlackDiff))
        .def_readwrite("deglitch", &RawAprilTagConfig::QuadThresholds::deglitch, DOC(dai, RawAprilTagConfig, QuadThresholds, deglitch))
        ;


    rawAprilTagConfig
        .def(py::init<>())
        .def_readwrite("family", &RawAprilTagConfig::family, DOC(dai, RawAprilTagConfig, family))
        .def_readwrite("quadDecimate", &RawAprilTagConfig::quadDecimate, DOC(dai, RawAprilTagConfig, quadDecimate))
        .def_readwrite("quadSigma", &RawAprilTagConfig::quadSigma, DOC(dai, RawAprilTagConfig, quadSigma))
        .def_readwrite("refineEdges", &RawAprilTagConfig::refineEdges, DOC(dai, RawAprilTagConfig, refineEdges))
        .def_readwrite("decodeSharpening", &RawAprilTagConfig::decodeSharpening, DOC(dai, RawAprilTagConfig, decodeSharpening))
        .def_readwrite("maxHammingDistance", &RawAprilTagConfig::maxHammingDistance, DOC(dai, RawAprilTagConfig, maxHammingDistance))
        .def_readwrite("quadThresholds", &RawAprilTagConfig::quadThresholds, DOC(dai, RawAprilTagConfig, quadThresholds))
        ;

    // Message
    aprilTagConfig
        .def(py::init<>())
        .def("setFamily", &AprilTagConfig::setFamily, py::arg("family"), DOC(dai, AprilTagConfig, setFamily))
        .def("set", &AprilTagConfig::set, DOC(dai, AprilTagConfig, set))
        .def("get", &AprilTagConfig::get, DOC(dai, AprilTagConfig, get))
        ;
    m.attr("AprilTagConfig").attr("Family") = m.attr("RawAprilTagConfig").attr("Family");
    m.attr("AprilTagConfig").attr("QuadThresholds") = m.attr("RawAprilTagConfig").attr("QuadThresholds");

}
