#include "PipelineBindings.hpp"

// depthai
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"
#include "depthai/pipeline/Pipeline.hpp"

// depthai-shared
#include "depthai-shared/pb/properties/GlobalProperties.hpp"

void PipelineBindings::bind(pybind11::module& m){

    using namespace dai;


    // Bind global properties
    py::class_<GlobalProperties>(m, "GlobalProperties")
        .def_readwrite("leonOsFrequencyHz", &GlobalProperties::leonOsFrequencyHz)
        .def_readwrite("leonRtFrequencyHz", &GlobalProperties::leonRtFrequencyHz)
        .def_readwrite("pipelineName", &GlobalProperties::pipelineName)
        .def_readwrite("pipelineVersion", &GlobalProperties::pipelineVersion)
        ;



    // bind pipeline
    py::class_<Pipeline>(m, "Pipeline")
        .def(py::init<>())
        .def(py::init<const Pipeline&>())
        .def("getGlobalProperties", &Pipeline::getGlobalProperties)
        //.def("getPipelineSchema", &Pipeline::getPipelineSchema)
        .def("getNodes", &Pipeline::getNodes)        
        // TODO - .def("getAssetManager", &Pipeline::getAssetManager)

         // templated create<NODE> function 
        .def("createXLinkIn", &Pipeline::create<node::XLinkIn>)
        .def("createXLinkOut", &Pipeline::create<node::XLinkOut>)
        .def("createNeuralNetwork", &Pipeline::create<node::NeuralNetwork>)
        .def("createColorCamera", &Pipeline::create<node::ColorCamera>)
        .def("createMonoCamera", &Pipeline::create<node::MonoCamera>)
        .def("createStereoDepth", &Pipeline::create<node::StereoDepth>)
        ;
    

}