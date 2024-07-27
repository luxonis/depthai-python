#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/VisionHealth.hpp"


void bind_visionhealth(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<VisionHealthProperties> properties(m, "VisionHealthProperties", DOC(dai, VisionHealthProperties));
    auto featureTracker = ADD_NODE(VisionHealth);

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

    // VisionHealth properties
    properties
        .def_readwrite("initialConfig", &VisionHealthProperties::initialConfig, DOC(dai, VisionHealthProperties, initialConfig))
        .def_readwrite("bootThreshold", &VisionHealthProperties::bootThreshold, DOC(dai, VisionHealthProperties, bootThreshold))
        .def_readwrite("checkThreshold", &VisionHealthProperties::checkThreshold, DOC(dai, VisionHealthProperties, checkThreshold))
        ;

    // VisionHealth Node
    featureTracker
        .def_readonly("inputImage", &VisionHealth::inputImage, DOC(dai, node, VisionHealth, inputImage))
        .def_readonly("outputMetrics", &VisionHealth::outputMetrics, DOC(dai, node, VisionHealth, outputMetrics))
        .def_readonly("passthroughInputImage", &VisionHealth::passthroughInputImage, DOC(dai, node, VisionHealth, passthroughInputImage))
        .def_readonly("initialConfig", &VisionHealth::initialConfig, DOC(dai, node, VisionHealth, initialConfig))
        ;
    daiNodeModule.attr("VisionHealth").attr("Properties") = properties;

}
