#include "NodeBindings.hpp"

#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/XLinkIn.hpp"
#include "depthai/pipeline/node/XLinkOut.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"
#include "depthai/pipeline/node/NeuralNetwork.hpp"
#include "depthai/pipeline/node/VideoEncoder.hpp"
#include "depthai/pipeline/node/ImageManip.hpp"


void NodeBindings::bind(pybind11::module& m){

    using namespace dai;
    using namespace dai::node;


    // Base 'Node' class binding
    py::class_<Node, std::shared_ptr<Node>> pyNode(m, "Node");
    pyNode 
        .def("getName", &Node::getName)
        .def("getOutputs", &Node::getOutputs)
        .def("getInputs", &Node::getInputs)
        .def("getAssets", &Node::getAssets)
    ;

    // Input and Output bindings (Node::Input && Node::Output)
    py::class_<Node::Input>(pyNode, "Input")
        .def("setBlocking", &Node::Input::setBlocking)
        .def("getBlocking", &Node::Input::getBlocking)
    ;
    
    py::class_<Node::Output>(pyNode, "Output")
        .def("canConnect", &Node::Output::canConnect)
        .def("link", &Node::Output::link)
        .def("unlink", &Node::Output::unlink)
        .def("getConnections", &Node::Output::getConnections)
    ;


    // XLinkIn node
    py::class_<XLinkIn, Node, std::shared_ptr<XLinkIn>>(m, "XLinkIn")
        .def_readonly("out", &XLinkIn::out)
        .def("setStreamName", &XLinkIn::setStreamName)
        .def("setMaxDataSize", &XLinkIn::setMaxDataSize)
        .def("setNumFrames",  &XLinkIn::setNumFrames)   
        ;

    // XLinkOut node
    py::class_<XLinkOut, Node, std::shared_ptr<XLinkOut>>(m, "XLinkOut")
        .def_readonly("input", &XLinkOut::input)
        .def("setStreamName", &XLinkOut::setStreamName)
        .def("setFpsLimit", &XLinkOut::setFpsLimit)
        ;

    // ColorCamera node
    py::class_<ColorCamera, Node, std::shared_ptr<ColorCamera>>(m, "ColorCamera")
        .def_readonly("video", &ColorCamera::video)
        .def_readonly("preview", &ColorCamera::preview)
        .def_readonly("still", &ColorCamera::still)
        .def("setCamId", &ColorCamera::setCamId)
        .def("getCamId", &ColorCamera::getCamId)
        .def("setColorOrder", &ColorCamera::setColorOrder)
        .def("setInterleaved", &ColorCamera::setInterleaved)
        .def("setPreviewSize", &ColorCamera::setPreviewSize)
        .def("setResolution", &ColorCamera::setResolution)
        .def("setFps", &ColorCamera::setFps)
        ;

    // NeuralNetwork node
    py::class_<NeuralNetwork, Node, std::shared_ptr<NeuralNetwork>>(m, "NeuralNetwork")
        .def_readonly("input", &NeuralNetwork::input)
        .def_readonly("out", &NeuralNetwork::out)
        .def("setBlobPath", &NeuralNetwork::setBlobPath)
        .def("setNumPoolFrames", &NeuralNetwork::setNumPoolFrames)
        ;

    // ImageManip node
    py::class_<ImageManip, Node, std::shared_ptr<ImageManip>>(m, "ImageManip")
        .def_readonly("inputConfig", &ImageManip::inputConfig)
        .def_readonly("inputImage", &ImageManip::inputImage)
        .def_readonly("out", &ImageManip::out)
        // setters
        .def("setCropRect", &ImageManip::setCropRect)
        .def("setCenterCrop", &ImageManip::setCenterCrop)
        .def("setResize", &ImageManip::setResize)
        .def("setResizeThumbnail", &ImageManip::setResizeThumbnail)
        .def("setFrameType", &ImageManip::setFrameType)
        .def("setHorizontalFlip", &ImageManip::setHorizontalFlip)
        .def("setWaitForConfigInput", &ImageManip::setWaitForConfigInput)
        .def("setNumFramesPool", &ImageManip::setNumFramesPool)
        .def("setMaxOutputFrameSize", &ImageManip::setMaxOutputFrameSize)
        ;

     // MonoCamera node
    py::class_<MonoCamera, Node, std::shared_ptr<MonoCamera>>(m, "MonoCamera")
        .def_readonly("out",  &MonoCamera::out)
        .def("setCamId",      &MonoCamera::setCamId)
        .def("getCamId",      &MonoCamera::getCamId)
        .def("setResolution", &MonoCamera::setResolution)
        .def("setFps",        &MonoCamera::setFps)
        ;

    // StereoDepth node
    py::class_<StereoDepth, Node, std::shared_ptr<StereoDepth>>(m, "StereoDepth")
        .def_readonly("left",           &StereoDepth::left)
        .def_readonly("right",          &StereoDepth::right)
        .def_readonly("depth",          &StereoDepth::depth)
        .def_readonly("disparity",      &StereoDepth::disparity)
        .def_readonly("syncedLeft",     &StereoDepth::syncedLeft)
        .def_readonly("syncedRight",    &StereoDepth::syncedRight)
        .def_readonly("rectifiedLeft",  &StereoDepth::rectifiedLeft)
        .def_readonly("rectifiedRight", &StereoDepth::rectifiedRight)
        .def("loadCalibrationFile",     &StereoDepth::loadCalibrationFile)
        .def("loadCalibrationData",     &StereoDepth::loadCalibrationData)
        .def("setEmptyCalibration",     &StereoDepth::setEmptyCalibration)
        .def("setInputResolution",      &StereoDepth::setInputResolution)
        .def("setMedianFilter",         &StereoDepth::setMedianFilter)
        .def("setConfidenceThreshold",  &StereoDepth::setConfidenceThreshold)
        .def("setLeftRightCheck",       &StereoDepth::setLeftRightCheck)
        .def("setSubpixel",             &StereoDepth::setSubpixel)
        .def("setExtendedDisparity",    &StereoDepth::setExtendedDisparity)
        .def("setRectifyEdgeFillColor", &StereoDepth::setRectifyEdgeFillColor)
        .def("setRectifyMirrorFrame",   &StereoDepth::setRectifyMirrorFrame)
        .def("setOutputRectified",      &StereoDepth::setOutputRectified)
        .def("setOutputDepth",          &StereoDepth::setOutputDepth)
        ;

    // VideoEncoder node
    py::class_<VideoEncoder, Node, std::shared_ptr<VideoEncoder>>(m, "VideoEncoder")
        .def_readonly("input", &VideoEncoder::input)
        .def_readonly("bitstream", &VideoEncoder::bitstream)
        .def("setDefaultProfilePreset", &VideoEncoder::setDefaultProfilePreset)
        .def("setNumFramesPool", &VideoEncoder::setNumFramesPool)
        .def("getNumFramesPool", &VideoEncoder::getNumFramesPool)
        .def("setRateControlMode", &VideoEncoder::setRateControlMode)
        .def("setProfile", &VideoEncoder::setProfile)
        .def("setBitrate", &VideoEncoder::setBitrate)
        .def("setKeyframeFrequency", &VideoEncoder::setKeyframeFrequency)
        //.def("setMaxBitrate", &VideoEncoder::setMaxBitrate)
        .def("setNumBFrames", &VideoEncoder::setNumBFrames)
        .def("setQuality", &VideoEncoder::setQuality)
        .def("setWidth", &VideoEncoder::setWidth)
        .def("setHeight", &VideoEncoder::setHeight)
        .def("setFrameRate", &VideoEncoder::setFrameRate)
        .def("getRateControlMode", &VideoEncoder::getRateControlMode)
        .def("getProfile", &VideoEncoder::getProfile)
        .def("getBitrate", &VideoEncoder::getBitrate)
        .def("getKeyframeFrequency", &VideoEncoder::getKeyframeFrequency)
        //.def("getMaxBitrate", &VideoEncoder::getMaxBitrate)
        .def("getNumBFrames", &VideoEncoder::getNumBFrames)
        .def("getQuality", &VideoEncoder::getQuality)
        .def("getWidth", &VideoEncoder::getWidth)
        .def("getHeight", &VideoEncoder::getHeight)
        .def("getFrameRate", &VideoEncoder::getFrameRate)
    ;

    ////////////////////////////////////
    // Node properties bindings
    ////////////////////////////////////
    py::class_<ColorCameraProperties> colorCameraProperties(m, "ColorCameraProperties");
    colorCameraProperties
        .def_readwrite("camId", &ColorCameraProperties::camId)
        .def_readwrite("colorOrder", &ColorCameraProperties::colorOrder)
        .def_readwrite("interleaved", &ColorCameraProperties::interleaved)
        .def_readwrite("previewHeight", &ColorCameraProperties::previewHeight)
        .def_readwrite("previewWidth", &ColorCameraProperties::previewWidth)
        .def_readwrite("resolution", &ColorCameraProperties::resolution)
        .def_readwrite("fps", &ColorCameraProperties::fps)
    ;

    py::enum_<ColorCameraProperties::SensorResolution>(colorCameraProperties, "SensorResolution")
        .value("THE_1080_P", ColorCameraProperties::SensorResolution::THE_1080_P)
        .value("THE_4_K", ColorCameraProperties::SensorResolution::THE_4_K)
        .value("THE_12_MP", ColorCameraProperties::SensorResolution::THE_12_MP)
        ;

    py::enum_<ColorCameraProperties::ColorOrder>(colorCameraProperties, "ColorOrder")
        .value("BGR", ColorCameraProperties::ColorOrder::BGR)
        .value("RGB", ColorCameraProperties::ColorOrder::RGB)
        ;
        

    // MonoCamera props
    py::class_<MonoCameraProperties> monoCameraProperties(m, "MonoCameraProperties");
    monoCameraProperties
        .def_readwrite("camId",      &MonoCameraProperties::camId)
        .def_readwrite("resolution", &MonoCameraProperties::resolution)
        .def_readwrite("fps",        &MonoCameraProperties::fps)
    ;

    py::enum_<MonoCameraProperties::SensorResolution>(monoCameraProperties, "SensorResolution")
        .value("THE_720_P", MonoCameraProperties::SensorResolution::THE_720_P)
        .value("THE_800_P", MonoCameraProperties::SensorResolution::THE_800_P)
        .value("THE_400_P", MonoCameraProperties::SensorResolution::THE_400_P)
        ;

   

    // StereoDepth props
    py::class_<StereoDepthProperties> stereoDepthProperties(m, "StereoDepthProperties");
    stereoDepthProperties
        .def_readwrite("calibration",             &StereoDepthProperties::calibration)
        .def_readwrite("median",                  &StereoDepthProperties::median)
        .def_readwrite("confidenceThreshold",     &StereoDepthProperties::confidenceThreshold)
        .def_readwrite("enableLeftRightCheck",    &StereoDepthProperties::enableLeftRightCheck)
        .def_readwrite("enableSubpixel",          &StereoDepthProperties::enableSubpixel)
        .def_readwrite("enableExtendedDisparity", &StereoDepthProperties::enableExtendedDisparity)
        .def_readwrite("rectifyMirrorFrame",      &StereoDepthProperties::rectifyMirrorFrame)
        .def_readwrite("rectifyEdgeFillColor",    &StereoDepthProperties::rectifyEdgeFillColor)
        .def_readwrite("enableOutputRectified",   &StereoDepthProperties::enableOutputRectified)
        .def_readwrite("enableOutputDepth",       &StereoDepthProperties::enableOutputDepth)
        .def_readwrite("width",                   &StereoDepthProperties::width)
        .def_readwrite("height",                  &StereoDepthProperties::height)
        ;

    py::enum_<StereoDepthProperties::MedianFilter>(stereoDepthProperties, "MedianFilter")
        .value("MEDIAN_OFF", StereoDepthProperties::MedianFilter::MEDIAN_OFF)
        .value("KERNEL_3x3", StereoDepthProperties::MedianFilter::KERNEL_3x3)
        .value("KERNEL_5x5", StereoDepthProperties::MedianFilter::KERNEL_5x5)
        .value("KERNEL_7x7", StereoDepthProperties::MedianFilter::KERNEL_7x7)
        ;


    // VideoEncoder props
    py::class_<VideoEncoderProperties> videoEncoderProperties(m, "VideoEncoderProperties");
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

    py::enum_<VideoEncoderProperties::Profile>(videoEncoderProperties, "Profile")
        .value("H264_BASELINE", VideoEncoderProperties::Profile::H264_BASELINE)
        .value("H264_HIGH", VideoEncoderProperties::Profile::H264_HIGH)
        .value("H264_MAIN", VideoEncoderProperties::Profile::H264_MAIN)
        .value("H265_MAIN", VideoEncoderProperties::Profile::H265_MAIN)
        .value("MJPEG", VideoEncoderProperties::Profile::MJPEG)
        ;

    py::enum_<VideoEncoderProperties::RateControlMode>(videoEncoderProperties, "RateControlMode")
        .value("CBR", VideoEncoderProperties::RateControlMode::CBR)
        .value("VBR", VideoEncoderProperties::RateControlMode::VBR)
        ;     


}