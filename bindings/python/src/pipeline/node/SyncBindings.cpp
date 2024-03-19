#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/properties/SyncProperties.hpp"
#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Sync.hpp"

void bind_sync(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<SyncProperties> syncProperties(m, "SyncProperties", DOC(dai, SyncProperties));
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

    // Properties
    syncProperties
        .def_readwrite("syncThresholdNs", &SyncProperties::syncThresholdNs)
        .def_readwrite("syncAttempts", &SyncProperties::syncAttempts)
        ;

    // Node
    sync
        .def_readonly("out", &Sync::out, DOC(dai, node, Sync, out))
        .def_readonly("inputs", &Sync::inputs, DOC(dai, node, Sync, inputs))
        .def("setSyncThreshold", &Sync::setSyncThreshold, py::arg("syncThreshold"), DOC(dai, node, Sync, setSyncThreshold))
        .def("setSyncAttempts", &Sync::setSyncAttempts, py::arg("maxDataSize"), DOC(dai, node, Sync, setSyncAttempts))
        .def("getSyncThreshold", &Sync::getSyncThreshold, DOC(dai, node, Sync, getSyncThreshold))
        .def("getSyncAttempts", &Sync::getSyncAttempts, DOC(dai, node, Sync, getSyncAttempts))
        ;
    daiNodeModule.attr("Sync").attr("Properties") = syncProperties;

}
