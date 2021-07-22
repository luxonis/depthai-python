#include "NodeBindings.hpp"

#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"
#include "depthai/pipeline/node/VideoEncoder.hpp"
#include "depthai/pipeline/node/ImageManip.hpp"
#include "depthai/pipeline/node/SPIOut.hpp"
#include "depthai/pipeline/node/DetectionNetwork.hpp"
#include "depthai/pipeline/node/SystemLogger.hpp"
#include "depthai/pipeline/node/SpatialLocationCalculator.hpp"
#include "depthai/pipeline/node/SpatialDetectionNetwork.hpp"
#include "depthai/pipeline/node/ObjectTracker.hpp"
#include "depthai/pipeline/node/IMU.hpp"
#include "depthai/pipeline/node/EdgeDetector.hpp"

// Libraries
#include "hedley/hedley.h"

void NodeBindings::bind(pybind11::module& m){

    using namespace dai;
    using namespace dai::node;


    // Base 'Node' class binding
    py::class_<Node, std::shared_ptr<Node>> pyNode(m, "Node", DOC(dai, Node));


    // Node::Input bindings
    py::class_<Node::Input> pyInput(pyNode, "Input", DOC(dai, Node, Input));
    py::enum_<Node::Input::Type>(pyInput, "Type")
        .value("SReceiver", Node::Input::Type::SReceiver)
        .value("MReceiver", Node::Input::Type::MReceiver)
    ;
    pyInput
        .def_property_readonly("name", [](const Node::Input& input){ return input.name; })
        .def_property_readonly("type", [](const Node::Input& input){ return input.type; })
        .def("setBlocking", &Node::Input::setBlocking, py::arg("blocking"), DOC(dai, Node, Input, setBlocking))
        .def("getBlocking", &Node::Input::getBlocking, DOC(dai, Node, Input, getBlocking))
        .def("setQueueSize", &Node::Input::setQueueSize, py::arg("size"), DOC(dai, Node, Input, setQueueSize))
        .def("getQueueSize", &Node::Input::getQueueSize, DOC(dai, Node, Input, getQueueSize))
    ;

    // Node::Output bindings
    py::class_<Node::Output> pyOutput(pyNode, "Output", DOC(dai, Node, Output));
    py::enum_<Node::Output::Type>(pyOutput, "Type")
        .value("MSender", Node::Output::Type::MSender)
        .value("SSender", Node::Output::Type::SSender)
    ;
    pyOutput
        .def("canConnect", &Node::Output::canConnect, py::arg("in"), DOC(dai, Node, Output, canConnect))
        .def("link", &Node::Output::link, py::arg("in"), DOC(dai, Node, Output, link))
        .def("unlink", &Node::Output::unlink, py::arg("in"), DOC(dai, Node, Output, unlink))
        .def("getConnections", &Node::Output::getConnections, DOC(dai, Node, Output, getConnections))
    ;
    // Node::Id bindings
    py::class_<Node::Id>(pyNode, "Id", "Node identificator. Unique for every node on a single Pipeline");

    // Node::Connection bindings
    py::class_<Node::Connection>(pyNode, "Connection", DOC(dai, Node, Connection))
        .def_property("outputId", [](Node::Connection& conn) { return conn.outputId; }, [](Node::Connection& conn, Node::Id id) {conn.outputId = id; }, DOC(dai, Node, Connection, outputId))
        .def_property("outputName", [](Node::Connection& conn) { return conn.outputName; }, [](Node::Connection& conn, std::string name) {conn.outputName = name; }, DOC(dai, Node, Connection, outputName))
        .def_property("inputId", [](Node::Connection& conn) { return conn.inputId; }, [](Node::Connection& conn, Node::Id id) {conn.inputId = id; }, DOC(dai, Node, Connection, inputId))
        .def_property("inputName", [](Node::Connection& conn) { return conn.inputName; }, [](Node::Connection& conn, std::string name) {conn.inputName = name; }, DOC(dai, Node, Connection, inputName))
    ;

    pyNode
        .def_readonly("id", &Node::id, DOC(dai, Node, id))
        .def("getName", &Node::getName, DOC(dai, Node, getName))
        .def("getOutputs", &Node::getOutputs, DOC(dai, Node, getOutputs))
        .def("getInputs", &Node::getInputs, DOC(dai, Node, getInputs))
        .def("getAssets", &Node::getAssets, DOC(dai, Node, getAssets))
        .def("getOutputRefs", static_cast<std::vector< Node::Output*> (Node::*)()>(&Node::getOutputRefs), DOC(dai, Node, getOutputRefs), py::return_value_policy::reference_internal)
        .def("getInputRefs", static_cast<std::vector<Node::Input*> (Node::*)()>(&Node::getInputRefs), DOC(dai, Node, getInputRefs), py::return_value_policy::reference_internal)
        .def("getOutputRefs", static_cast<std::vector<const Node::Output*> (Node::*)() const>(&Node::getOutputRefs), DOC(dai, Node, getOutputRefs), py::return_value_policy::reference_internal)
        .def("getInputRefs", static_cast<std::vector<const Node::Input*> (Node::*)() const>(&Node::getInputRefs), DOC(dai, Node, getInputRefs), py::return_value_policy::reference_internal)
        .def("getParentPipeline", py::overload_cast<>(&Node::getParentPipeline), DOC(dai, Node, getParentPipeline))
        .def("getParentPipeline", py::overload_cast<>(&Node::getParentPipeline, py::const_), DOC(dai, Node, getParentPipeline))
    ;

    // MSVC errors out with:
    // Error C2326 'void NodeBindings::bind(pybind11::module &)': function cannot access 'dai::Node::Connection::outputId'
    // ...
    // py::class_<Node::Connection>(pyNode, "Connection")
    //     .def_readwrite("outputId", &dai::Node::Connection::outputId)
    //     .def_readwrite("outputName", &dai::Node::Connection::outputName)
    //     .def_readwrite("inputId", &dai::Node::Connection::inputId)
    //     .def_readwrite("inputName", &dai::Node::Connection::inputName)
    // ;

    //// Bindings for actual nodes

    // XLinkIn node
    py::class_<XLinkIn, Node, std::shared_ptr<XLinkIn>>(m, "XLinkIn", DOC(dai, node, XLinkIn))
        .def_readonly("out", &XLinkIn::out, DOC(dai, node, XLinkIn, out))
        .def("setStreamName", &XLinkIn::setStreamName, py::arg("streamName"), DOC(dai, node, XLinkIn, setStreamName))
        .def("setMaxDataSize", &XLinkIn::setMaxDataSize, py::arg("maxDataSize"), DOC(dai, node, XLinkIn, setMaxDataSize))
        .def("setNumFrames",  &XLinkIn::setNumFrames, py::arg("numFrames"), DOC(dai, node, XLinkIn, setNumFrames))
        .def("getStreamName", &XLinkIn::getStreamName, DOC(dai, node, XLinkIn, getStreamName))
        .def("getMaxDataSize", &XLinkIn::getMaxDataSize, DOC(dai, node, XLinkIn, getMaxDataSize))
        .def("getNumFrames",  &XLinkIn::getNumFrames, DOC(dai, node, XLinkIn, getNumFrames))
        ;

    // XLinkOut node
    py::class_<XLinkOut, Node, std::shared_ptr<XLinkOut>>(m, "XLinkOut", DOC(dai, node, XLinkOut))
        .def_readonly("input", &XLinkOut::input, DOC(dai, node, XLinkOut, input))
        .def("setStreamName", &XLinkOut::setStreamName, py::arg("streamName"), DOC(dai, node, XLinkOut, setStreamName))
        .def("setFpsLimit", &XLinkOut::setFpsLimit, py::arg("fpsLimit"), DOC(dai, node, XLinkOut, setFpsLimit))
        .def("getStreamName", &XLinkOut::getStreamName, DOC(dai, node, XLinkOut, getStreamName))
        .def("getFpsLimit", &XLinkOut::getFpsLimit, DOC(dai, node, XLinkOut, getFpsLimit))
        .def("setMetadataOnly", &XLinkOut::setMetadataOnly, DOC(dai, node, XLinkOut, setMetadataOnly))
        .def("getMetadataOnly", &XLinkOut::getMetadataOnly, DOC(dai, node, XLinkOut, getMetadataOnly))
        ;

    // ColorCamera node
    py::class_<ColorCamera, Node, std::shared_ptr<ColorCamera>>(m, "ColorCamera", DOC(dai, node, ColorCamera))
        .def_readonly("inputConfig", &ColorCamera::inputConfig, DOC(dai, node, ColorCamera, inputConfig))
        .def_readonly("inputControl", &ColorCamera::inputControl, DOC(dai, node, ColorCamera, inputControl))
        .def_readonly("initialControl", &ColorCamera::initialControl, DOC(dai, node, ColorCamera, initialControl))
        .def_readonly("video", &ColorCamera::video, DOC(dai, node, ColorCamera, video))
        .def_readonly("preview", &ColorCamera::preview, DOC(dai, node, ColorCamera, preview))
        .def_readonly("still", &ColorCamera::still, DOC(dai, node, ColorCamera, still))
        .def_readonly("isp", &ColorCamera::isp, DOC(dai, node, ColorCamera, isp))
        .def_readonly("raw", &ColorCamera::raw, DOC(dai, node, ColorCamera, raw))
        .def("setCamId", [](ColorCamera& c, int64_t id) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCamId() is deprecated, use setBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            c.setCamId(id);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("getCamId", [](ColorCamera& c) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "getCamId() is deprecated, use getBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return c.getCamId();
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setBoardSocket", &ColorCamera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, ColorCamera, setBoardSocket))
        .def("getBoardSocket", &ColorCamera::getBoardSocket, DOC(dai, node, ColorCamera, getBoardSocket))
        .def("setImageOrientation", &ColorCamera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, ColorCamera, setImageOrientation))
        .def("getImageOrientation", &ColorCamera::getImageOrientation, DOC(dai, node, ColorCamera, getImageOrientation))
        .def("setColorOrder", &ColorCamera::setColorOrder, py::arg("colorOrder"), DOC(dai, node, ColorCamera, setColorOrder))
        .def("getColorOrder", &ColorCamera::getColorOrder, DOC(dai, node, ColorCamera, getColorOrder))
        .def("setInterleaved", &ColorCamera::setInterleaved, py::arg("interleaved"), DOC(dai, node, ColorCamera, setInterleaved))
        .def("getInterleaved", &ColorCamera::getInterleaved, DOC(dai, node, ColorCamera, getInterleaved))
        .def("setFp16", &ColorCamera::setFp16, py::arg("fp16"), DOC(dai, node, ColorCamera, setFp16))
        .def("getFp16", &ColorCamera::getFp16, DOC(dai, node, ColorCamera, getFp16))
        .def("setPreviewSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setPreviewSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setPreviewSize))
        .def("setPreviewSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setPreviewSize), py::arg("size"), DOC(dai, node, ColorCamera, setPreviewSize, 2))
        .def("setVideoSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setVideoSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setVideoSize))
        .def("setVideoSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setVideoSize), py::arg("size"), DOC(dai, node, ColorCamera, setVideoSize, 2))
        .def("setStillSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setStillSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setStillSize))
        .def("setStillSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setStillSize), py::arg("size"), DOC(dai, node, ColorCamera, setStillSize, 2))
        .def("setResolution", &ColorCamera::setResolution, py::arg("resolution"), DOC(dai, node, ColorCamera, setResolution))
        .def("getResolution", &ColorCamera::getResolution, DOC(dai, node, ColorCamera, getResolution))
        .def("setFps", &ColorCamera::setFps, py::arg("fps"), DOC(dai, node, ColorCamera, setFps))
        .def("getFps", &ColorCamera::getFps, DOC(dai, node, ColorCamera, getFps))
        .def("getPreviewSize", &ColorCamera::getPreviewSize, DOC(dai, node, ColorCamera, getPreviewSize))
        .def("getPreviewWidth", &ColorCamera::getPreviewWidth, DOC(dai, node, ColorCamera, getPreviewWidth))
        .def("getPreviewHeight", &ColorCamera::getPreviewHeight, DOC(dai, node, ColorCamera, getPreviewHeight))
        .def("getVideoSize", &ColorCamera::getVideoSize, DOC(dai, node, ColorCamera, getVideoSize))
        .def("getVideoWidth", &ColorCamera::getVideoWidth, DOC(dai, node, ColorCamera, getVideoWidth))
        .def("getVideoHeight", &ColorCamera::getVideoHeight, DOC(dai, node, ColorCamera, getVideoHeight))
        .def("getStillSize", &ColorCamera::getStillSize, DOC(dai, node, ColorCamera, getStillSize))
        .def("getStillWidth", &ColorCamera::getStillWidth, DOC(dai, node, ColorCamera, getStillWidth))
        .def("getStillHeight", &ColorCamera::getStillHeight, DOC(dai, node, ColorCamera, getStillHeight))
        .def("getResolutionSize", &ColorCamera::getResolutionSize, DOC(dai, node, ColorCamera, getResolutionSize))
        .def("getResolutionWidth", &ColorCamera::getResolutionWidth, DOC(dai, node, ColorCamera, getResolutionWidth))
        .def("getResolutionHeight", &ColorCamera::getResolutionHeight, DOC(dai, node, ColorCamera, getResolutionHeight))
        .def("sensorCenterCrop", &ColorCamera::sensorCenterCrop, DOC(dai, node, ColorCamera, sensorCenterCrop))
        .def("setSensorCrop", &ColorCamera::setSensorCrop, py::arg("x"), py::arg("y"), DOC(dai, node, ColorCamera, setSensorCrop))
        .def("getSensorCrop", &ColorCamera::getSensorCrop, DOC(dai, node, ColorCamera, getSensorCrop))
        .def("getSensorCropX", &ColorCamera::getSensorCropX, DOC(dai, node, ColorCamera, getSensorCropX))
        .def("getSensorCropY", &ColorCamera::getSensorCropY, DOC(dai, node, ColorCamera, getSensorCropY))
        .def("setWaitForConfigInput", &ColorCamera::setWaitForConfigInput, py::arg("wait"), DOC(dai, node, ColorCamera, setWaitForConfigInput))
        .def("getWaitForConfigInput", &ColorCamera::getWaitForConfigInput, DOC(dai, node, ColorCamera, getWaitForConfigInput))
        .def("setPreviewKeepAspectRatio", &ColorCamera::setPreviewKeepAspectRatio, py::arg("keep"), DOC(dai, node, ColorCamera, setPreviewKeepAspectRatio))
        .def("getPreviewKeepAspectRatio", &ColorCamera::getPreviewKeepAspectRatio, DOC(dai, node, ColorCamera, getPreviewKeepAspectRatio))
        .def("setIspScale", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setIspScale), py::arg("numerator"), py::arg("denominator"), DOC(dai, node, ColorCamera, setIspScale))
        .def("setIspScale", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setIspScale), py::arg("scale"), DOC(dai, node, ColorCamera, setIspScale, 2))
        .def("setIspScale", static_cast<void(ColorCamera::*)(int,int,int,int)>(&ColorCamera::setIspScale), py::arg("horizNum"), py::arg("horizDenom"), py::arg("vertNum"), py::arg("vertDenom"), DOC(dai, node, ColorCamera, setIspScale, 3))
        .def("setIspScale", static_cast<void(ColorCamera::*)(std::tuple<int,int>,std::tuple<int,int>)>(&ColorCamera::setIspScale), py::arg("horizScale"), py::arg("vertScale"), DOC(dai, node, ColorCamera, setIspScale, 4))
        .def("getIspSize", &ColorCamera::getIspSize, DOC(dai, node, ColorCamera, getIspSize))
        .def("getIspWidth", &ColorCamera::getIspWidth, DOC(dai, node, ColorCamera, getIspWidth))
        .def("getIspHeight", &ColorCamera::getIspHeight, DOC(dai, node, ColorCamera, getIspHeight))
        ;



    // NeuralNetwork node
    py::class_<NeuralNetwork, Node, std::shared_ptr<NeuralNetwork>>(m, "NeuralNetwork", DOC(dai, node, NeuralNetwork))
        .def_readonly("input", &NeuralNetwork::input, DOC(dai, node, NeuralNetwork, input))
        .def_readonly("out", &NeuralNetwork::out, DOC(dai, node, NeuralNetwork, out))
        .def_readonly("passthrough", &NeuralNetwork::passthrough, DOC(dai, node, NeuralNetwork, passthrough))
        .def("setBlobPath", &NeuralNetwork::setBlobPath, py::arg("path"), DOC(dai, node, NeuralNetwork, setBlobPath))
        .def("setNumPoolFrames", &NeuralNetwork::setNumPoolFrames, py::arg("numFrames"), DOC(dai, node, NeuralNetwork, setNumPoolFrames))
        .def("setNumInferenceThreads", &NeuralNetwork::setNumInferenceThreads, py::arg("numThreads"), DOC(dai, node, NeuralNetwork, setNumInferenceThreads))
        .def("setNumNCEPerInferenceThread", &NeuralNetwork::setNumNCEPerInferenceThread, py::arg("numNCEPerThread"), DOC(dai, node, NeuralNetwork, setNumNCEPerInferenceThread))
        .def("getNumInferenceThreads", &NeuralNetwork::getNumInferenceThreads, DOC(dai, node, NeuralNetwork, getNumInferenceThreads))
        ;


    // ImageManip node
    py::class_<ImageManip, Node, std::shared_ptr<ImageManip>>(m, "ImageManip", DOC(dai, node, ImageManip))
        .def_readonly("inputConfig", &ImageManip::inputConfig, DOC(dai, node, ImageManip, inputConfig))
        .def_readonly("inputImage", &ImageManip::inputImage, DOC(dai, node, ImageManip, inputImage))
        .def_readonly("out", &ImageManip::out, DOC(dai, node, ImageManip, out))
        .def_readonly("initialConfig", &ImageManip::initialConfig, DOC(dai, node, ImageManip, initialConfig))
        // setters

        .def("setCropRect", [](ImageManip& im, float xmin, float ymin, float xmax, float ymax) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCropRect() is deprecated, use initialConfig.setCropRect() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setCropRect(xmin, ymin, xmax, ymax);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setCenterCrop", [](ImageManip& im, float ratio, float whRatio = 1.0f) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCenterCrop() is deprecated, use initialConfig.setCenterCrop() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setCenterCrop(ratio, whRatio);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setResize", [](ImageManip& im, int w, int h) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setResize() is deprecated, use initialConfig.setResize() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setResize(w, h);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setResizeThumbnail", [](ImageManip& im, int w, int h, int bgRed = 0, int bgGreen = 0, int bgBlue = 0) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setResizeThumbnail() is deprecated, use initialConfig.setResizeThumbnail() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setResizeThumbnail(w, h, bgRed, bgGreen, bgBlue);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setFrameType", [](ImageManip& im, dai::RawImgFrame::Type name) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setFrameType() is deprecated, use initialConfig.setFrameType() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setFrameType(name);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setHorizontalFlip", [](ImageManip& im, bool flip) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setHorizontalFlip() is deprecated, use initialConfig.setHorizontalFlip() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setHorizontalFlip(flip);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setKeepAspectRatio", &ImageManip::setKeepAspectRatio, DOC(dai, node, ImageManip, setKeepAspectRatio))

        .def("setWaitForConfigInput", &ImageManip::setWaitForConfigInput, DOC(dai, node, ImageManip, setWaitForConfigInput))
        .def("setNumFramesPool", &ImageManip::setNumFramesPool, DOC(dai, node, ImageManip, setNumFramesPool))
        .def("setMaxOutputFrameSize", &ImageManip::setMaxOutputFrameSize, DOC(dai, node, ImageManip, setMaxOutputFrameSize))
        ;

     // MonoCamera node
    py::class_<MonoCamera, Node, std::shared_ptr<MonoCamera>>(m, "MonoCamera", DOC(dai, node, MonoCamera))
        .def_readonly("inputControl", &MonoCamera::inputControl, DOC(dai, node, MonoCamera, inputControl))
        .def_readonly("out",  &MonoCamera::out, DOC(dai, node, MonoCamera, out))
        .def_readonly("raw",  &MonoCamera::raw, DOC(dai, node, MonoCamera, raw))
        .def_readonly("initialControl",  &MonoCamera::initialControl, DOC(dai, node, MonoCamera, initialControl))
        .def("setCamId", [](MonoCamera& c, int64_t id) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCamId() is deprecated, use setBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            c.setCamId(id);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("getCamId", [](MonoCamera& c) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "getCamId() is deprecated, use getBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return c.getCamId();
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setBoardSocket", &MonoCamera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, MonoCamera, setBoardSocket))
        .def("getBoardSocket", &MonoCamera::getBoardSocket, DOC(dai, node, MonoCamera, getBoardSocket))
        .def("setImageOrientation", &MonoCamera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, MonoCamera, setImageOrientation))
        .def("getImageOrientation", &MonoCamera::getImageOrientation, DOC(dai, node, MonoCamera, getImageOrientation))
        .def("setResolution", &MonoCamera::setResolution, py::arg("resolution"), DOC(dai, node, MonoCamera, setResolution))
        .def("getResolution", &MonoCamera::getResolution, DOC(dai, node, MonoCamera, getResolution))
        .def("setFps",        &MonoCamera::setFps, py::arg("fps"), DOC(dai, node, MonoCamera, setFps))
        .def("getFps",        &MonoCamera::getFps, DOC(dai, node, MonoCamera, getFps))
        .def("getResolutionSize", &MonoCamera::getResolutionSize, DOC(dai, node, MonoCamera, getResolutionSize))
        .def("getResolutionWidth", &MonoCamera::getResolutionWidth, DOC(dai, node, MonoCamera, getResolutionWidth))
        .def("getResolutionHeight", &MonoCamera::getResolutionHeight, DOC(dai, node, MonoCamera, getResolutionHeight))
        ;



    // StereoDepth node
    py::class_<StereoDepth, Node, std::shared_ptr<StereoDepth>>(m, "StereoDepth", DOC(dai, node, StereoDepth))
        .def_readonly("initialConfig",  &StereoDepth::initialConfig, DOC(dai, node, StereoDepth, initialConfig))
        .def_readonly("inputConfig",    &StereoDepth::inputConfig, DOC(dai, node, StereoDepth, inputConfig))
        .def_readonly("left",           &StereoDepth::left, DOC(dai, node, StereoDepth, left))
        .def_readonly("right",          &StereoDepth::right, DOC(dai, node, StereoDepth, right))
        .def_readonly("depth",          &StereoDepth::depth, DOC(dai, node, StereoDepth, depth))
        .def_readonly("disparity",      &StereoDepth::disparity, DOC(dai, node, StereoDepth, disparity))
        .def_readonly("syncedLeft",     &StereoDepth::syncedLeft, DOC(dai, node, StereoDepth, syncedLeft))
        .def_readonly("syncedRight",    &StereoDepth::syncedRight, DOC(dai, node, StereoDepth, syncedRight))
        .def_readonly("rectifiedLeft",  &StereoDepth::rectifiedLeft, DOC(dai, node, StereoDepth, rectifiedLeft))
        .def_readonly("rectifiedRight", &StereoDepth::rectifiedRight, DOC(dai, node, StereoDepth, rectifiedRight))
        .def("loadMeshFiles",           &StereoDepth::loadMeshFiles, py::arg("pathLeft"), py::arg("pathRight"), DOC(dai, node, StereoDepth, loadMeshFiles))
        .def("loadMeshData",            &StereoDepth::loadMeshData, py::arg("dataLeft"), py::arg("dataRight"), DOC(dai, node, StereoDepth, loadMeshData))
        .def("setMeshStep",             &StereoDepth::setMeshStep, py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setMeshStep))
        .def("setInputResolution",      &StereoDepth::setInputResolution, py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setInputResolution))
        .def("setOutputSize",           &StereoDepth::setOutputSize, py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setOutputSize))
        .def("setOutputKeepAspectRatio",&StereoDepth::setOutputKeepAspectRatio, py::arg("keep"), DOC(dai, node, StereoDepth, setOutputKeepAspectRatio))
        .def("setDepthAlign",           static_cast<void(StereoDepth::*)(StereoDepthProperties::DepthAlign)>(&StereoDepth::setDepthAlign), py::arg("align"), DOC(dai, node, StereoDepth, setDepthAlign))
        .def("setDepthAlign",           static_cast<void(StereoDepth::*)(CameraBoardSocket)>(&StereoDepth::setDepthAlign), py::arg("camera"), DOC(dai, node, StereoDepth, setDepthAlign, 2))
        .def("setRectification",        &StereoDepth::setRectification, py::arg("enable"), DOC(dai, node, StereoDepth, setRectification))
        .def("setLeftRightCheck",       &StereoDepth::setLeftRightCheck, py::arg("enable"), DOC(dai, node, StereoDepth, setLeftRightCheck))
        .def("setSubpixel",             &StereoDepth::setSubpixel, py::arg("enable"), DOC(dai, node, StereoDepth, setSubpixel))
        .def("setExtendedDisparity",    &StereoDepth::setExtendedDisparity, py::arg("enable"), DOC(dai, node, StereoDepth, setExtendedDisparity))
        .def("setRectifyEdgeFillColor", &StereoDepth::setRectifyEdgeFillColor, py::arg("color"), DOC(dai, node, StereoDepth, setRectifyEdgeFillColor))
        .def("setRectifyMirrorFrame",   &StereoDepth::setRectifyMirrorFrame, py::arg("enable"), DOC(dai, node, StereoDepth, setRectifyMirrorFrame))
        .def("setConfidenceThreshold", [](StereoDepth& s, int confThr) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setConfidenceThreshold() is deprecated, Use 'initialConfig.setConfidenceThreshold()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setConfidenceThreshold(confThr);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setConfidenceThreshold))
        .def("setMedianFilter", [](StereoDepth& s, dai::MedianFilter median) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setMedianFilter() is deprecated, Use 'initialConfig.setMedianFilter()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setMedianFilter(median);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setMedianFilter))
        .def("setOutputRectified", [](StereoDepth& s, bool enable) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setOutputRectified() is deprecated, the output is auto-enabled if used.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setOutputRectified(enable);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setOutputDepth", [](StereoDepth& s, bool enable) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setOutputDepth() is deprecated, the output is auto-enabled if used.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setOutputDepth(enable);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("loadCalibrationFile", [](StereoDepth& s, std::string path){
            PyErr_WarnEx(PyExc_DeprecationWarning, "loadCalibrationFile() is deprecated, Use 'Pipeline.setCalibrationData()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.loadCalibrationFile(path);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("loadCalibrationData", [](StereoDepth& s, std::vector<std::uint8_t> data){
            PyErr_WarnEx(PyExc_DeprecationWarning, "loadCalibrationData() is deprecated, Use 'Pipeline.setCalibrationData()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.loadCalibrationData(data);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setEmptyCalibration", [](StereoDepth& s){
            PyErr_WarnEx(PyExc_DeprecationWarning, "setEmptyCalibration() is deprecated, Use 'setRectification(False)' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setEmptyCalibration();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setEmptyCalibration))
        .def("getMaxDisparity", &StereoDepth::getMaxDisparity, DOC(dai, node, StereoDepth, getMaxDisparity))
        ;

    // VideoEncoder node
    py::class_<VideoEncoder, Node, std::shared_ptr<VideoEncoder>>(m, "VideoEncoder", DOC(dai, node, VideoEncoder))
        .def_readonly("input", &VideoEncoder::input, DOC(dai, node, VideoEncoder, input), DOC(dai, node, VideoEncoder, input))
        .def_readonly("bitstream", &VideoEncoder::bitstream, DOC(dai, node, VideoEncoder, bitstream), DOC(dai, node, VideoEncoder, bitstream))
        .def("setDefaultProfilePreset", static_cast<void(VideoEncoder::*)(int, int, float, VideoEncoderProperties::Profile)>(&VideoEncoder::setDefaultProfilePreset), py::arg("width"), py::arg("height"), py::arg("fps"), py::arg("profile"), DOC(dai, node, VideoEncoder, setDefaultProfilePreset))
        .def("setDefaultProfilePreset", static_cast<void(VideoEncoder::*)(std::tuple<int,int>, float, VideoEncoderProperties::Profile)>(&VideoEncoder::setDefaultProfilePreset), py::arg("size"), py::arg("fps"), py::arg("profile"), DOC(dai, node, VideoEncoder, setDefaultProfilePreset, 2))
        .def("setNumFramesPool", &VideoEncoder::setNumFramesPool, py::arg("frames"), DOC(dai, node, VideoEncoder, setNumFramesPool))
        .def("getNumFramesPool", &VideoEncoder::getNumFramesPool, DOC(dai, node, VideoEncoder, getNumFramesPool))
        .def("setRateControlMode", &VideoEncoder::setRateControlMode, py::arg("mode"), DOC(dai, node, VideoEncoder, setRateControlMode))
        .def("setProfile", static_cast<void(VideoEncoder::*)(std::tuple<int,int>, VideoEncoder::Properties::Profile)>(&VideoEncoder::setProfile), py::arg("size"), py::arg("profile"), DOC(dai, node, VideoEncoder, setProfile))
        .def("setProfile", static_cast<void(VideoEncoder::*)(int, int, VideoEncoder::Properties::Profile)>(&VideoEncoder::setProfile), py::arg("width"), py::arg("height"), py::arg("profile"), DOC(dai, node, VideoEncoder, setProfile, 2))
        .def("setBitrate", &VideoEncoder::setBitrate, py::arg("bitrateKbps"), DOC(dai, node, VideoEncoder, setBitrate))
        .def("setBitrateKbps", &VideoEncoder::setBitrateKbps, py::arg("bitrateKbps"), DOC(dai, node, VideoEncoder, setBitrateKbps))
        .def("setKeyframeFrequency", &VideoEncoder::setKeyframeFrequency, py::arg("freq"), DOC(dai, node, VideoEncoder, setKeyframeFrequency))
        //.def("setMaxBitrate", &VideoEncoder::setMaxBitrate)
        .def("setNumBFrames", &VideoEncoder::setNumBFrames, py::arg("numBFrames"), DOC(dai, node, VideoEncoder, setNumBFrames))
        .def("setQuality", &VideoEncoder::setQuality, py::arg("quality"), DOC(dai, node, VideoEncoder, setQuality))
        .def("setLossless", &VideoEncoder::setLossless, DOC(dai, node, VideoEncoder, setLossless))
        .def("setFrameRate", &VideoEncoder::setFrameRate, py::arg("frameRate"), DOC(dai, node, VideoEncoder, setFrameRate))
        .def("getRateControlMode", &VideoEncoder::getRateControlMode, DOC(dai, node, VideoEncoder, getRateControlMode))
        .def("getProfile", &VideoEncoder::getProfile, DOC(dai, node, VideoEncoder, getProfile))
        .def("getBitrate", &VideoEncoder::getBitrate, DOC(dai, node, VideoEncoder, getBitrate))
        .def("getBitrateKbps", &VideoEncoder::getBitrateKbps, DOC(dai, node, VideoEncoder, getBitrateKbps))
        .def("getKeyframeFrequency", &VideoEncoder::getKeyframeFrequency, DOC(dai, node, VideoEncoder, getKeyframeFrequency))
        //.def("getMaxBitrate", &VideoEncoder::getMaxBitrate)
        .def("getNumBFrames", &VideoEncoder::getNumBFrames, DOC(dai, node, VideoEncoder, getNumBFrames))
        .def("getQuality", &VideoEncoder::getQuality, DOC(dai, node, VideoEncoder, getQuality))
        .def("getWidth", &VideoEncoder::getWidth, DOC(dai, node, VideoEncoder, getWidth))
        .def("getHeight", &VideoEncoder::getHeight, DOC(dai, node, VideoEncoder, getHeight))
        .def("getFrameRate", &VideoEncoder::getFrameRate, DOC(dai, node, VideoEncoder, getFrameRate))
        .def("getSize", &VideoEncoder::getSize, DOC(dai, node, VideoEncoder, getSize))
        .def("getLossless", &VideoEncoder::getLossless, DOC(dai, node, VideoEncoder, getLossless))
    ;

    // SPIOut node
    py::class_<SPIOut, Node, std::shared_ptr<SPIOut>>(m, "SPIOut", DOC(dai, node, SPIOut))
        .def_readonly("input", &SPIOut::input, DOC(dai, node, SPIOut, input))
        .def("setStreamName", &SPIOut::setStreamName, py::arg("name"), DOC(dai, node, SPIOut, setStreamName))
        .def("setBusId", &SPIOut::setBusId, py::arg("id"), DOC(dai, node, SPIOut, setBusId))
        ;

    py::class_<DetectionNetwork, NeuralNetwork, std::shared_ptr<DetectionNetwork>>(m, "DetectionNetwork", DOC(dai, node, DetectionNetwork))
        .def_readonly("input", &DetectionNetwork::input, DOC(dai, node, DetectionNetwork, input))
        .def_readonly("out", &DetectionNetwork::out, DOC(dai, node, DetectionNetwork, out))
        .def_readonly("passthrough", &DetectionNetwork::passthrough, DOC(dai, node, DetectionNetwork, passthrough))
        .def("setConfidenceThreshold", &DetectionNetwork::setConfidenceThreshold, py::arg("thresh"), DOC(dai, node, DetectionNetwork, setConfidenceThreshold))
        ;

    // MobileNetDetectionNetwork node
    py::class_<MobileNetDetectionNetwork, DetectionNetwork, std::shared_ptr<MobileNetDetectionNetwork>>(m, "MobileNetDetectionNetwork", DOC(dai, node, MobileNetDetectionNetwork))
        ;

    // YoloDetectionNetwork node
    py::class_<YoloDetectionNetwork, DetectionNetwork, std::shared_ptr<YoloDetectionNetwork>>(m, "YoloDetectionNetwork", DOC(dai, node, YoloDetectionNetwork))
        .def("setNumClasses", &YoloDetectionNetwork::setNumClasses, py::arg("numClasses"), DOC(dai, node, YoloDetectionNetwork, setNumClasses))
        .def("setCoordinateSize", &YoloDetectionNetwork::setCoordinateSize, py::arg("coordinates"), DOC(dai, node, YoloDetectionNetwork, setCoordinateSize))
        .def("setAnchors", &YoloDetectionNetwork::setAnchors, py::arg("anchors"), DOC(dai, node, YoloDetectionNetwork, setAnchors))
        .def("setAnchorMasks", &YoloDetectionNetwork::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, YoloDetectionNetwork, setAnchorMasks))
        .def("setIouThreshold", &YoloDetectionNetwork::setIouThreshold, py::arg("thresh"), DOC(dai, node, YoloDetectionNetwork, setIouThreshold))
        ;

    py::class_<SpatialDetectionNetwork, DetectionNetwork, std::shared_ptr<SpatialDetectionNetwork>>(m, "SpatialDetectionNetwork", DOC(dai, node, SpatialDetectionNetwork))
        .def_readonly("input", &SpatialDetectionNetwork::input, DOC(dai, node, SpatialDetectionNetwork, input))
        .def_readonly("inputDepth", &SpatialDetectionNetwork::inputDepth, DOC(dai, node, SpatialDetectionNetwork, inputDepth))
        .def_readonly("out", &SpatialDetectionNetwork::out, DOC(dai, node, SpatialDetectionNetwork, out))
        .def_readonly("boundingBoxMapping", &SpatialDetectionNetwork::boundingBoxMapping, DOC(dai, node, SpatialDetectionNetwork, boundingBoxMapping))
        .def_readonly("passthrough", &SpatialDetectionNetwork::passthrough, DOC(dai, node, SpatialDetectionNetwork, passthrough))
        .def_readonly("passthroughDepth", &SpatialDetectionNetwork::passthroughDepth, DOC(dai, node, SpatialDetectionNetwork, passthroughDepth))

        .def("setBoundingBoxScaleFactor", &SpatialDetectionNetwork::setBoundingBoxScaleFactor, py::arg("scaleFactor"), DOC(dai, node, SpatialDetectionNetwork, setBoundingBoxScaleFactor))
        .def("setDepthLowerThreshold", &SpatialDetectionNetwork::setDepthLowerThreshold, py::arg("lowerThreshold"), DOC(dai, node, SpatialDetectionNetwork, setDepthLowerThreshold))
        .def("setDepthUpperThreshold", &SpatialDetectionNetwork::setDepthUpperThreshold, py::arg("upperThreshold"), DOC(dai, node, SpatialDetectionNetwork, setDepthUpperThreshold))
        ;

    py::class_<MobileNetSpatialDetectionNetwork, SpatialDetectionNetwork, std::shared_ptr<MobileNetSpatialDetectionNetwork>>(m, "MobileNetSpatialDetectionNetwork", DOC(dai, node, MobileNetSpatialDetectionNetwork))
        ;

    // YoloSpatialDetectionNetwork node
    py::class_<YoloSpatialDetectionNetwork, SpatialDetectionNetwork, std::shared_ptr<YoloSpatialDetectionNetwork>>(m, "YoloSpatialDetectionNetwork", DOC(dai, node, YoloSpatialDetectionNetwork))
        .def("setNumClasses", &YoloSpatialDetectionNetwork::setNumClasses, py::arg("numClasses"), DOC(dai, node, YoloSpatialDetectionNetwork, setNumClasses))
        .def("setCoordinateSize", &YoloSpatialDetectionNetwork::setCoordinateSize, py::arg("coordinates"), DOC(dai, node, YoloSpatialDetectionNetwork, setCoordinateSize))
        .def("setAnchors", &YoloSpatialDetectionNetwork::setAnchors, py::arg("anchors"), DOC(dai, node, YoloSpatialDetectionNetwork, setAnchors))
        .def("setAnchorMasks", &YoloSpatialDetectionNetwork::setAnchorMasks, py::arg("anchorMasks"), DOC(dai, node, YoloSpatialDetectionNetwork, setAnchorMasks))
        .def("setIouThreshold", &YoloSpatialDetectionNetwork::setIouThreshold, py::arg("thresh"), DOC(dai, node, YoloSpatialDetectionNetwork, setIouThreshold))
        ;

    // SpatialLocationCalculator node
    py::class_<SpatialLocationCalculator, Node, std::shared_ptr<SpatialLocationCalculator>>(m, "SpatialLocationCalculator", DOC(dai, node, SpatialLocationCalculator))
        .def_readonly("inputConfig", &SpatialLocationCalculator::inputConfig, DOC(dai, node, SpatialLocationCalculator, inputConfig))
        .def_readonly("inputDepth", &SpatialLocationCalculator::inputDepth, DOC(dai, node, SpatialLocationCalculator, inputDepth))
        .def_readonly("out", &SpatialLocationCalculator::out, DOC(dai, node, SpatialLocationCalculator, out))
        .def_readonly("passthroughDepth", &SpatialLocationCalculator::passthroughDepth, DOC(dai, node, SpatialLocationCalculator, passthroughDepth))
        .def_readonly("initialConfig", &SpatialLocationCalculator::initialConfig, DOC(dai, node, SpatialLocationCalculator, initialConfig))
        .def("setWaitForConfigInput", &SpatialLocationCalculator::setWaitForConfigInput, py::arg("wait"), DOC(dai, node, SpatialLocationCalculator, setWaitForConfigInput))
        ;

    // SystemLogger node
    py::class_<SystemLogger, Node, std::shared_ptr<SystemLogger>>(m, "SystemLogger", DOC(dai, node, SystemLogger))
        .def_readonly("out", &SystemLogger::out, DOC(dai, node, SystemLogger, out))
        .def("setRate", &SystemLogger::setRate, py::arg("hz"), DOC(dai, node, SystemLogger, setRate))
        ;

    // NeuralNetwork node
    py::class_<ObjectTracker, Node, std::shared_ptr<ObjectTracker>>(m, "ObjectTracker", DOC(dai, node, ObjectTracker))
        .def_readonly("inputTrackerFrame", &ObjectTracker::inputTrackerFrame, DOC(dai, node, ObjectTracker, inputTrackerFrame))
        .def_readonly("inputDetectionFrame", &ObjectTracker::inputDetectionFrame, DOC(dai, node, ObjectTracker, inputDetectionFrame))
        .def_readonly("inputDetections", &ObjectTracker::inputDetections, DOC(dai, node, ObjectTracker, inputDetections))
        .def_readonly("out", &ObjectTracker::out, DOC(dai, node, ObjectTracker, out))
        .def_readonly("passthroughTrackerFrame", &ObjectTracker::passthroughTrackerFrame, DOC(dai, node, ObjectTracker, passthroughTrackerFrame))
        .def_readonly("passthroughDetectionFrame", &ObjectTracker::passthroughDetectionFrame, DOC(dai, node, ObjectTracker, passthroughDetectionFrame))
        .def_readonly("passthroughDetections", &ObjectTracker::passthroughDetections, DOC(dai, node, ObjectTracker, passthroughDetections))

        .def("setTrackerThreshold", &ObjectTracker::setTrackerThreshold, py::arg("threshold"), DOC(dai, node, ObjectTracker, setTrackerThreshold))
        .def("setMaxObjectsToTrack", &ObjectTracker::setMaxObjectsToTrack, py::arg("maxObjectsToTrack"), DOC(dai, node, ObjectTracker, setMaxObjectsToTrack))
        .def("setDetectionLabelsToTrack", &ObjectTracker::setDetectionLabelsToTrack, py::arg("labels"), DOC(dai, node, ObjectTracker, setDetectionLabelsToTrack))
        .def("setTrackerType", &ObjectTracker::setTrackerType, py::arg("type"), DOC(dai, node, ObjectTracker, setTrackerType))
        .def("setTrackerIdAssigmentPolicy", &ObjectTracker::setTrackerIdAssigmentPolicy, py::arg("type"), DOC(dai, node, ObjectTracker, setTrackerIdAssigmentPolicy))
        ;

    // IMU node
    py::class_<IMU, Node, std::shared_ptr<IMU>>(m, "IMU", DOC(dai, node, IMU))
        .def_readonly("out", &IMU::out, DOC(dai, node, IMU, out))
        .def("enableIMUSensor", static_cast<void(IMU::*)(IMUSensorConfig imuSensor)>(&IMU::enableIMUSensor), py::arg("sensorConfig"), DOC(dai, node, IMU, enableIMUSensor))
        .def("enableIMUSensor", static_cast<void(IMU::*)(const std::vector<IMUSensorConfig>& imuSensors)>(&IMU::enableIMUSensor), py::arg("sensorConfigs"), DOC(dai, node, IMU, enableIMUSensor, 2))
        .def("enableIMUSensor", static_cast<void(IMU::*)(IMUSensor sensor, uint32_t reportRate)>(&IMU::enableIMUSensor), py::arg("sensor"), py::arg("reportRate"), DOC(dai, node, IMU, enableIMUSensor, 3))
        .def("enableIMUSensor", static_cast<void(IMU::*)(const std::vector<IMUSensor>& sensors, uint32_t reportRate)>(&IMU::enableIMUSensor), py::arg("sensors"), py::arg("reportRate"), DOC(dai, node, IMU, enableIMUSensor, 4))
        .def("setBatchReportThreshold", &IMU::setBatchReportThreshold, py::arg("batchReportThreshold"), DOC(dai, node, IMU, setBatchReportThreshold))
        .def("getBatchReportThreshold", &IMU::getBatchReportThreshold, DOC(dai, node, IMU, getBatchReportThreshold))
        .def("setMaxBatchReports", &IMU::setMaxBatchReports, py::arg("maxBatchReports"), DOC(dai, node, IMU, setMaxBatchReports))
        .def("getMaxBatchReports", &IMU::getMaxBatchReports, DOC(dai, node, IMU, getMaxBatchReports))
        ;

    // EdgeDetector node
    py::class_<EdgeDetector, Node, std::shared_ptr<EdgeDetector>>(m, "EdgeDetector", DOC(dai, node, EdgeDetector))
        .def_readonly("initialConfig", &EdgeDetector::initialConfig, DOC(dai, node, EdgeDetector, initialConfig))
        .def_readonly("inputConfig", &EdgeDetector::inputConfig, DOC(dai, node, EdgeDetector, inputConfig))
        .def_readonly("inputImage", &EdgeDetector::inputImage, DOC(dai, node, EdgeDetector, inputImage))
        .def_readonly("outputImage", &EdgeDetector::outputImage, DOC(dai, node, EdgeDetector, outputImage))
        .def("setWaitForConfigInput", &EdgeDetector::setWaitForConfigInput, DOC(dai, node, EdgeDetector, setWaitForConfigInput))
        .def("setNumFramesPool", &EdgeDetector::setNumFramesPool, DOC(dai, node, EdgeDetector, setNumFramesPool))
        .def("setMaxOutputFrameSize", &EdgeDetector::setMaxOutputFrameSize, DOC(dai, node, EdgeDetector, setMaxOutputFrameSize))
        ;

    ////////////////////////////////////
    // Node properties bindings
    ////////////////////////////////////
    py::class_<ColorCameraProperties> colorCameraProperties(m, "ColorCameraProperties", DOC(dai, ColorCameraProperties));
    colorCameraProperties
        .def_readwrite("initialControl", &ColorCameraProperties::initialControl)
        .def_readwrite("boardSocket", &ColorCameraProperties::boardSocket)
        .def_readwrite("colorOrder", &ColorCameraProperties::colorOrder)
        .def_readwrite("interleaved", &ColorCameraProperties::interleaved)
        .def_readwrite("previewHeight", &ColorCameraProperties::previewHeight)
        .def_readwrite("previewWidth", &ColorCameraProperties::previewWidth)
        .def_readwrite("videoHeight", &ColorCameraProperties::videoHeight)
        .def_readwrite("videoWidth", &ColorCameraProperties::videoWidth)
        .def_readwrite("stillHeight", &ColorCameraProperties::stillHeight)
        .def_readwrite("stillWidth", &ColorCameraProperties::stillWidth)
        .def_readwrite("resolution", &ColorCameraProperties::resolution)
        .def_readwrite("fps", &ColorCameraProperties::fps)
        .def_readwrite("sensorCropX", &ColorCameraProperties::sensorCropX)
        .def_readwrite("sensorCropY", &ColorCameraProperties::sensorCropY)
    ;

    py::enum_<ColorCameraProperties::SensorResolution>(colorCameraProperties, "SensorResolution", DOC(dai, ColorCameraProperties, SensorResolution))
        .value("THE_1080_P", ColorCameraProperties::SensorResolution::THE_1080_P)
        .value("THE_4_K", ColorCameraProperties::SensorResolution::THE_4_K)
        .value("THE_12_MP", ColorCameraProperties::SensorResolution::THE_12_MP)
        ;

    py::enum_<ColorCameraProperties::ColorOrder>(colorCameraProperties, "ColorOrder", DOC(dai, ColorCameraProperties, ColorOrder))
        .value("BGR", ColorCameraProperties::ColorOrder::BGR)
        .value("RGB", ColorCameraProperties::ColorOrder::RGB)
        ;
    // ALIAS
    m.attr("ColorCamera").attr("Properties") = colorCameraProperties;



    // MonoCamera props
    py::class_<MonoCameraProperties> monoCameraProperties(m, "MonoCameraProperties", DOC(dai, MonoCameraProperties));
    monoCameraProperties
        .def_readwrite("initialControl", &MonoCameraProperties::initialControl)
        .def_readwrite("boardSocket", &MonoCameraProperties::boardSocket)
        .def_readwrite("resolution", &MonoCameraProperties::resolution)
        .def_readwrite("fps",  &MonoCameraProperties::fps)
    ;

    py::enum_<MonoCameraProperties::SensorResolution>(monoCameraProperties, "SensorResolution", DOC(dai, MonoCameraProperties, SensorResolution))
        .value("THE_720_P", MonoCameraProperties::SensorResolution::THE_720_P)
        .value("THE_800_P", MonoCameraProperties::SensorResolution::THE_800_P)
        .value("THE_400_P", MonoCameraProperties::SensorResolution::THE_400_P)
        ;
    // ALIAS
    m.attr("MonoCamera").attr("Properties") = monoCameraProperties;


    // StereoDepth props
    py::class_<StereoDepthProperties> stereoDepthProperties(m, "StereoDepthProperties", DOC(dai, StereoDepthProperties));
    stereoDepthProperties
        .def_readwrite("calibration",             &StereoDepthProperties::calibration)
        .def_readwrite("initialConfig",           &StereoDepthProperties::initialConfig)
        .def_readwrite("inputConfigSync",         &StereoDepthProperties::inputConfigSync)
        .def_readwrite("depthAlign",              &StereoDepthProperties::depthAlign)
        .def_readwrite("depthAlignCamera",        &StereoDepthProperties::depthAlignCamera)
        .def_readwrite("enableLeftRightCheck",    &StereoDepthProperties::enableLeftRightCheck)
        .def_readwrite("enableSubpixel",          &StereoDepthProperties::enableSubpixel)
        .def_readwrite("enableExtendedDisparity", &StereoDepthProperties::enableExtendedDisparity)
        .def_readwrite("rectifyMirrorFrame",      &StereoDepthProperties::rectifyMirrorFrame)
        .def_readwrite("rectifyEdgeFillColor",    &StereoDepthProperties::rectifyEdgeFillColor)
        .def_readwrite("width",                   &StereoDepthProperties::width)
        .def_readwrite("height",                  &StereoDepthProperties::height)
        .def_readwrite("outWidth",                &StereoDepthProperties::outWidth, DOC(dai, StereoDepthProperties, outWidth))
        .def_readwrite("outHeight",               &StereoDepthProperties::outHeight, DOC(dai, StereoDepthProperties, outHeight))
        .def_readwrite("outKeepAspectRatio",      &StereoDepthProperties::outKeepAspectRatio, DOC(dai, StereoDepthProperties, outKeepAspectRatio))
        .def_readwrite("mesh",                    &StereoDepthProperties::mesh, DOC(dai, StereoDepthProperties, mesh))
        ;

    py::enum_<MedianFilter> medianFilter(m, "MedianFilter", DOC(dai, MedianFilter));
    medianFilter
        .value("MEDIAN_OFF", MedianFilter::MEDIAN_OFF)
        .value("KERNEL_3x3", MedianFilter::KERNEL_3x3)
        .value("KERNEL_5x5", MedianFilter::KERNEL_5x5)
        .value("KERNEL_7x7", MedianFilter::KERNEL_7x7)
        ;

    py::enum_<StereoDepthProperties::DepthAlign>(stereoDepthProperties, "DepthAlign")
        .value("RECTIFIED_RIGHT", StereoDepthProperties::DepthAlign::RECTIFIED_RIGHT)
        .value("RECTIFIED_LEFT",  StereoDepthProperties::DepthAlign::RECTIFIED_LEFT)
        .value("CENTER",          StereoDepthProperties::DepthAlign::CENTER)
        ;

    py::class_<StereoDepthConfigData> stereoDepthConfigData(m, "StereoDepthConfigData", DOC(dai, StereoDepthConfigData));
    stereoDepthConfigData
        .def(py::init<>())
        .def_readwrite("median", &StereoDepthConfigData::median,  DOC(dai, StereoDepthConfigData, median))
        .def_readwrite("confidenceThreshold", &StereoDepthConfigData::confidenceThreshold,  DOC(dai, StereoDepthConfigData, confidenceThreshold))
        .def_readwrite("bilateralSigmaValue", &StereoDepthConfigData::bilateralSigmaValue,  DOC(dai, StereoDepthConfigData, bilateralSigmaValue))
        .def_readwrite("leftRightCheckThreshold", &StereoDepthConfigData::leftRightCheckThreshold,  DOC(dai, StereoDepthConfigData, leftRightCheckThreshold))
        ;

    m.attr("StereoDepthProperties").attr("MedianFilter") = medianFilter;
    m.attr("StereoDepthConfigData").attr("MedianFilter") = medianFilter;

    // ALIAS
    m.attr("StereoDepth").attr("Properties") = stereoDepthProperties;



    // VideoEncoder props
    py::class_<VideoEncoderProperties> videoEncoderProperties(m, "VideoEncoderProperties", DOC(dai, VideoEncoderProperties));
    videoEncoderProperties
        .def_readwrite("bitrate", &VideoEncoderProperties::bitrate)
        .def_readwrite("keyframeFrequency", &VideoEncoderProperties::keyframeFrequency)
        .def_readwrite("maxBitrate", &VideoEncoderProperties::maxBitrate)
        .def_readwrite("numBFrames", &VideoEncoderProperties::numBFrames)
        .def_readwrite("numFramesPool", &VideoEncoderProperties::numFramesPool)
        .def_readwrite("profile", &VideoEncoderProperties::profile)
        .def_readwrite("quality", &VideoEncoderProperties::quality)
        .def_readwrite("rateCtrlMode", &VideoEncoderProperties::rateCtrlMode)
        .def_readwrite("width", &VideoEncoderProperties::width)
        .def_readwrite("height", &VideoEncoderProperties::height)
        ;

    py::enum_<VideoEncoderProperties::Profile>(videoEncoderProperties, "Profile", DOC(dai, VideoEncoderProperties, Profile))
        .value("H264_BASELINE", VideoEncoderProperties::Profile::H264_BASELINE)
        .value("H264_HIGH", VideoEncoderProperties::Profile::H264_HIGH)
        .value("H264_MAIN", VideoEncoderProperties::Profile::H264_MAIN)
        .value("H265_MAIN", VideoEncoderProperties::Profile::H265_MAIN)
        .value("MJPEG", VideoEncoderProperties::Profile::MJPEG)
        ;

    py::enum_<VideoEncoderProperties::RateControlMode>(videoEncoderProperties, "RateControlMode", DOC(dai, VideoEncoderProperties, RateControlMode))
        .value("CBR", VideoEncoderProperties::RateControlMode::CBR)
        .value("VBR", VideoEncoderProperties::RateControlMode::VBR)
        ;
    // ALIAS
    m.attr("VideoEncoder").attr("Properties") = videoEncoderProperties;




    py::class_<SystemLoggerProperties>(m, "SystemLoggerProperties", DOC(dai, SystemLoggerProperties))
        .def_readwrite("rateHz", &SystemLoggerProperties::rateHz)
        ;

    py::class_<NeuralNetworkProperties, std::shared_ptr<NeuralNetworkProperties>> neuralNetworkProperties(m, "NeuralNetworkProperties", DOC(dai, NeuralNetworkProperties));
    neuralNetworkProperties
        .def_readwrite("blobSize", &NeuralNetworkProperties::blobSize)
        .def_readwrite("blobUri", &NeuralNetworkProperties::blobUri)
        .def_readwrite("numFrames", &NeuralNetworkProperties::numFrames)
        .def_readwrite("numThreads", &NeuralNetworkProperties::numThreads)
        .def_readwrite("numNCEPerThread", &NeuralNetworkProperties::numNCEPerThread)
        ;
    m.attr("NeuralNetwork").attr("Properties") = neuralNetworkProperties;


    py::class_<DetectionNetworkProperties, NeuralNetworkProperties, std::shared_ptr<DetectionNetworkProperties>> detectionNetworkProperties(m, "DetectionNetworkProperties", DOC(dai, DetectionNetworkProperties));
    detectionNetworkProperties
        .def_readwrite("nnFamily", &DetectionNetworkProperties::nnFamily)
        .def_readwrite("confidenceThreshold", &DetectionNetworkProperties::confidenceThreshold)
        .def_readwrite("classes", &DetectionNetworkProperties::classes)
        .def_readwrite("coordinates", &DetectionNetworkProperties::coordinates)
        .def_readwrite("anchors", &DetectionNetworkProperties::anchors)
        .def_readwrite("anchorMasks", &DetectionNetworkProperties::anchorMasks)
        .def_readwrite("iouThreshold", &DetectionNetworkProperties::iouThreshold)
        ;
    // ALIAS
    m.attr("DetectionNetwork").attr("Properties") = detectionNetworkProperties;


    py::class_<SpatialDetectionNetworkProperties, DetectionNetworkProperties, std::shared_ptr<SpatialDetectionNetworkProperties>> spatialDetectionNetworkProperties(m, "SpatialDetectionNetworkProperties", DOC(dai, SpatialDetectionNetworkProperties));
    spatialDetectionNetworkProperties
        .def_readwrite("detectedBBScaleFactor", &SpatialDetectionNetworkProperties::detectedBBScaleFactor)
        .def_readwrite("depthThresholds", &SpatialDetectionNetworkProperties::depthThresholds)
        ;
    // ALIAS
    m.attr("SpatialDetectionNetwork").attr("Properties") = spatialDetectionNetworkProperties;


    py::class_<SpatialLocationCalculatorProperties> spatialLocationCalculatorProperties(m, "SpatialLocationCalculatorProperties", DOC(dai, SpatialLocationCalculatorProperties));
    spatialLocationCalculatorProperties
        .def_readwrite("roiConfig", &SpatialLocationCalculatorProperties::roiConfig)
        .def_readwrite("inputConfigSync", &SpatialLocationCalculatorProperties::inputConfigSync)
        ;
    m.attr("SpatialLocationCalculator").attr("Properties") = spatialLocationCalculatorProperties;


    py::enum_<TrackerType>(m, "TrackerType")
        .value("ZERO_TERM_IMAGELESS", TrackerType::ZERO_TERM_IMAGELESS)
        .value("ZERO_TERM_COLOR_HISTOGRAM", TrackerType::ZERO_TERM_COLOR_HISTOGRAM)
    ;

    py::enum_<TrackerIdAssigmentPolicy>(m, "TrackerIdAssigmentPolicy")
        .value("UNIQUE_ID", TrackerIdAssigmentPolicy::UNIQUE_ID)
        .value("SMALLEST_ID", TrackerIdAssigmentPolicy::SMALLEST_ID)
    ;


    py::class_<ObjectTrackerProperties, std::shared_ptr<ObjectTrackerProperties>> objectTrackerProperties(m, "ObjectTrackerProperties", DOC(dai, ObjectTrackerProperties));
    objectTrackerProperties
        .def_readwrite("trackerThreshold", &ObjectTrackerProperties::trackerThreshold)
        .def_readwrite("maxObjectsToTrack", &ObjectTrackerProperties::maxObjectsToTrack)
        .def_readwrite("detectionLabelsToTrack", &ObjectTrackerProperties::detectionLabelsToTrack)
        .def_readwrite("trackerType", &ObjectTrackerProperties::trackerType)
        .def_readwrite("trackerIdAssigmentPolicy", &ObjectTrackerProperties::trackerIdAssigmentPolicy)
        ;
    m.attr("ObjectTracker").attr("Properties") = objectTrackerProperties;

    py::enum_<IMUSensor>(m, "IMUSensor", DOC(dai, IMUSensor))
        .value("ACCELEROMETER_RAW", IMUSensor::ACCELEROMETER_RAW, DOC(dai, IMUSensor, ACCELEROMETER_RAW))
        .value("ACCELEROMETER", IMUSensor::ACCELEROMETER, DOC(dai, IMUSensor, ACCELEROMETER))
        .value("LINEAR_ACCELERATION", IMUSensor::LINEAR_ACCELERATION, DOC(dai, IMUSensor, LINEAR_ACCELERATION))
        .value("GRAVITY", IMUSensor::GRAVITY, DOC(dai, IMUSensor, GRAVITY))
        .value("GYROSCOPE_RAW", IMUSensor::GYROSCOPE_RAW, DOC(dai, IMUSensor, GYROSCOPE_RAW))
        .value("GYROSCOPE_CALIBRATED", IMUSensor::GYROSCOPE_CALIBRATED, DOC(dai, IMUSensor, GYROSCOPE_CALIBRATED))
        .value("GYROSCOPE_UNCALIBRATED", IMUSensor::GYROSCOPE_UNCALIBRATED, DOC(dai, IMUSensor, GYROSCOPE_UNCALIBRATED))
        .value("MAGNETOMETER_RAW", IMUSensor::MAGNETOMETER_RAW, DOC(dai, IMUSensor, MAGNETOMETER_RAW))
        .value("MAGNETOMETER_CALIBRATED", IMUSensor::MAGNETOMETER_CALIBRATED, DOC(dai, IMUSensor, MAGNETOMETER_CALIBRATED))
        .value("MAGNETOMETER_UNCALIBRATED", IMUSensor::MAGNETOMETER_UNCALIBRATED, DOC(dai, IMUSensor, MAGNETOMETER_UNCALIBRATED))
        .value("ROTATION_VECTOR", IMUSensor::ROTATION_VECTOR, DOC(dai, IMUSensor, ROTATION_VECTOR))
        .value("GAME_ROTATION_VECTOR", IMUSensor::GAME_ROTATION_VECTOR, DOC(dai, IMUSensor, GAME_ROTATION_VECTOR))
        .value("GEOMAGNETIC_ROTATION_VECTOR", IMUSensor::GEOMAGNETIC_ROTATION_VECTOR, DOC(dai, IMUSensor, GEOMAGNETIC_ROTATION_VECTOR))
        .value("ARVR_STABILIZED_ROTATION_VECTOR", IMUSensor::ARVR_STABILIZED_ROTATION_VECTOR, DOC(dai, IMUSensor, ARVR_STABILIZED_ROTATION_VECTOR))
        .value("ARVR_STABILIZED_GAME_ROTATION_VECTOR", IMUSensor::ARVR_STABILIZED_GAME_ROTATION_VECTOR, DOC(dai, IMUSensor, ARVR_STABILIZED_GAME_ROTATION_VECTOR))
        // .value("GYRO_INTEGRATED_ROTATION_VECTOR", IMUSensor::GYRO_INTEGRATED_ROTATION_VECTOR)
    ;

    py::class_<IMUSensorConfig, std::shared_ptr<IMUSensorConfig>>(m, "IMUSensorConfig", DOC(dai, IMUSensorConfig))
        .def(py::init<>())
        .def_readwrite("sensitivityEnabled", &IMUSensorConfig::sensitivityEnabled)
        .def_readwrite("sensitivityRelative", &IMUSensorConfig::sensitivityRelative)
        .def_readwrite("changeSensitivity", &IMUSensorConfig::changeSensitivity)
        .def_readwrite("reportRate", &IMUSensorConfig::reportRate)
        .def_readwrite("sensorId", &IMUSensorConfig::sensorId)
        ;

    py::class_<IMUProperties> imuProperties(m, "IMUProperties", DOC(dai, IMUProperties));
    imuProperties
        .def_readwrite("imuSensors", &IMUProperties::imuSensors, DOC(dai, IMUProperties, imuSensors))
        .def_readwrite("batchReportThreshold", &IMUProperties::batchReportThreshold, DOC(dai, IMUProperties, batchReportThreshold))
        .def_readwrite("maxBatchReports", &IMUProperties::maxBatchReports, DOC(dai, IMUProperties, maxBatchReports))
    ;
    m.attr("IMU").attr("Properties") = imuProperties;


    py::class_<EdgeDetectorProperties> edgeDetectorProperties(m, "EdgeDetectorProperties", DOC(dai, EdgeDetectorProperties));
    edgeDetectorProperties
        .def_readwrite("initialConfig", &EdgeDetectorProperties::initialConfig, DOC(dai, EdgeDetectorProperties, initialConfig))
        .def_readwrite("inputConfigSync", &EdgeDetectorProperties::inputConfigSync, DOC(dai, EdgeDetectorProperties, inputConfigSync))
        .def_readwrite("outputFrameSize", &EdgeDetectorProperties::outputFrameSize, DOC(dai, EdgeDetectorProperties, outputFrameSize))
        .def_readwrite("numFramesPool", &EdgeDetectorProperties::numFramesPool, DOC(dai, EdgeDetectorProperties, numFramesPool))

    ;
    m.attr("EdgeDetector").attr("Properties") = edgeDetectorProperties;



}
