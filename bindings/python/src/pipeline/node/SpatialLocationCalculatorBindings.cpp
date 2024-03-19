#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/SpatialLocationCalculator.hpp"

void bind_spatiallocationcalculator(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<SpatialLocationCalculatorProperties> spatialLocationCalculatorProperties(m, "SpatialLocationCalculatorProperties", DOC(dai, SpatialLocationCalculatorProperties));
    auto spatialLocationCalculator = ADD_NODE(SpatialLocationCalculator);


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
    spatialLocationCalculatorProperties
        .def_readwrite("roiConfig", &SpatialLocationCalculatorProperties::roiConfig)
        ;

    // Node
    spatialLocationCalculator
        .def_readonly("inputConfig", &SpatialLocationCalculator::inputConfig, DOC(dai, node, SpatialLocationCalculator, inputConfig))
        .def_readonly("inputDepth", &SpatialLocationCalculator::inputDepth, DOC(dai, node, SpatialLocationCalculator, inputDepth))
        .def_readonly("out", &SpatialLocationCalculator::out, DOC(dai, node, SpatialLocationCalculator, out))
        .def_readonly("passthroughDepth", &SpatialLocationCalculator::passthroughDepth, DOC(dai, node, SpatialLocationCalculator, passthroughDepth))
        .def_readonly("initialConfig", &SpatialLocationCalculator::initialConfig, DOC(dai, node, SpatialLocationCalculator, initialConfig))

        .def("setWaitForConfigInput", [](SpatialLocationCalculator& obj, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            obj.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, SpatialLocationCalculator, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](SpatialLocationCalculator& obj){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return obj.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, SpatialLocationCalculator, getWaitForConfigInput))

        ;
    // ALIAS
    daiNodeModule.attr("SpatialLocationCalculator").attr("Properties") = spatialLocationCalculatorProperties;

}
