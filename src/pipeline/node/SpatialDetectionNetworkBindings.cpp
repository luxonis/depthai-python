#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/SpatialDetectionNetwork.hpp"

void bind_spatialdetectionnetwork(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<SpatialDetectionNetworkProperties, DetectionNetworkProperties, std::shared_ptr<SpatialDetectionNetworkProperties>> spatialDetectionNetworkProperties(m, "SpatialDetectionNetworkProperties", DOC(dai, SpatialDetectionNetworkProperties));
    auto spatialDetectionNetwork = ADD_NODE_DERIVED_ABSTRACT(SpatialDetectionNetwork, DetectionNetwork);
    auto mobileNetSpatialDetectionNetwork = ADD_NODE_DERIVED(MobileNetSpatialDetectionNetwork, SpatialDetectionNetwork);
    auto yoloSpatialDetectionNetwork = ADD_NODE_DERIVED(YoloSpatialDetectionNetwork, SpatialDetectionNetwork);

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
    spatialDetectionNetworkProperties
        .def_readwrite("detectedBBScaleFactor", &SpatialDetectionNetworkProperties::detectedBBScaleFactor)
        .def_readwrite("depthThresholds", &SpatialDetectionNetworkProperties::depthThresholds)
        ;


    // Node
    spatialDetectionNetwork
        .def_readonly("input", &SpatialDetectionNetwork::input, DOC(dai, node, SpatialDetectionNetwork, input))
        .def_readonly("inputDepth", &SpatialDetectionNetwork::inputDepth, DOC(dai, node, SpatialDetectionNetwork, inputDepth))
        .def_readonly("out", &SpatialDetectionNetwork::out, DOC(dai, node, SpatialDetectionNetwork, out))
        .def_readonly("boundingBoxMapping", &SpatialDetectionNetwork::boundingBoxMapping, DOC(dai, node, SpatialDetectionNetwork, boundingBoxMapping))
        .def_readonly("passthrough", &SpatialDetectionNetwork::passthrough, DOC(dai, node, SpatialDetectionNetwork, passthrough))
        .def_readonly("passthroughDepth", &SpatialDetectionNetwork::passthroughDepth, DOC(dai, node, SpatialDetectionNetwork, passthroughDepth))
        .def_readonly("spatialLocationCalculatorOutput", &SpatialDetectionNetwork::spatialLocationCalculatorOutput, DOC(dai, node, SpatialDetectionNetwork, spatialLocationCalculatorOutput))

        .def("setDepthLowerThreshold", &SpatialDetectionNetwork::setDepthLowerThreshold, py::arg("lowerThreshold"), DOC(dai, node, SpatialDetectionNetwork, setDepthLowerThreshold))
        .def("setBoundingBoxScaleFactor", &SpatialDetectionNetwork::setBoundingBoxScaleFactor, py::arg("scaleFactor"), DOC(dai, node, SpatialDetectionNetwork, setBoundingBoxScaleFactor))
        .def("setDepthUpperThreshold", &SpatialDetectionNetwork::setDepthUpperThreshold, py::arg("upperThreshold"), DOC(dai, node, SpatialDetectionNetwork, setDepthUpperThreshold))
        .def("setSpatialCalculationAlgorithm", &SpatialDetectionNetwork::setSpatialCalculationAlgorithm, py::arg("calculationAlgorithm"), DOC(dai, node, SpatialDetectionNetwork, setSpatialCalculationAlgorithm))
        .def("setSpatialCalculationStepSize", &SpatialDetectionNetwork::setSpatialCalculationStepSize, py::arg("stepSize"), DOC(dai, node, SpatialDetectionNetwork, setSpatialCalculationStepSize))
        ;
     // ALIAS
    daiNodeModule.attr("SpatialDetectionNetwork").attr("Properties") = spatialDetectionNetworkProperties;

    // // MobileNetSpatialDetectionNetwork
    // mobileNetSpatialDetectionNetwork;
    // YoloSpatialDetectionNetwork
    yoloSpatialDetectionNetwork
        .def("setNumClasses", &YoloSpatialDetectionNetwork::setNumClasses, py::arg("numClasses"), DOC(dai, node, YoloSpatialDetectionNetwork, setNumClasses))
        .def("setCoordinateSize", &YoloSpatialDetectionNetwork::setCoordinateSize, py::arg("coordinates"), DOC(dai, node, YoloSpatialDetectionNetwork, setCoordinateSize))
        .def("setAnchors", &YoloSpatialDetectionNetwork::setAnchors, py::arg("anchors"), DOC(dai, node, YoloSpatialDetectionNetwork, setAnchors))
        .def("setAnchorMasks", &YoloSpatialDetectionNetwork::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, YoloSpatialDetectionNetwork, setAnchorMasks))
        .def("setIouThreshold", &YoloSpatialDetectionNetwork::setIouThreshold, py::arg("thresh"), DOC(dai, node, YoloSpatialDetectionNetwork, setIouThreshold))
        .def("getNumClasses", &YoloSpatialDetectionNetwork::getNumClasses, DOC(dai, node, YoloSpatialDetectionNetwork, getNumClasses))
        .def("getCoordinateSize", &YoloSpatialDetectionNetwork::getCoordinateSize, DOC(dai, node, YoloSpatialDetectionNetwork, getCoordinateSize))
        .def("getAnchors", &YoloSpatialDetectionNetwork::getAnchors, DOC(dai, node, YoloSpatialDetectionNetwork, getAnchors))
        .def("getAnchorMasks", &YoloSpatialDetectionNetwork::getAnchorMasks, DOC(dai, node, YoloSpatialDetectionNetwork, getAnchorMasks))
        .def("getIouThreshold", &YoloSpatialDetectionNetwork::getIouThreshold, DOC(dai, node, YoloSpatialDetectionNetwork, getIouThreshold))
        ;

}
