#include "Common.hpp"
#include "NodeBindings.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/node/DetectionNetwork.hpp"
#include "depthai/pipeline/node/DetectionNetworkSub.hpp"

void bind_detectionnetwork(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<DetectionNetworkProperties, NeuralNetworkProperties, std::shared_ptr<DetectionNetworkProperties>> detectionNetworkProperties(m, "DetectionNetworkProperties", DOC(dai, DetectionNetworkProperties));
    auto detectionNetwork = ADD_NODE_DERIVED_ABSTRACT(DetectionNetwork, NeuralNetwork);
    auto mobileNetDetectionNetwork = ADD_NODE_DERIVED(MobileNetDetectionNetwork, DetectionNetwork);
    auto yoloDetectionNetwork = ADD_NODE_DERIVED(YoloDetectionNetwork, DetectionNetwork);

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

    // DetectionNetwork Properties
    detectionNetworkProperties
        .def_readwrite("parser", &DetectionNetworkProperties::parser)
        ;

    // DetectionNetwork Node
    detectionNetwork
        .def_readonly("input", &DetectionNetwork::input, DOC(dai, node, NeuralNetwork, input))
        .def_readonly("out", &DetectionNetwork::out, DOC(dai, node, DetectionNetwork, out))
        .def_readonly("outNetwork", &DetectionNetwork::outNetwork, DOC(dai, node, DetectionNetwork, outNetwork))
        .def_readonly("passthrough", &DetectionNetwork::passthrough, DOC(dai, node, NeuralNetwork, passthrough))
        .def("setConfidenceThreshold", &DetectionNetwork::setConfidenceThreshold, py::arg("thresh"), DOC(dai, node, DetectionNetwork, setConfidenceThreshold))
        .def("getConfidenceThreshold", &DetectionNetwork::getConfidenceThreshold, DOC(dai, node, DetectionNetwork, getConfidenceThreshold))
        ;
    // ALIAS
    daiNodeModule.attr("DetectionNetwork").attr("Properties") = detectionNetworkProperties;

    // YoloDetectionNetwork node
    yoloDetectionNetwork
        .def("setNumClasses", &YoloDetectionNetwork::setNumClasses, py::arg("numClasses"), DOC(dai, node, YoloDetectionNetwork, setNumClasses))
        .def("setCoordinateSize", &YoloDetectionNetwork::setCoordinateSize, py::arg("coordinates"), DOC(dai, node, YoloDetectionNetwork, setCoordinateSize))
        .def("setAnchors", &YoloDetectionNetwork::setAnchors, py::arg("anchors"), DOC(dai, node, YoloDetectionNetwork, setAnchors))
        .def("setAnchorMasks", &YoloDetectionNetwork::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, YoloDetectionNetwork, setAnchorMasks))
        .def("setIouThreshold", &YoloDetectionNetwork::setIouThreshold, py::arg("thresh"), DOC(dai, node, YoloDetectionNetwork, setIouThreshold))
        .def("getNumClasses", &YoloDetectionNetwork::getNumClasses, DOC(dai, node, YoloDetectionNetwork, getNumClasses))
        .def("getCoordinateSize", &YoloDetectionNetwork::getCoordinateSize, DOC(dai, node, YoloDetectionNetwork, getCoordinateSize))
        .def("getAnchors", &YoloDetectionNetwork::getAnchors, DOC(dai, node, YoloDetectionNetwork, getAnchors))
        .def("getAnchorMasks", &YoloDetectionNetwork::getAnchorMasks, DOC(dai, node, YoloDetectionNetwork, getAnchorMasks))
        .def("getIouThreshold", &YoloDetectionNetwork::getIouThreshold, DOC(dai, node, YoloDetectionNetwork, getIouThreshold))
    ;


}

