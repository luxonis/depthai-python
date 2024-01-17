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

    // py::class_<AprilTagConfig, RawBuffer, std::shared_ptr<RawAprilTagConfig>>
    //     rawAprilTagConfig(m, "RawAprilTagConfig", DOC(dai, RawAprilTagConfig));
    py::class_<AprilTagConfig, Buffer, std::shared_ptr<AprilTagConfig>> aprilTagConfig(m, "AprilTagConfig", DOC(dai, AprilTagConfig));
    py::enum_<AprilTagConfig::Family> aprilTagFamily(aprilTagConfig, "Family", DOC(dai, RawAprilTagConfig, Family));
    py::class_<AprilTagConfig::QuadThresholds> quadThresholds(aprilTagConfig, "QuadThresholds", DOC(dai, RawAprilTagConfig, QuadThresholds));

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
        .value("TAG_36H11", AprilTagConfig::Family::TAG_36H11)
        .value("TAG_36H10", AprilTagConfig::Family::TAG_36H10)
        .value("TAG_25H9", AprilTagConfig::Family::TAG_25H9)
        .value("TAG_16H5", AprilTagConfig::Family::TAG_16H5)
        .value("TAG_CIR21H7", AprilTagConfig::Family::TAG_CIR21H7)
        .value("TAG_STAND41H12", AprilTagConfig::Family::TAG_STAND41H12)
        ;

    quadThresholds
        .def(py::init<>())
        .def_readwrite("minClusterPixels", &AprilTagConfig::QuadThresholds::minClusterPixels, DOC(dai, AprilTagConfig, QuadThresholds, minClusterPixels))
        .def_readwrite("maxNmaxima", &AprilTagConfig::QuadThresholds::maxNmaxima, DOC(dai, AprilTagConfig, QuadThresholds, maxNmaxima))
        .def_readwrite("criticalDegree", &AprilTagConfig::QuadThresholds::criticalDegree, DOC(dai, AprilTagConfig, QuadThresholds, criticalDegree))
        .def_readwrite("maxLineFitMse", &AprilTagConfig::QuadThresholds::maxLineFitMse, DOC(dai, AprilTagConfig, QuadThresholds, maxLineFitMse))
        .def_readwrite("minWhiteBlackDiff", &AprilTagConfig::QuadThresholds::minWhiteBlackDiff, DOC(dai, AprilTagConfig, QuadThresholds, minWhiteBlackDiff))
        .def_readwrite("deglitch", &AprilTagConfig::QuadThresholds::deglitch, DOC(dai, AprilTagConfig, QuadThresholds, deglitch))
        ;


    aprilTagConfig
        .def(py::init<>())
        .def_readwrite("family", &AprilTagConfig::family, DOC(dai, AprilTagConfig, family))
        .def_readwrite("quadDecimate", &AprilTagConfig::quadDecimate, DOC(dai, AprilTagConfig, quadDecimate))
        .def_readwrite("quadSigma", &AprilTagConfig::quadSigma, DOC(dai, AprilTagConfig, quadSigma))
        .def_readwrite("refineEdges", &AprilTagConfig::refineEdges, DOC(dai, AprilTagConfig, refineEdges))
        .def_readwrite("decodeSharpening", &AprilTagConfig::decodeSharpening, DOC(dai, AprilTagConfig, decodeSharpening))
        .def_readwrite("maxHammingDistance", &AprilTagConfig::maxHammingDistance, DOC(dai, AprilTagConfig, maxHammingDistance))
        .def_readwrite("quadThresholds", &AprilTagConfig::quadThresholds, DOC(dai, AprilTagConfig, quadThresholds))
        ;

    // Message
    aprilTagConfig
        .def(py::init<>())
        .def("setFamily", &AprilTagConfig::setFamily, py::arg("family"), DOC(dai, AprilTagConfig, setFamily))
        // .def("set", &AprilTagConfig::set, DOC(dai, AprilTagConfig, set))
        // .def("get", &AprilTagConfig::get, DOC(dai, AprilTagConfig, get))
        ;
    m.attr("AprilTagConfig").attr("Family") = m.attr("AprilTagConfig").attr("Family");
    m.attr("AprilTagConfig").attr("QuadThresholds") = m.attr("AprilTagConfig").attr("QuadThresholds");

}
