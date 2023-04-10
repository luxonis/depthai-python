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
        .def_readwrite("syncThresholdMs", &SyncProperties::syncThresholdMs, DOC(dai, SyncProperties, syncThresholdMs))
    ;

    // Node
    sync
        .def_readonly("inputs", &Sync::inputs, DOC(dai, node, Sync, inputs))
        .def_readonly("outputs", &Sync::outputs, DOC(dai, node, Sync, outputs))
        .def("setSyncThresholdMs", &Sync::setSyncThresholdMs, DOC(dai, node, Sync, setSyncThresholdMs))
        ;
    daiNodeModule.attr("Sync").attr("Properties") = syncProperties;

}
