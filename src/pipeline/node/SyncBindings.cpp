#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai-shared/properties/SyncProperties.hpp"
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
        .def_readwrite("syncIntervalMs", &SyncProperties::syncIntervalMs)
        .def_readwrite("syncAttempts", &SyncProperties::syncAttempts)
        ;

    // Node
    sync
        .def_readonly("out", &Sync::out, DOC(dai, node, Sync, out))
        .def_readonly("inputs", &Sync::inputs, DOC(dai, node, Sync, inputs))
        .def("setSyncIntervalMs", &Sync::setSyncIntervalMs, py::arg("syncInterval"), DOC(dai, node, Sync, setSyncInterval))
        .def("setSyncAttempts", &Sync::setSyncAttempts, py::arg("maxDataSize"), DOC(dai, node, Sync, setSyncAttempts))
        .def("getSyncIntervalMs", &Sync::getSyncIntervalMs, DOC(dai, node, Sync, getSyncIntervalMs))
        .def("getSyncAttempts", &Sync::getSyncAttempts, DOC(dai, node, Sync, getSyncAttempts))
        ;
    daiNodeModule.attr("Sync").attr("Properties") = syncProperties;

}
