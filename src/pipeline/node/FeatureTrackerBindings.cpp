#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/FeatureTracker.hpp"


void bind_featuretracker(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<FeatureTrackerProperties> featureTrackerProperties(m, "FeatureTrackerProperties", DOC(dai, FeatureTrackerProperties));
    auto featureTracker = ADD_NODE(FeatureTracker);

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

    // FeatureTracker properties
    featureTrackerProperties
        .def_readwrite("initialConfig", &FeatureTrackerProperties::initialConfig, DOC(dai, FeatureTrackerProperties, initialConfig))
        .def_readwrite("numShaves", &FeatureTrackerProperties::numShaves, DOC(dai, FeatureTrackerProperties, numShaves))
        .def_readwrite("numMemorySlices", &FeatureTrackerProperties::numMemorySlices, DOC(dai, FeatureTrackerProperties, numMemorySlices))
        ;

    // FeatureTracker Node
    featureTracker
        .def_readonly("inputConfig", &FeatureTracker::inputConfig, DOC(dai, node, FeatureTracker, inputConfig))
        .def_readonly("inputImage", &FeatureTracker::inputImage, DOC(dai, node, FeatureTracker, inputImage))
        .def_readonly("outputFeatures", &FeatureTracker::outputFeatures, DOC(dai, node, FeatureTracker, outputFeatures))
        .def_readonly("outputDescriptors", &FeatureTracker::outputDescriptors, DOC(dai, node, FeatureTracker, outputDescriptors))
        .def_readonly("passthroughInputImage", &FeatureTracker::passthroughInputImage, DOC(dai, node, FeatureTracker, passthroughInputImage))
        .def_readonly("initialConfig", &FeatureTracker::initialConfig, DOC(dai, node, FeatureTracker, initialConfig))

        .def("setWaitForConfigInput", [](FeatureTracker& obj, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            obj.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, FeatureTracker, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](FeatureTracker& obj){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return obj.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, FeatureTracker, getWaitForConfigInput))

        .def("setHardwareResources", &FeatureTracker::setHardwareResources, py::arg("numShaves"), py::arg("numMemorySlices"), DOC(dai, node, FeatureTracker, setHardwareResources))
        ;
    daiNodeModule.attr("FeatureTracker").attr("Properties") = featureTrackerProperties;

}
