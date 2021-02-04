#include "PipelineBindings.hpp"

// depthai
#include "depthai/pipeline/Pipeline.hpp"

// depthai - nodes
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/VideoEncoder.hpp"
#include "depthai/pipeline/node/SPIOut.hpp"
#include "depthai/pipeline/node/ImageManip.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"
#include "depthai/pipeline/node/DetectionNetwork.hpp"
#include "depthai/pipeline/node/UVC.hpp"

// depthai-shared
#include "depthai-shared/pb/properties/GlobalProperties.hpp"

void PipelineBindings::bind(pybind11::module& m){

    using namespace dai;


    // Bind global properties
    py::class_<GlobalProperties>(m, "GlobalProperties")
        .def_readwrite("leonOsFrequencyHz", &GlobalProperties::leonCssFrequencyHz)
        .def_readwrite("leonRtFrequencyHz", &GlobalProperties::leonMssFrequencyHz)
        .def_readwrite("pipelineName", &GlobalProperties::pipelineName)
        .def_readwrite("pipelineVersion", &GlobalProperties::pipelineVersion)
        ;



    // bind pipeline
    py::class_<Pipeline>(m, "Pipeline")
        .def(py::init<>())
        .def(py::init<const Pipeline&>())
        .def("getAssetManager", static_cast<const AssetManager& (Pipeline::*)() const>(&Pipeline::getAssetManager), py::return_value_policy::reference_internal)
        .def("getAssetManager", static_cast<AssetManager& (Pipeline::*)()>(&Pipeline::getAssetManager), py::return_value_policy::reference_internal)
        .def("getGlobalProperties", &Pipeline::getGlobalProperties)
        .def("getAllAssets", &Pipeline::getAllAssets)
        .def("remove", &Pipeline::remove)
        .def("getAllNodes", static_cast<std::vector<std::shared_ptr<const Node>> (Pipeline::*)() const>(&Pipeline::getAllNodes), py::return_value_policy::reference_internal)
        .def("getAllNodes", static_cast<std::vector<std::shared_ptr< Node>> (Pipeline::*)()>(&Pipeline::getAllNodes), py::return_value_policy::reference_internal)
        
        .def("getNode", static_cast<std::shared_ptr<const Node> (Pipeline::*)(Node::Id) const>(&Pipeline::getNode), py::return_value_policy::reference_internal)
        .def("getNode", static_cast<std::shared_ptr<Node> (Pipeline::*)(Node::Id)>(&Pipeline::getNode), py::return_value_policy::reference_internal)

        .def("getConnections", &Pipeline::getConnections)
        .def("link", &Pipeline::link)
        .def("unlink", &Pipeline::unlink)


         // templated create<NODE> function 
        .def("createXLinkIn", &Pipeline::create<node::XLinkIn>)
        .def("createXLinkOut", &Pipeline::create<node::XLinkOut>)
        .def("createNeuralNetwork", &Pipeline::create<node::NeuralNetwork>)
        .def("createColorCamera", &Pipeline::create<node::ColorCamera>)
        .def("createVideoEncoder", &Pipeline::create<node::VideoEncoder>)
        .def("createSPIOut", &Pipeline::create<node::SPIOut>)
        .def("createImageManip", &Pipeline::create<node::ImageManip>)
        .def("createMonoCamera", &Pipeline::create<node::MonoCamera>)
        .def("createStereoDepth", &Pipeline::create<node::StereoDepth>)
        .def("createMobileNetDetectionNetwork", &Pipeline::create<node::MobileNetDetectionNetwork>)
        .def("createYoloDetectionNetwork", &Pipeline::create<node::YoloDetectionNetwork>)
        .def("createUVC", &Pipeline::create<node::UVC>)
        ;
    

}
