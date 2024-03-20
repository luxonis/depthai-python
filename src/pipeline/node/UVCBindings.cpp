
#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/UVC.hpp"

void bind_uvc(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<UVCProperties> uvcProperties(m, "UVCProperties", DOC(dai, UVCProperties));
    auto uvc = ADD_NODE(UVC);

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
    uvcProperties
        .def_readwrite("gpioInit", &UVCProperties::gpioInit)
        .def_readwrite("gpioStreamOn", &UVCProperties::gpioStreamOn)
        .def_readwrite("gpioStreamOff", &UVCProperties::gpioStreamOff)
        ;

    // UVC node
    uvc
        .def_readonly("input", &UVC::input, DOC(dai, node, UVC, input))
        .def("setGpiosOnInit", &UVC::setGpiosOnInit, py::arg("list"), DOC(dai, node, UVC, setGpiosOnInit))
        .def("setGpiosOnStreamOn", &UVC::setGpiosOnStreamOn, py::arg("list"), DOC(dai, node, UVC, setGpiosOnStreamOn))
        .def("setGpiosOnStreamOff", &UVC::setGpiosOnStreamOff, py::arg("list"), DOC(dai, node, UVC, setGpiosOnStreamOff))
        ;

    // ALIAS
    daiNodeModule.attr("UVC").attr("Properties") = uvcProperties;

}

