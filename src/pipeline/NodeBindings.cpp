#include "NodeBindings.hpp"

#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"

void NodeBindings::bind(pybind11::module& m){

    using namespace dai;
    using namespace dai::node;


    py::class_<Node::Input>(m, "Input");

    py::class_<Node::Output>(m, "Output")
        .def("canConnect", &dai::Node::Output::canConnect)
        .def("link", &dai::Node::Output::link)
    ;

    py::class_<Node, std::shared_ptr<Node>>(m, "Node");

    py::class_<XLinkIn, Node, std::shared_ptr<XLinkIn>>(m, "XLinkIn")
        .def_readonly("out", &XLinkIn::out)
        .def("setStreamName", &XLinkIn::setStreamName)
        .def("setMaxDataSize", &XLinkIn::setMaxDataSize)
        .def("setNumFrames",  &XLinkIn::setNumFrames)   
        ;


    py::class_<XLinkOut, Node, std::shared_ptr<XLinkOut>>(m, "XLinkOut")
        .def_readonly("input", &XLinkOut::in)
        .def("setStreamName", &XLinkOut::setStreamName)
        .def("setFpsLimit", &XLinkOut::setFpsLimit)
        ;

    py::class_<NeuralNetwork, Node, std::shared_ptr<NeuralNetwork>>(m, "NeuralNetwork")
        .def_readonly("input", &NeuralNetwork::in)
        .def_readonly("out", &NeuralNetwork::out)
        .def("setBlobPath", &NeuralNetwork::setBlobPath)
        .def("setNumPoolFrames", &NeuralNetwork::setNumPoolFrames)
        ;

    // properties

    py::class_<ColorCameraProperties> colorCameraProperties(m, "ColorCameraProperties");
    colorCameraProperties
        .def_readwrite("camId", &ColorCameraProperties::camId)
        .def_readwrite("colorOrder", &ColorCameraProperties::colorOrder)
        .def_readwrite("interleaved", &ColorCameraProperties::interleaved)
        .def_readwrite("previewHeight", &ColorCameraProperties::previewHeight)
        .def_readwrite("previewWidth", &ColorCameraProperties::previewWidth)
        .def_readwrite("resolution", &ColorCameraProperties::resolution)
    ;

    py::enum_<ColorCameraProperties::SensorResolution>(colorCameraProperties, "SensorResolution")
        .value("THE_1080_P", ColorCameraProperties::SensorResolution::THE_1080_P)
        .value("THE_4_K", ColorCameraProperties::SensorResolution::THE_4_K)
        .export_values()
        ;

    py::enum_<ColorCameraProperties::ColorOrder>(colorCameraProperties, "ColorOrder")
        .value("BGR", ColorCameraProperties::ColorOrder::BGR)
        .value("RGB", ColorCameraProperties::ColorOrder::RGB)
        .export_values()
        ;
        
    py::class_<ColorCamera, Node, std::shared_ptr<ColorCamera>>(m, "ColorCamera")
        .def_readonly("video", &ColorCamera::video)
        .def_readonly("preview", &ColorCamera::preview)
        .def_readonly("still", &ColorCamera::still)
        .def("setCamId", &ColorCamera::setCamId)
        .def("setColorOrder", &ColorCamera::setColorOrder)
        .def("setInterleaved", &ColorCamera::setInterleaved)
        .def("setPreviewSize", &ColorCamera::setPreviewSize)
        .def("setResolution", &ColorCamera::setResolution)
        ;




}