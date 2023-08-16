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

    // Properties
    tofProperties
        .def_readwrite("initialConfig", &ToFProperties::initialConfig, DOC(dai, ToFProperties, initialConfig))
        ;

    // Node
    tof
        .def_readonly("inputConfig", &ToF::inputConfig, DOC(dai, node, ToF, inputConfig), DOC(dai, node, ToF, inputConfig))
        .def_readonly("input", &ToF::input, DOC(dai, node, ToF, input), DOC(dai, node, ToF, input))
        .def_readonly("depth", &ToF::depth, DOC(dai, node, ToF, depth), DOC(dai, node, ToF, depth))
        .def_readonly("amplitude", &ToF::amplitude, DOC(dai, node, ToF, amplitude), DOC(dai, node, ToF, amplitude))
        .def_readonly("error", &ToF::error, DOC(dai, node, ToF, error), DOC(dai, node, ToF, error))
        .def_readonly("initialConfig", &ToF::initialConfig, DOC(dai, node, ToF, initialConfig), DOC(dai, node, ToF, initialConfig))
    ;
    // ALIAS
    daiNodeModule.attr("ToF").attr("Properties") = tofProperties;

}
