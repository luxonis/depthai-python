#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/TofCamera.hpp"


void bind_tofcamera(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<TofCameraProperties> tofCameraProperties(m, "TofCameraProperties", DOC(dai, TofCameraProperties));
    auto featureTracker = ADD_NODE(TofCamera);

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

    // TofCamera properties
    tofCameraProperties
        .def_readwrite("initialConfig", &TofCameraProperties::initialConfig, DOC(dai, TofCameraProperties, initialConfig))
        ;

    // TofCamera Node
    featureTracker
        .def_readonly("inputConfig", &TofCamera::inputConfig, DOC(dai, node, TofCamera, inputConfig))
        .def_readonly("inputRaw", &TofCamera::inputRaw, DOC(dai, node, TofCamera, inputRaw))
        .def_readonly("depth", &TofCamera::depth, DOC(dai, node, TofCamera, depth))
        .def_readonly("passthroughInputRaw", &TofCamera::passthroughInputRaw, DOC(dai, node, TofCamera, passthroughInputRaw))
        .def_readonly("initialConfig", &TofCamera::initialConfig, DOC(dai, node, TofCamera, initialConfig))

        .def("setWaitForConfigInput", [](TofCamera& obj, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            obj.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, TofCamera, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](TofCamera& obj){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return obj.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, TofCamera, getWaitForConfigInput))

        ;
    daiNodeModule.attr("TofCamera").attr("Properties") = tofCameraProperties;

}
