#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/SystemLogger.hpp"

void bind_systemlogger(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<SystemLoggerProperties> systemLoggerProperties(m, "SystemLoggerProperties", DOC(dai, SystemLoggerProperties));
    auto systemLogger = ADD_NODE(SystemLogger);


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
    systemLoggerProperties
        .def_readwrite("rateHz", &SystemLoggerProperties::rateHz)
        ;

    // Node
    systemLogger
        .def_readonly("out", &SystemLogger::out, DOC(dai, node, SystemLogger, out))
        .def("setRate", &SystemLogger::setRate, py::arg("hz"), DOC(dai, node, SystemLogger, setRate))
        .def("getRate", &SystemLogger::getRate, DOC(dai, node, SystemLogger, getRate))
        ;


}
