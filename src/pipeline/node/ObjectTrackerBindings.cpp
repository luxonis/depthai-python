#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/ObjectTracker.hpp"

void bind_objecttracker(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::enum_<TrackerType> trackerType(m, "TrackerType");
    py::enum_<TrackerIdAssignmentPolicy> trackerIdAssignmentPolicy(m, "TrackerIdAssignmentPolicy");
    py::class_<ObjectTrackerProperties, std::shared_ptr<ObjectTrackerProperties>> objectTrackerProperties(m, "ObjectTrackerProperties", DOC(dai, ObjectTrackerProperties));
    auto objectTracker = ADD_NODE(ObjectTracker);

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

    // Properties
    trackerType
        .value("SHORT_TERM_KCF", TrackerType::SHORT_TERM_KCF, DOC(dai, TrackerType, SHORT_TERM_KCF))
        .value("SHORT_TERM_IMAGELESS", TrackerType::SHORT_TERM_IMAGELESS, DOC(dai, TrackerType, SHORT_TERM_IMAGELESS))
        .value("ZERO_TERM_IMAGELESS", TrackerType::ZERO_TERM_IMAGELESS, DOC(dai, TrackerType, ZERO_TERM_IMAGELESS))
        .value("ZERO_TERM_COLOR_HISTOGRAM", TrackerType::ZERO_TERM_COLOR_HISTOGRAM, DOC(dai, TrackerType, ZERO_TERM_COLOR_HISTOGRAM))
    ;

    trackerIdAssignmentPolicy
        .value("UNIQUE_ID", TrackerIdAssignmentPolicy::UNIQUE_ID)
        .value("SMALLEST_ID", TrackerIdAssignmentPolicy::SMALLEST_ID)
    ;

    objectTrackerProperties
        .def_readwrite("trackerThreshold", &ObjectTrackerProperties::trackerThreshold, DOC(dai, ObjectTrackerProperties, trackerThreshold))
        .def_readwrite("maxObjectsToTrack", &ObjectTrackerProperties::maxObjectsToTrack, DOC(dai, ObjectTrackerProperties, maxObjectsToTrack))
        .def_readwrite("detectionLabelsToTrack", &ObjectTrackerProperties::detectionLabelsToTrack, DOC(dai, ObjectTrackerProperties, detectionLabelsToTrack))
        .def_readwrite("trackerType", &ObjectTrackerProperties::trackerType, DOC(dai, ObjectTrackerProperties, trackerType))
        .def_readwrite("trackerIdAssignmentPolicy", &ObjectTrackerProperties::trackerIdAssignmentPolicy, DOC(dai, ObjectTrackerProperties, trackerIdAssignmentPolicy))
        .def_readwrite("trackingPerClass", &ObjectTrackerProperties::trackingPerClass, DOC(dai, ObjectTrackerProperties, trackingPerClass))
        .def_readwrite("occlusionRatioThreshold", &ObjectTrackerProperties::occlusionRatioThreshold, DOC(dai, ObjectTrackerProperties, occlusionRatioThreshold))
        .def_readwrite("trackingPerClass", &ObjectTrackerProperties::trackingPerClass, DOC(dai, ObjectTrackerProperties, trackingPerClass))
        ;

    // Node
    objectTracker
        .def_readonly("inputTrackerFrame", &ObjectTracker::inputTrackerFrame, DOC(dai, node, ObjectTracker, inputTrackerFrame))
        .def_readonly("inputDetectionFrame", &ObjectTracker::inputDetectionFrame, DOC(dai, node, ObjectTracker, inputDetectionFrame))
        .def_readonly("inputDetections", &ObjectTracker::inputDetections, DOC(dai, node, ObjectTracker, inputDetections))
        .def_readonly("inputConfig", &ObjectTracker::inputConfig, DOC(dai, node, ObjectTracker, inputConfig))
        .def_readonly("out", &ObjectTracker::out, DOC(dai, node, ObjectTracker, out))
        .def_readonly("passthroughTrackerFrame", &ObjectTracker::passthroughTrackerFrame, DOC(dai, node, ObjectTracker, passthroughTrackerFrame))
        .def_readonly("passthroughDetectionFrame", &ObjectTracker::passthroughDetectionFrame, DOC(dai, node, ObjectTracker, passthroughDetectionFrame))
        .def_readonly("passthroughDetections", &ObjectTracker::passthroughDetections, DOC(dai, node, ObjectTracker, passthroughDetections))

        .def("setTrackerThreshold", &ObjectTracker::setTrackerThreshold, py::arg("threshold"), DOC(dai, node, ObjectTracker, setTrackerThreshold))
        .def("setMaxObjectsToTrack", &ObjectTracker::setMaxObjectsToTrack, py::arg("maxObjectsToTrack"), DOC(dai, node, ObjectTracker, setMaxObjectsToTrack))
        .def("setDetectionLabelsToTrack", &ObjectTracker::setDetectionLabelsToTrack, py::arg("labels"), DOC(dai, node, ObjectTracker, setDetectionLabelsToTrack))
        .def("setTrackerType", &ObjectTracker::setTrackerType, py::arg("type"), DOC(dai, node, ObjectTracker, setTrackerType))
        .def("setTrackerIdAssignmentPolicy", &ObjectTracker::setTrackerIdAssignmentPolicy, py::arg("type"), DOC(dai, node, ObjectTracker, setTrackerIdAssignmentPolicy))
        .def("setTrackingPerClass", &ObjectTracker::setTrackingPerClass, py::arg("trackingPerClass"), DOC(dai, node, ObjectTracker, setTrackingPerClass))
        .def("setOcclusionRatioThreshold", &ObjectTracker::setOcclusionRatioThreshold, py::arg("occlusionRatioThreshold"), DOC(dai, node, ObjectTracker, setOcclusionRatioThreshold))
        .def("setTrackletMaxLifespan", &ObjectTracker::setTrackletMaxLifespan, py::arg("lifespan"), DOC(dai, node, ObjectTracker, setTrackletMaxLifespan))
        .def("setTrackletBirthThreshold", &ObjectTracker::setTrackletBirthThreshold, py::arg("threshold"), DOC(dai, node, ObjectTracker, setTrackletBirthThreshold))
        ;
    daiNodeModule.attr("ObjectTracker").attr("Properties") = objectTrackerProperties;

}
