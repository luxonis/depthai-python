#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/DepthAlign.hpp"

void bind_depthalign(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<DepthAlignProperties> properties(m, "DepthAlignProperties", DOC(dai, DepthAlignProperties));
    auto node = ADD_NODE(DepthAlign);

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
    properties
        .def_readwrite("initialConfig", &DepthAlignProperties::initialConfig, DOC(dai, DepthAlignProperties, initialConfig))
        .def_readwrite("alignFrom", &DepthAlignProperties::alignFrom, DOC(dai, DepthAlignProperties, alignFrom))
        .def_readwrite("alignTo", &DepthAlignProperties::alignTo, DOC(dai, DepthAlignProperties, alignTo))
        .def_readwrite("alignWidth", &DepthAlignProperties::alignWidth, DOC(dai, DepthAlignProperties, alignWidth))
        .def_readwrite("alignHeight", &DepthAlignProperties::alignHeight, DOC(dai, DepthAlignProperties, alignHeight))
        .def_readwrite("downscaleSource", &DepthAlignProperties::downscaleSource, DOC(dai, DepthAlignProperties, downscaleSource))
    ;
    // Node
    node
        .def_readonly("inputConfig", &DepthAlign::inputConfig, DOC(dai, node, DepthAlign, inputConfig))
        .def_readonly("inputDepth", &DepthAlign::inputDepth, DOC(dai, node, DepthAlign, inputDepth))
        .def_readonly("outputAlignedDepth", &DepthAlign::outputAlignedDepth, DOC(dai, node, DepthAlign, outputAlignedDepth))
        .def_readonly("passthroughDepth", &DepthAlign::passthroughDepth, DOC(dai, node, DepthAlign, passthroughDepth))
        .def_readonly("initialConfig", &DepthAlign::initialConfig, DOC(dai, node, DepthAlign, initialConfig))
        .def("setAlignTo", &DepthAlign::setAlignTo, DOC(dai, node, DepthAlign, setAlignTo))
        .def("setOutputSize", &DepthAlign::setOutputSize, DOC(dai, node, DepthAlign, setOutputSize))
        ;
	daiNodeModule.attr("DepthAlign").attr("Properties") = properties;

}
