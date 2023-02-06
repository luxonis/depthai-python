#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/UAC.hpp"

void bind_uac(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<UACProperties> uacProperties(m, "UACProperties", DOC(dai, UACProperties));
    auto uac = ADD_NODE(UAC);

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

    // Properties TODO
    //uacProperties
    //    .def_readwrite("streamName", &UACProperties::streamName)
    //;

    // Node
    // UAC node
    uac
        .def_readonly("input", &UAC::input, DOC(dai, node, UAC, input))
        .def_readonly("inputConfig", &UAC::inputConfig, DOC(dai, node, UAC, inputConfig))
        ;
    // ALIAS
    daiNodeModule.attr("UAC").attr("Properties") = uacProperties;


}
