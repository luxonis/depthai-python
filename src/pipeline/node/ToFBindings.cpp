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

    // Properties TODO
    //tofProperties
    //    .def_readwrite("streamName", &ToFProperties::streamName)
    //;

    // Node
    // ToF node
    tof
        .def_readonly("inputImage", &ToF::inputImage, DOC(dai, node, ToF, inputImage))
        .def_readonly("out", &ToF::out, DOC(dai, node, ToF, out))
        .def_readonly("amp_out", &ToF::amp_out, DOC(dai, node, ToF, amp_out))
        .def_readonly("err_out", &ToF::err_out, DOC(dai, node, ToF, err_out))
        ;
    // ALIAS
    daiNodeModule.attr("ToF").attr("Properties") = tofProperties;


}
