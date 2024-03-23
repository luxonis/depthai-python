#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Cast.hpp"

void bind_cast(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<CastProperties, std::shared_ptr<CastProperties>> castProperties(m, "CastProperties", DOC(dai, CastProperties));
    auto cast = ADD_NODE(Cast);

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

    castProperties
        .def_readwrite("numFramesPool", &CastProperties::numFramesPool, DOC(dai, CastProperties, numFramesPool))
        .def_readwrite("outputType", &CastProperties::outputType, DOC(dai, CastProperties, outputType))
        .def_readwrite("scale", &CastProperties::scale, DOC(dai, CastProperties, scale))
        ;

    // Node
    cast
        .def_readonly("input", &Cast::input, DOC(dai, node, Cast, input))
        .def_readonly("output", &Cast::output, DOC(dai, node, Cast, output))
        .def_readonly("passthroughInput", &Cast::passthroughInput, DOC(dai, node, Cast, passthroughInput))
        .def("setNumFramesPool", &Cast::setNumFramesPool, DOC(dai, node, Cast, setNumFramesPool))
        .def("setOutputFrameType", &Cast::setOutputFrameType, DOC(dai, node, Cast, setOutputFrameType))
        .def("setScale", &Cast::setScale, DOC(dai, node, Cast, setScale))
        ;
    daiNodeModule.attr("Cast").attr("Properties") = castProperties;

}
