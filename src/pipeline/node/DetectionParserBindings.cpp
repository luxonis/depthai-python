#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/DetectionParser.hpp"


void bind_detectionparser(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<DetectionParserProperties> detectionParserProperties(m, "DetectionParserProperties", DOC(dai, DetectionParserProperties));
    auto detectionParser = ADD_NODE(DetectionParser);

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
    detectionParserProperties
        .def_readwrite("parser", &DetectionParserProperties::parser, DOC(dai, DetectionParserProperties, parser))
    ;
    // Node
    detectionParser
        .def_readonly("input", &DetectionParser::input, DOC(dai, node, DetectionParser, input))
        .def_readonly("out", &DetectionParser::out, DOC(dai, node, DetectionParser, out))
        .def("setNumFramesPool", &DetectionParser::setNumFramesPool, py::arg("numFramesPool"), DOC(dai, node, DetectionParser, setNumFramesPool))
        .def("getNumFramesPool", &DetectionParser::getNumFramesPool, DOC(dai, node, DetectionParser, getNumFramesPool))
        .def("setBlob", &DetectionParser::setBlob, py::arg("blob"), DOC(dai, node, DetectionParser, setBlob))
        .def("setNNFamily", &DetectionParser::setNNFamily, py::arg("type"), DOC(dai, node, DetectionParser, setNNFamily))
        .def("getNNFamily", &DetectionParser::getNNFamily, DOC(dai, node, DetectionParser, getNNFamily))
        .def("setConfidenceThreshold", &DetectionParser::setConfidenceThreshold, py::arg("thresh"), DOC(dai, node, DetectionParser, setConfidenceThreshold))
        .def("getConfidenceThreshold", &DetectionParser::getConfidenceThreshold, DOC(dai, node, DetectionParser, getConfidenceThreshold))
        .def("setNumClasses", &DetectionParser::setNumClasses, py::arg("numClasses"), DOC(dai, node, DetectionParser, setNumClasses))
        .def("setCoordinateSize", &DetectionParser::setCoordinateSize, py::arg("coordinates"), DOC(dai, node, DetectionParser, setCoordinateSize))
        .def("setAnchors", &DetectionParser::setAnchors, py::arg("anchors"), DOC(dai, node, DetectionParser, setAnchors))
        .def("setAnchorMasks", &DetectionParser::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, DetectionParser, setAnchorMasks))
        .def("setIouThreshold", &DetectionParser::setIouThreshold, py::arg("thresh"), DOC(dai, node, DetectionParser, setIouThreshold))
        .def("getNumClasses", &DetectionParser::getNumClasses, DOC(dai, node, DetectionParser, getNumClasses))
        .def("getCoordinateSize", &DetectionParser::getCoordinateSize, DOC(dai, node, DetectionParser, getCoordinateSize))
        .def("getAnchors", &DetectionParser::getAnchors, DOC(dai, node, DetectionParser, getAnchors))
        .def("getAnchorMasks", &DetectionParser::getAnchorMasks, DOC(dai, node, DetectionParser, getAnchorMasks))
        .def("getIouThreshold", &DetectionParser::getIouThreshold, DOC(dai, node, DetectionParser, getIouThreshold))
        ;
    daiNodeModule.attr("DetectionParser").attr("Properties") = detectionParserProperties;

}
