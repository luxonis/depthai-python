#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/ToF.hpp"


void bind_tof(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<ToFProperties> tofProperties(m, "ToFProperties", DOC(dai, ToFProperties));
    auto tof = ADD_NODE(ToF);

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

    // ToF properties
    tofProperties
        .def_readwrite("initialConfig", &ToFProperties::initialConfig, DOC(dai, ToFProperties, initialConfig))
        ;

    // ToF Node
    tof
        .def_readonly("inputConfig", &ToF::inputConfig, DOC(dai, node, ToF, inputConfig))
        .def_readonly("inputRaw", &ToF::inputRaw, DOC(dai, node, ToF, inputRaw))
        .def_readonly("depth", &ToF::depth, DOC(dai, node, ToF, depth))
        .def_readonly("passthroughInputRaw", &ToF::passthroughInputRaw, DOC(dai, node, ToF, passthroughInputRaw))
        .def_readonly("initialConfig", &ToF::initialConfig, DOC(dai, node, ToF, initialConfig))
        ;
    daiNodeModule.attr("ToF").attr("Properties") = tofProperties;

}