void bind_detectionnetworksub(pybind11::module& m, void* pCallstack) {
    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    // py::class_<DetectionNetworkProperties, NeuralNetworkProperties, std::shared_ptr<DetectionNetworkProperties>> detectionNetworkProperties(
    //     m, "DetectionNetworkProperties", DOC(dai, DetectionNetworkProperties));
    auto detectionNetworkSub = ADD_NODE_DERIVED_ABSTRACT(DetectionNetworkSub, NeuralNetwork);
    auto mobileNetDetectionNetworkSub = ADD_NODE_DERIVED(MobileNetDetectionNetworkSub, DetectionNetworkSub);
    auto yoloDetectionNetworkSub = ADD_NODE_DERIVED(YoloDetectionNetworkSub, DetectionNetworkSub);

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

    // DetectionNetwork Properties
    // detectionNetworkProperties.def_readwrite("parser", &DetectionNetworkProperties::parser);

    // DetectionNetwork Node
    detectionNetworkSub
        .def_property_readonly(
            "input",
            [](const DetectionNetworkSub n) { return &n.neuralNetwork->input; },
            py::return_value_policy::reference_internal,
            DOC(dai, node, NeuralNetwork, input))
        .def_property_readonly(
            "out",
            [](const DetectionNetworkSub n) { return &n.detectionParser->out; },
            py::return_value_policy::reference_internal,
            DOC(dai, node, DetectionNetworkSub, out))
        .def_property_readonly(
            "outNetwork",
            [](const DetectionNetworkSub n) { return &n.neuralNetwork->out; },
            py::return_value_policy::reference_internal,
            DOC(dai, node, DetectionNetworkSub, outNetwork))
        .def_property_readonly(
            "passthrough",
            [](const DetectionNetworkSub n) { return &n.neuralNetwork->passthrough; },
            py::return_value_policy::reference_internal,
            DOC(dai, node, NeuralNetwork, passthrough))
        .def("setConfidenceThreshold",
             &DetectionNetworkSub::setConfidenceThreshold,
             py::arg("thresh"),
             DOC(dai, node, DetectionNetworkSub, setConfidenceThreshold))
        .def("getConfidenceThreshold", &DetectionNetworkSub::getConfidenceThreshold, DOC(dai, node, DetectionNetworkSub, getConfidenceThreshold));
    // ALIAS
    // daiNodeModule.attr("DetectionNetworkSub").attr("Properties") = detectionNetworkProperties;

    // YoloDetectionNetwork node
    yoloDetectionNetworkSub
        .def("setNumClasses", &YoloDetectionNetworkSub::setNumClasses, py::arg("numClasses"), DOC(dai, node, YoloDetectionNetworkSub, setNumClasses))
        .def("setCoordinateSize",
             &YoloDetectionNetworkSub::setCoordinateSize,
             py::arg("coordinates"),
             DOC(dai, node, YoloDetectionNetworkSub, setCoordinateSize))
        .def("setAnchors", &YoloDetectionNetworkSub::setAnchors, py::arg("anchors"), DOC(dai, node, YoloDetectionNetworkSub, setAnchors))
        .def("setAnchorMasks", &YoloDetectionNetworkSub::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, YoloDetectionNetworkSub, setAnchorMasks))
        .def("setIouThreshold", &YoloDetectionNetworkSub::setIouThreshold, py::arg("thresh"), DOC(dai, node, YoloDetectionNetworkSub, setIouThreshold))
        .def("getNumClasses", &YoloDetectionNetworkSub::getNumClasses, DOC(dai, node, YoloDetectionNetworkSub, getNumClasses))
        .def("getCoordinateSize", &YoloDetectionNetworkSub::getCoordinateSize, DOC(dai, node, YoloDetectionNetworkSub, getCoordinateSize))
        .def("getAnchors", &YoloDetectionNetworkSub::getAnchors, DOC(dai, node, YoloDetectionNetworkSub, getAnchors))
        .def("getAnchorMasks", &YoloDetectionNetworkSub::getAnchorMasks, DOC(dai, node, YoloDetectionNetworkSub, getAnchorMasks))
        .def("getIouThreshold", &YoloDetectionNetworkSub::getIouThreshold, DOC(dai, node, YoloDetectionNetworkSub, getIouThreshold));
}
