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
        .def_readwrite("numFramesPool", &ToFProperties::numFramesPool, DOC(dai, ToFProperties, numFramesPool))
        .def_readwrite("numShaves", &ToFProperties::numShaves, DOC(dai, ToFProperties, numShaves))
        .def_readwrite("warpHwIds", &ToFProperties::warpHwIds, DOC(dai, ToFProperties, warpHwIds))
        ;

    // Node
    tof
        .def_readonly("inputConfig", &ToF::inputConfig, DOC(dai, node, ToF, inputConfig), DOC(dai, node, ToF, inputConfig))
        .def_readonly("input", &ToF::input, DOC(dai, node, ToF, input), DOC(dai, node, ToF, input))
        .def_readonly("depth", &ToF::depth, DOC(dai, node, ToF, depth), DOC(dai, node, ToF, depth))
        .def_readonly("amplitude", &ToF::amplitude, DOC(dai, node, ToF, amplitude), DOC(dai, node, ToF, amplitude))
        .def_readonly("intensity", &ToF::intensity, DOC(dai, node, ToF, intensity), DOC(dai, node, ToF, intensity))
        .def_readonly("phase", &ToF::phase, DOC(dai, node, ToF, phase), DOC(dai, node, ToF, phase))
        .def_readonly("initialConfig", &ToF::initialConfig, DOC(dai, node, ToF, initialConfig), DOC(dai, node, ToF, initialConfig))

        .def("setNumShaves", &ToF::setNumShaves, DOC(dai, node, ToF, setNumShaves))
        .def("setNumFramesPool", &ToF::setNumFramesPool, DOC(dai, node, ToF, setNumFramesPool))
    ;
    // ALIAS
    daiNodeModule.attr("ToF").attr("Properties") = tofProperties;

}
