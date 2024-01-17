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

    // py::class_<RawFeatureTrackerConfig, RawBuffer, std::shared_ptr<RawFeatureTrackerConfig>> rawFeatureTrackerConfig(m, "RawFeatureTrackerConfig", DOC(dai, RawFeatureTrackerConfig));
    py::class_<FeatureTrackerConfig, Buffer, std::shared_ptr<FeatureTrackerConfig>> featureTrackerConfig(m, "FeatureTrackerConfig", DOC(dai, FeatureTrackerConfig));
    py::class_<FeatureTrackerConfig::CornerDetector> cornerDetector(featureTrackerConfig, "CornerDetector", DOC(dai, FeatureTrackerConfig, CornerDetector));
    py::enum_<FeatureTrackerConfig::CornerDetector::Type> cornerDetectorType(cornerDetector, "Type", DOC(dai, FeatureTrackerConfig, CornerDetector, Type));
    py::class_<FeatureTrackerConfig::CornerDetector::Thresholds> cornerDetectorThresholds(cornerDetector, "Thresholds", DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds));
    py::class_<FeatureTrackerConfig::MotionEstimator> motionEstimator(featureTrackerConfig, "MotionEstimator", DOC(dai, FeatureTrackerConfig, MotionEstimator));
    py::enum_<FeatureTrackerConfig::MotionEstimator::Type> motionEstimatorType(motionEstimator, "Type", DOC(dai, FeatureTrackerConfig, MotionEstimator, Type));
    py::class_<FeatureTrackerConfig::MotionEstimator::OpticalFlow> motionEstimatorOpticalFlow(motionEstimator, "OpticalFlow", DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow));
    py::class_<FeatureTrackerConfig::FeatureMaintainer> featureMaintainer(featureTrackerConfig, "FeatureMaintainer", DOC(dai, FeatureTrackerConfig, FeatureMaintainer));

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

    // // Metadata / raw
    // rawFeatureTrackerConfig
    //     .def(py::init<>())
    //     .def_readwrite("cornerDetector", &FeatureTrackerConfig::cornerDetector, DOC(dai, FeatureTrackerConfig, cornerDetector))
    //     .def_readwrite("motionEstimator", &FeatureTrackerConfig::motionEstimator, DOC(dai, FeatureTrackerConfig, motionEstimator))
    //     .def_readwrite("featureMaintainer", &FeatureTrackerConfig::featureMaintainer, DOC(dai, FeatureTrackerConfig, featureMaintainer))
    //     ;

    cornerDetectorType
        .value("HARRIS", FeatureTrackerConfig::CornerDetector::Type::HARRIS)
        .value("SHI_THOMASI", FeatureTrackerConfig::CornerDetector::Type::SHI_THOMASI)
    ;

    cornerDetectorThresholds
        .def(py::init<>())
        .def_readwrite("initialValue", &FeatureTrackerConfig::CornerDetector::Thresholds::initialValue, DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds, initialValue))
        .def_readwrite("min", &FeatureTrackerConfig::CornerDetector::Thresholds::min, DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds, min))
        .def_readwrite("max", &FeatureTrackerConfig::CornerDetector::Thresholds::max, DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds, max))
        .def_readwrite("decreaseFactor", &FeatureTrackerConfig::CornerDetector::Thresholds::decreaseFactor, DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds, decreaseFactor))
        .def_readwrite("increaseFactor", &FeatureTrackerConfig::CornerDetector::Thresholds::increaseFactor, DOC(dai, FeatureTrackerConfig, CornerDetector, Thresholds, increaseFactor))
        ;

    cornerDetector
        .def(py::init<>())
        .def_readwrite("type", &FeatureTrackerConfig::CornerDetector::type, DOC(dai, FeatureTrackerConfig, CornerDetector, type))
        .def_readwrite("cellGridDimension", &FeatureTrackerConfig::CornerDetector::cellGridDimension, DOC(dai, FeatureTrackerConfig, CornerDetector, cellGridDimension))
        .def_readwrite("numTargetFeatures", &FeatureTrackerConfig::CornerDetector::numTargetFeatures, DOC(dai, FeatureTrackerConfig, CornerDetector, numTargetFeatures))
        .def_readwrite("numMaxFeatures", &FeatureTrackerConfig::CornerDetector::numMaxFeatures, DOC(dai, FeatureTrackerConfig, CornerDetector, numMaxFeatures))
        .def_readwrite("enableSobel", &FeatureTrackerConfig::CornerDetector::enableSobel, DOC(dai, FeatureTrackerConfig, CornerDetector, enableSobel))
        .def_readwrite("enableSorting", &FeatureTrackerConfig::CornerDetector::enableSorting, DOC(dai, FeatureTrackerConfig, CornerDetector, enableSorting))
        .def_readwrite("thresholds", &FeatureTrackerConfig::CornerDetector::thresholds, DOC(dai, FeatureTrackerConfig, CornerDetector, thresholds))
        ;

    motionEstimatorType
        .value("LUCAS_KANADE_OPTICAL_FLOW", FeatureTrackerConfig::MotionEstimator::Type::LUCAS_KANADE_OPTICAL_FLOW)
        .value("HW_MOTION_ESTIMATION", FeatureTrackerConfig::MotionEstimator::Type::HW_MOTION_ESTIMATION)
    ;

    motionEstimatorOpticalFlow
        .def(py::init<>())
        .def_readwrite("pyramidLevels", &FeatureTrackerConfig::MotionEstimator::OpticalFlow::pyramidLevels, DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow, pyramidLevels))
        .def_readwrite("searchWindowWidth", &FeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowWidth, DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowWidth))
        .def_readwrite("searchWindowHeight", &FeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowHeight, DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowHeight))
        .def_readwrite("epsilon", &FeatureTrackerConfig::MotionEstimator::OpticalFlow::epsilon, DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow, epsilon))
        .def_readwrite("maxIterations", &FeatureTrackerConfig::MotionEstimator::OpticalFlow::maxIterations, DOC(dai, FeatureTrackerConfig, MotionEstimator, OpticalFlow, maxIterations))
        ;

    motionEstimator
        .def(py::init<>())
        .def_readwrite("enable", &FeatureTrackerConfig::MotionEstimator::enable, DOC(dai, FeatureTrackerConfig, MotionEstimator, enable))
        .def_readwrite("type", &FeatureTrackerConfig::MotionEstimator::type, DOC(dai, FeatureTrackerConfig, MotionEstimator, type))
        .def_readwrite("opticalFlow", &FeatureTrackerConfig::MotionEstimator::opticalFlow, DOC(dai, FeatureTrackerConfig, MotionEstimator, opticalFlow))
        ;

    featureMaintainer
        .def(py::init<>())
        .def_readwrite("enable", &FeatureTrackerConfig::FeatureMaintainer::enable, DOC(dai, FeatureTrackerConfig, FeatureMaintainer, enable))
        .def_readwrite("minimumDistanceBetweenFeatures", &FeatureTrackerConfig::FeatureMaintainer::minimumDistanceBetweenFeatures, DOC(dai, FeatureTrackerConfig, FeatureMaintainer, minimumDistanceBetweenFeatures))
        .def_readwrite("lostFeatureErrorThreshold", &FeatureTrackerConfig::FeatureMaintainer::lostFeatureErrorThreshold, DOC(dai, FeatureTrackerConfig, FeatureMaintainer, lostFeatureErrorThreshold))
        .def_readwrite("trackedFeatureThreshold", &FeatureTrackerConfig::FeatureMaintainer::trackedFeatureThreshold, DOC(dai, FeatureTrackerConfig, FeatureMaintainer, trackedFeatureThreshold))
        ;

    // Message
    featureTrackerConfig
        .def(py::init<>())
        // .def(py::init<std::shared_ptr<FeatureTrackerConfig>>())

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

        // .def("set", &FeatureTrackerConfig::set, py::arg("config"), DOC(dai, FeatureTrackerConfig, set))
        // .def("get", &FeatureTrackerConfig::get, DOC(dai, FeatureTrackerConfig, get))
        ;

    // add aliases
    m.attr("FeatureTrackerConfig").attr("CornerDetector") = m.attr("FeatureTrackerConfig").attr("CornerDetector");
    m.attr("FeatureTrackerConfig").attr("MotionEstimator") = m.attr("FeatureTrackerConfig").attr("MotionEstimator");
    m.attr("FeatureTrackerConfig").attr("FeatureMaintainer") = m.attr("FeatureTrackerConfig").attr("FeatureMaintainer");

}
