#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Sync.hpp"

void bind_sync(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<SyncProperties> syncProperties(m, "SyncProperties");
    auto sync = ADD_NODE(Sync);

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

    // // Properties
    syncProperties
        .def_readwrite("syncPolicy", &SyncProperties::syncPolicy)
    ;

    // Node
    sync
        .def_readonly("input1", &Sync::input1, DOC(dai, node, Sync, input1))
        .def_readonly("input2", &Sync::input2, DOC(dai, node, Sync, input2))
        .def_readonly("input3", &Sync::input3, DOC(dai, node, Sync, input3))
        .def_readonly("output1", &Sync::output1, DOC(dai, node, Sync, output1))
        .def_readonly("output2", &Sync::output2, DOC(dai, node, Sync, output2))
        .def_readonly("output3", &Sync::output3, DOC(dai, node, Sync, output3))
        ;
    daiNodeModule.attr("Sync").attr("Properties") = syncProperties;

}
