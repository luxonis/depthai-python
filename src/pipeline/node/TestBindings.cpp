#include "Common.hpp"
#include "NodeBindings.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/node/TestIn.hpp"
#include "depthai/pipeline/node/TestOut.hpp"

void bind_testnodes(pybind11::module& m, void* pCallstack) {
    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    // py::class_<TestInProperties> testInProperties(m, "testInProperties", DOC(dai, testInProperties));
    auto testIn = ADD_NODE(TestIn);
    auto testOut = ADD_NODE(TestOut);

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*)pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Properties
    // xlinkInProperties.def_readwrite("streamName", &XLinkInProperties::streamName)
    //     .def_readwrite("maxDataSize", &XLinkInProperties::maxDataSize)
    //     .def_readwrite("numFrames", &XLinkInProperties::numFrames);

    // Node
    testIn.def_readonly("out", &TestIn::out, DOC(dai, node, TestIn, out))
        .def_readonly("input", &TestIn::input, DOC(dai, node, TestIn, input))
        .def("setNumMessagesToSend", &TestIn::setNumMessagesToSend, py::arg("num"), DOC(dai, node, TestIn, setNumMessagesToSend));
    testOut.def_readonly("input", &TestOut::input, DOC(dai, node, TestOut, input));
}
