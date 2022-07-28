#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/XLinkIn.hpp"

void bind_xlinkin(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<XLinkInProperties> xlinkInProperties(m, "XLinkInProperties", DOC(dai, XLinkInProperties));
    auto xlinkIn = ADD_NODE(XLinkIn);

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
    xlinkInProperties
        .def_readwrite("streamName", &XLinkInProperties::streamName)
        .def_readwrite("maxDataSize", &XLinkInProperties::maxDataSize)
        .def_readwrite("numFrames", &XLinkInProperties::numFrames)
        ;

    // Node
    xlinkIn
        .def_readonly("out", &XLinkIn::out, DOC(dai, node, XLinkIn, out))
        .def("setStreamName", &XLinkIn::setStreamName, py::arg("streamName"), DOC(dai, node, XLinkIn, setStreamName))
        .def("setMaxDataSize", &XLinkIn::setMaxDataSize, py::arg("maxDataSize"), DOC(dai, node, XLinkIn, setMaxDataSize))
        .def("setNumFrames",  &XLinkIn::setNumFrames, py::arg("numFrames"), DOC(dai, node, XLinkIn, setNumFrames))
        .def("getStreamName", &XLinkIn::getStreamName, DOC(dai, node, XLinkIn, getStreamName))
        .def("getMaxDataSize", &XLinkIn::getMaxDataSize, DOC(dai, node, XLinkIn, getMaxDataSize))
        .def("getNumFrames",  &XLinkIn::getNumFrames, DOC(dai, node, XLinkIn, getNumFrames))
        ;
    daiNodeModule.attr("XLinkIn").attr("Properties") = xlinkInProperties;

}
