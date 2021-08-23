
#include "PipelineBindings.hpp"
#include "NodeBindings.hpp"

// depthai
#include "depthai/pipeline/Pipeline.hpp"

// depthai - nodes
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/VideoEncoder.hpp"
#include "depthai/pipeline/node/SPIOut.hpp"
#include "depthai/pipeline/node/SPIIn.hpp"
#include "depthai/pipeline/node/ImageManip.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"
#include "depthai/pipeline/node/DetectionNetwork.hpp"
#include "depthai/pipeline/node/Script.hpp"
#include "depthai/pipeline/node/SystemLogger.hpp"
#include "depthai/pipeline/node/SpatialLocationCalculator.hpp"
#include "depthai/pipeline/node/SpatialDetectionNetwork.hpp"
#include "depthai/pipeline/node/ObjectTracker.hpp"
#include "depthai/pipeline/node/IMU.hpp"
#include "depthai/pipeline/node/EdgeDetector.hpp"

// depthai-shared
#include "depthai-shared/properties/GlobalProperties.hpp"



std::shared_ptr<dai::Node> createNode(dai::Pipeline& p, py::object class_){
    auto nodeCreateMap = NodeBindings::getNodeCreateMap();
    for(auto& kv : nodeCreateMap){
        auto& node = kv.first;
        auto& create = kv.second;
        if(node.is(class_)){
            return create(p, class_);
        }
    }
    return nullptr;
}

void PipelineBindings::bind(pybind11::module& m, void* pCallstack){
    using namespace dai;

    // Type definitions
    py::class_<GlobalProperties> globalProperties(m, "GlobalProperties", DOC(dai, GlobalProperties));
    py::class_<Pipeline> pipeline(m, "Pipeline", DOC(dai, Pipeline, 2));

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


    // Bind global properties
    globalProperties
        .def_readwrite("leonOsFrequencyHz", &GlobalProperties::leonCssFrequencyHz)
        .def_readwrite("leonRtFrequencyHz", &GlobalProperties::leonMssFrequencyHz)
        .def_readwrite("pipelineName", &GlobalProperties::pipelineName)
        .def_readwrite("pipelineVersion", &GlobalProperties::pipelineVersion)
        .def_readwrite("cameraTuningBlobSize", &GlobalProperties::cameraTuningBlobSize, DOC(dai, GlobalProperties, cameraTuningBlobSize))
        .def_readwrite("cameraTuningBlobUri", &GlobalProperties::cameraTuningBlobUri, DOC(dai, GlobalProperties, cameraTuningBlobUri))
        ;

    // bind pipeline
    pipeline
        .def(py::init<>(), DOC(dai, Pipeline, Pipeline))
        //.def(py::init<const Pipeline&>())
        .def("getGlobalProperties", &Pipeline::getGlobalProperties, DOC(dai, Pipeline, getGlobalProperties))
        //.def("create", &Pipeline::create<node::XLinkIn>)
        .def("remove", &Pipeline::remove, py::arg("node"), DOC(dai, Pipeline, remove))
        .def("getAllNodes", static_cast<std::vector<std::shared_ptr<const Node>> (Pipeline::*)() const>(&Pipeline::getAllNodes), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getAllNodes))
        .def("getAllNodes", static_cast<std::vector<std::shared_ptr< Node>> (Pipeline::*)()>(&Pipeline::getAllNodes), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getAllNodes))
        .def("getNode", static_cast<std::shared_ptr<const Node> (Pipeline::*)(Node::Id) const>(&Pipeline::getNode), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getNode))
        .def("getNode", static_cast<std::shared_ptr<Node> (Pipeline::*)(Node::Id)>(&Pipeline::getNode), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getNode))
        .def("getConnections", &Pipeline::getConnections, DOC(dai, Pipeline, getConnections), DOC(dai, Pipeline, getConnections))
        .def("getConnectionMap", &Pipeline::getConnectionMap, DOC(dai, Pipeline, getConnectionMap), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getConnectionMap))
        .def("getNodeMap", &Pipeline::getNodeMap, DOC(dai, Pipeline, getNodeMap), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getNodeMap))
        .def("link", &Pipeline::link, DOC(dai, Pipeline, link), DOC(dai, Pipeline, link))
        .def("unlink", &Pipeline::unlink, DOC(dai, Pipeline, unlink), DOC(dai, Pipeline, unlink))
        .def("getAssetManager", static_cast<const AssetManager& (Pipeline::*)() const>(&Pipeline::getAssetManager), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getAssetManager))
        .def("getAssetManager", static_cast<AssetManager& (Pipeline::*)()>(&Pipeline::getAssetManager), py::return_value_policy::reference_internal, DOC(dai, Pipeline, getAssetManager))
        .def("setOpenVINOVersion", &Pipeline::setOpenVINOVersion, py::arg("version") = Pipeline::DEFAULT_OPENVINO_VERSION, DOC(dai, Pipeline, setOpenVINOVersion))
        .def("getOpenVINOVersion", &Pipeline::getOpenVINOVersion, DOC(dai, Pipeline, getOpenVINOVersion))
        .def("setCameraTuningBlobPath", &Pipeline::setCameraTuningBlobPath, py::arg("path"), DOC(dai, Pipeline, setCameraTuningBlobPath))
        .def("setCalibrationData", &Pipeline::setCalibrationData, py::arg("calibrationDataHandler"), DOC(dai, Pipeline, setCalibrationData))
        .def("getCalibrationData", &Pipeline::getCalibrationData, DOC(dai, Pipeline, getCalibrationData))
        // 'Template' create function
        .def("create", [](dai::Pipeline& p, py::object class_) {
            auto node = createNode(p, class_);
            if(node == nullptr){
                throw std::invalid_argument(std::string(py::str(class_)) + " is not a subclass of depthai.node");
            }
            return node;
        })
        // TODO(themarpe) DEPRECATE, use pipeline.create([class name])
        // templated create<NODE> function
        .def("createXLinkIn", &Pipeline::create<node::XLinkIn>)
        .def("createXLinkOut", &Pipeline::create<node::XLinkOut>)
        .def("createNeuralNetwork", &Pipeline::create<node::NeuralNetwork>)
        .def("createColorCamera", &Pipeline::create<node::ColorCamera>)
        .def("createVideoEncoder", &Pipeline::create<node::VideoEncoder>)
        .def("createSPIOut", &Pipeline::create<node::SPIOut>)
        .def("createSPIIn", &Pipeline::create<node::SPIIn>)
        .def("createImageManip", &Pipeline::create<node::ImageManip>)
        .def("createMonoCamera", &Pipeline::create<node::MonoCamera>)
        .def("createStereoDepth", &Pipeline::create<node::StereoDepth>)
        .def("createMobileNetDetectionNetwork", &Pipeline::create<node::MobileNetDetectionNetwork>)
        .def("createYoloDetectionNetwork", &Pipeline::create<node::YoloDetectionNetwork>)
        .def("createSystemLogger", &Pipeline::create<node::SystemLogger>)
        .def("createSpatialLocationCalculator", &Pipeline::create<node::SpatialLocationCalculator>)
        .def("createMobileNetSpatialDetectionNetwork", &Pipeline::create<node::MobileNetSpatialDetectionNetwork>)
        .def("createYoloSpatialDetectionNetwork", &Pipeline::create<node::YoloSpatialDetectionNetwork>)
        .def("createObjectTracker", &Pipeline::create<node::ObjectTracker>)
        .def("createIMU", &Pipeline::create<node::IMU>)
        .def("createEdgeDetector", &Pipeline::create<node::EdgeDetector>)
        ;


}
