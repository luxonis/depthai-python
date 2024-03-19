#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"

void bind_xlinkout(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<XLinkOutProperties> xlinkOutProperties(m, "XLinkOutProperties", DOC(dai, XLinkOutProperties));
    auto xlinkOut = ADD_NODE(XLinkOut);

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
    xlinkOutProperties
        .def_readwrite("maxFpsLimit", &XLinkOutProperties::maxFpsLimit)
        .def_readwrite("streamName", &XLinkOutProperties::streamName)
        .def_readwrite("metadataOnly", &XLinkOutProperties::metadataOnly)
        ;

    // Node
    xlinkOut
        .def_readonly("input", &XLinkOut::input, DOC(dai, node, XLinkOut, input))
        .def("setStreamName", &XLinkOut::setStreamName, py::arg("streamName"), DOC(dai, node, XLinkOut, setStreamName))
        .def("setFpsLimit", &XLinkOut::setFpsLimit, py::arg("fpsLimit"), DOC(dai, node, XLinkOut, setFpsLimit))
        .def("getStreamName", &XLinkOut::getStreamName, DOC(dai, node, XLinkOut, getStreamName))
        .def("getFpsLimit", &XLinkOut::getFpsLimit, DOC(dai, node, XLinkOut, getFpsLimit))
        .def("setMetadataOnly", &XLinkOut::setMetadataOnly, DOC(dai, node, XLinkOut, setMetadataOnly))
        .def("getMetadataOnly", &XLinkOut::getMetadataOnly, DOC(dai, node, XLinkOut, getMetadataOnly))
        ;
    daiNodeModule.attr("XLinkOut").attr("Properties") = xlinkOutProperties;

}
