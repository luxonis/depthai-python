#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/FeatureTrackerConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_featuretrackerconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawFeatureTrackerConfig, RawBuffer, std::shared_ptr<RawFeatureTrackerConfig>> rawFeatureTrackerConfig(m, "RawFeatureTrackerConfig", DOC(dai, RawFeatureTrackerConfig));
    py::class_<RawFeatureTrackerConfig::CornerDetector> cornerDetector(rawFeatureTrackerConfig, "CornerDetector", DOC(dai, RawFeatureTrackerConfig, CornerDetector));
    py::enum_<RawFeatureTrackerConfig::CornerDetector::Type> cornerDetectorType(cornerDetector, "Type", DOC(dai, RawFeatureTrackerConfig, CornerDetector, Type));
    py::class_<RawFeatureTrackerConfig::CornerDetector::Thresholds> cornerDetectorThresholds(cornerDetector, "Thresholds", DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds));
    py::class_<RawFeatureTrackerConfig::MotionEstimator> motionEstimator(rawFeatureTrackerConfig, "MotionEstimator", DOC(dai, RawFeatureTrackerConfig, MotionEstimator));
    py::enum_<RawFeatureTrackerConfig::MotionEstimator::Type> motionEstimatorType(motionEstimator, "Type", DOC(dai, RawFeatureTrackerConfig, MotionEstimator, Type));
    py::class_<RawFeatureTrackerConfig::MotionEstimator::OpticalFlow> motionEstimatorOpticalFlow(motionEstimator, "OpticalFlow", DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow));
    py::class_<RawFeatureTrackerConfig::FeatureMaintainer> featureMaintainer(rawFeatureTrackerConfig, "FeatureMaintainer", DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer));
    py::class_<FeatureTrackerConfig, Buffer, std::shared_ptr<FeatureTrackerConfig>> featureTrackerConfig(m, "FeatureTrackerConfig", DOC(dai, FeatureTrackerConfig));

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
    rawFeatureTrackerConfig
        .def(py::init<>())
        .def_readwrite("cornerDetector", &RawFeatureTrackerConfig::cornerDetector, DOC(dai, RawFeatureTrackerConfig, cornerDetector))
        .def_readwrite("motionEstimator", &RawFeatureTrackerConfig::motionEstimator, DOC(dai, RawFeatureTrackerConfig, motionEstimator))
        .def_readwrite("featureMaintainer", &RawFeatureTrackerConfig::featureMaintainer, DOC(dai, RawFeatureTrackerConfig, featureMaintainer))
        ;

    cornerDetectorType
        .value("HARRIS", RawFeatureTrackerConfig::CornerDetector::Type::HARRIS)
        .value("SHI_THOMASI", RawFeatureTrackerConfig::CornerDetector::Type::SHI_THOMASI)
    ;

    cornerDetectorThresholds
        .def(py::init<>())
        .def_readwrite("initialValue", &RawFeatureTrackerConfig::CornerDetector::Thresholds::initialValue, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, initialValue))
        .def_readwrite("min", &RawFeatureTrackerConfig::CornerDetector::Thresholds::min, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, min))
        .def_readwrite("max", &RawFeatureTrackerConfig::CornerDetector::Thresholds::max, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, max))
        .def_readwrite("decreaseFactor", &RawFeatureTrackerConfig::CornerDetector::Thresholds::decreaseFactor, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, decreaseFactor))
        .def_readwrite("increaseFactor", &RawFeatureTrackerConfig::CornerDetector::Thresholds::increaseFactor, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, increaseFactor))
        ;

    cornerDetector
        .def(py::init<>())
        .def_readwrite("type", &RawFeatureTrackerConfig::CornerDetector::type, DOC(dai, RawFeatureTrackerConfig, CornerDetector, type))
        .def_readwrite("cellGridDimension", &RawFeatureTrackerConfig::CornerDetector::cellGridDimension, DOC(dai, RawFeatureTrackerConfig, CornerDetector, cellGridDimension))
        .def_readwrite("numTargetFeatures", &RawFeatureTrackerConfig::CornerDetector::numTargetFeatures, DOC(dai, RawFeatureTrackerConfig, CornerDetector, numTargetFeatures))
        .def_readwrite("numMaxFeatures", &RawFeatureTrackerConfig::CornerDetector::numMaxFeatures, DOC(dai, RawFeatureTrackerConfig, CornerDetector, numMaxFeatures))
        .def_readwrite("enableSobel", &RawFeatureTrackerConfig::CornerDetector::enableSobel, DOC(dai, RawFeatureTrackerConfig, CornerDetector, enableSobel))
        .def_readwrite("enableSorting", &RawFeatureTrackerConfig::CornerDetector::enableSorting, DOC(dai, RawFeatureTrackerConfig, CornerDetector, enableSorting))
        .def_readwrite("thresholds", &RawFeatureTrackerConfig::CornerDetector::thresholds, DOC(dai, RawFeatureTrackerConfig, CornerDetector, thresholds))
        ;

    motionEstimatorType
        .value("LUCAS_KANADE_OPTICAL_FLOW", RawFeatureTrackerConfig::MotionEstimator::Type::LUCAS_KANADE_OPTICAL_FLOW)
        .value("HW_MOTION_ESTIMATION", RawFeatureTrackerConfig::MotionEstimator::Type::HW_MOTION_ESTIMATION)
    ;

    motionEstimatorOpticalFlow
        .def(py::init<>())
        .def_readwrite("pyramidLevels", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::pyramidLevels, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, pyramidLevels))
        .def_readwrite("searchWindowWidth", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowWidth, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowWidth))
        .def_readwrite("searchWindowHeight", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowHeight, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowHeight))
        .def_readwrite("epsilon", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::epsilon, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, epsilon))
        .def_readwrite("maxIterations", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::maxIterations, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, maxIterations))
        ;

    motionEstimator
        .def(py::init<>())
        .def_readwrite("enable", &RawFeatureTrackerConfig::MotionEstimator::enable, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, enable))
        .def_readwrite("type", &RawFeatureTrackerConfig::MotionEstimator::type, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, type))
        .def_readwrite("opticalFlow", &RawFeatureTrackerConfig::MotionEstimator::opticalFlow, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, opticalFlow))
        ;

    featureMaintainer
        .def(py::init<>())
        .def_readwrite("enable", &RawFeatureTrackerConfig::FeatureMaintainer::enable, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, enable))
        .def_readwrite("minimumDistanceBetweenFeatures", &RawFeatureTrackerConfig::FeatureMaintainer::minimumDistanceBetweenFeatures, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, minimumDistanceBetweenFeatures))
        .def_readwrite("lostFeatureErrorThreshold", &RawFeatureTrackerConfig::FeatureMaintainer::lostFeatureErrorThreshold, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, lostFeatureErrorThreshold))
        .def_readwrite("trackedFeatureThreshold", &RawFeatureTrackerConfig::FeatureMaintainer::trackedFeatureThreshold, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, trackedFeatureThreshold))
        ;

    // Message
    featureTrackerConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawFeatureTrackerConfig>>())

        .def("setCornerDetector", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::CornerDetector::Type)>(&FeatureTrackerConfig::setCornerDetector), py::arg("cornerDetector"), DOC(dai, FeatureTrackerConfig, setCornerDetector))
        .def("setCornerDetector", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::CornerDetector)>(&FeatureTrackerConfig::setCornerDetector), py::arg("config"), DOC(dai, FeatureTrackerConfig, setCornerDetector, 2))
        .def("setMotionEstimator", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(bool)>(&FeatureTrackerConfig::setMotionEstimator), py::arg("enable"), DOC(dai, FeatureTrackerConfig, setMotionEstimator))
        .def("setMotionEstimator", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::MotionEstimator)>(&FeatureTrackerConfig::setMotionEstimator), py::arg("config"), DOC(dai, FeatureTrackerConfig, setMotionEstimator, 2))
        .def("setOpticalFlow", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)()>(&FeatureTrackerConfig::setOpticalFlow), DOC(dai, FeatureTrackerConfig, setOpticalFlow))
        .def("setOpticalFlow", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::MotionEstimator::OpticalFlow)>(&FeatureTrackerConfig::setOpticalFlow), py::arg("config"), DOC(dai, FeatureTrackerConfig, setOpticalFlow, 2))
        .def("setHwMotionEstimation", &FeatureTrackerConfig::setHwMotionEstimation, DOC(dai, FeatureTrackerConfig, setHwMotionEstimation))
        .def("setFeatureMaintainer", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(bool)>(&FeatureTrackerConfig::setFeatureMaintainer), py::arg("enable"), DOC(dai, FeatureTrackerConfig, setFeatureMaintainer))
        .def("setFeatureMaintainer", static_cast<FeatureTrackerConfig&(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::FeatureMaintainer)>(&FeatureTrackerConfig::setFeatureMaintainer), py::arg("config"), DOC(dai, FeatureTrackerConfig, setFeatureMaintainer, 2))
        .def("setNumTargetFeatures", &FeatureTrackerConfig::setNumTargetFeatures, py::arg("numTargetFeatures"), DOC(dai, FeatureTrackerConfig, setNumTargetFeatures))

        .def("set", &FeatureTrackerConfig::set, py::arg("config"), DOC(dai, FeatureTrackerConfig, set))
        .def("get", &FeatureTrackerConfig::get, DOC(dai, FeatureTrackerConfig, get))
        ;

    // add aliases
    m.attr("FeatureTrackerConfig").attr("CornerDetector") = m.attr("RawFeatureTrackerConfig").attr("CornerDetector");
    m.attr("FeatureTrackerConfig").attr("MotionEstimator") = m.attr("RawFeatureTrackerConfig").attr("MotionEstimator");
    m.attr("FeatureTrackerConfig").attr("FeatureMaintainer") = m.attr("RawFeatureTrackerConfig").attr("FeatureMaintainer");

}
