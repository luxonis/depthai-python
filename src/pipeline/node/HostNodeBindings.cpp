#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/HostNode.hpp"

using namespace dai;

class PyHostNode : public NodeCRTP<HostNode, PyHostNode> {
public:
    void run() override {
        PYBIND11_OVERRIDE_PURE(
                void,
                HostNode,
                run);
    }
};

void bind_hostnode(pybind11::module& m, void* pCallstack){
    // declare upfront
    auto hostNode = py::class_<HostNode, PyHostNode, ThreadedNode, std::shared_ptr<HostNode>>(m, "HostNode", DOC(dai, node, HostNode));

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

    hostNode
        .def(py::init<>())
        .def("run", &HostNode::run)
    ;
}
