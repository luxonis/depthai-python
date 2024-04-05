#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/VideoEncoder.hpp"

void bind_videoencoder(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<VideoEncoderProperties> videoEncoderProperties(m, "VideoEncoderProperties", DOC(dai, VideoEncoderProperties));
    py::enum_<VideoEncoderProperties::Profile> videoEncoderPropertiesProfile(videoEncoderProperties, "Profile", DOC(dai, VideoEncoderProperties, Profile));
    py::enum_<VideoEncoderProperties::RateControlMode> videoEncoderPropertiesProfileRateControlMode(videoEncoderProperties, "RateControlMode", DOC(dai, VideoEncoderProperties, RateControlMode));
    auto videoEncoder = ADD_NODE(VideoEncoder);

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
    videoEncoderPropertiesProfile
        .value("H264_BASELINE", VideoEncoderProperties::Profile::H264_BASELINE)
        .value("H264_HIGH", VideoEncoderProperties::Profile::H264_HIGH)
        .value("H264_MAIN", VideoEncoderProperties::Profile::H264_MAIN)
        .value("H265_MAIN", VideoEncoderProperties::Profile::H265_MAIN)
        .value("MJPEG", VideoEncoderProperties::Profile::MJPEG)
        ;

    videoEncoderPropertiesProfileRateControlMode
        .value("CBR", VideoEncoderProperties::RateControlMode::CBR)
        .value("VBR", VideoEncoderProperties::RateControlMode::VBR)
        ;

    videoEncoderProperties
        .def_readwrite("bitrate", &VideoEncoderProperties::bitrate)
        .def_readwrite("keyframeFrequency", &VideoEncoderProperties::keyframeFrequency)
        .def_readwrite("maxBitrate", &VideoEncoderProperties::maxBitrate)
        .def_readwrite("numBFrames", &VideoEncoderProperties::numBFrames)
        .def_readwrite("numFramesPool", &VideoEncoderProperties::numFramesPool)
        .def_readwrite("profile", &VideoEncoderProperties::profile)
        .def_readwrite("quality", &VideoEncoderProperties::quality)
        .def_readwrite("rateCtrlMode", &VideoEncoderProperties::rateCtrlMode)
        .def_readwrite("outputFrameSize", &VideoEncoderProperties::outputFrameSize)
        .def_readwrite("miscProps", &VideoEncoderProperties::miscProps)
        ;

    // Node
    videoEncoder
        .def_readonly("input", &VideoEncoder::input, DOC(dai, node, VideoEncoder, input), DOC(dai, node, VideoEncoder, input))
        .def_readonly("bitstream", &VideoEncoder::bitstream, DOC(dai, node, VideoEncoder, bitstream), DOC(dai, node, VideoEncoder, bitstream))
        .def("setDefaultProfilePreset", static_cast<void(VideoEncoder::*)(float, VideoEncoderProperties::Profile)>(&VideoEncoder::setDefaultProfilePreset), py::arg("fps"), py::arg("profile"), DOC(dai, node, VideoEncoder, setDefaultProfilePreset))
        .def("setDefaultProfilePreset", [](VideoEncoder& v, int width, int height, float fps, VideoEncoderProperties::Profile profile){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input width/height no longer needed, automatically determined from first frame", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            v.setDefaultProfilePreset(width, height, fps, profile);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, setDefaultProfilePreset, 2))
        .def("setDefaultProfilePreset", [](VideoEncoder& v, std::tuple<int,int> size, float fps, VideoEncoderProperties::Profile profile){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input size no longer needed, automatically determined from first frame", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            v.setDefaultProfilePreset(size, fps, profile);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, setDefaultProfilePreset, 3))
        .def("setNumFramesPool", &VideoEncoder::setNumFramesPool, py::arg("frames"), DOC(dai, node, VideoEncoder, setNumFramesPool))
        .def("getNumFramesPool", &VideoEncoder::getNumFramesPool, DOC(dai, node, VideoEncoder, getNumFramesPool))
        .def("setRateControlMode", &VideoEncoder::setRateControlMode, py::arg("mode"), DOC(dai, node, VideoEncoder, setRateControlMode))
        .def("setProfile", static_cast<void(VideoEncoder::*)(VideoEncoder::Properties::Profile)>(&VideoEncoder::setProfile), py::arg("profile"), DOC(dai, node, VideoEncoder, setProfile))
        .def("setProfile", [](VideoEncoder& v, std::tuple<int,int> size, VideoEncoderProperties::Profile profile){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input width/height no longer needed, automatically determined from first frame", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            v.setProfile(size, profile);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, setProfile, 2))
        .def("setProfile", [](VideoEncoder& v, int width, int height, VideoEncoderProperties::Profile profile){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input width/height no longer needed, automatically determined from first frame", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            v.setProfile(width, height, profile);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, setProfile, 3))
        .def("setBitrate", &VideoEncoder::setBitrate, py::arg("bitrate"), DOC(dai, node, VideoEncoder, setBitrate))
        .def("setBitrateKbps", &VideoEncoder::setBitrateKbps, py::arg("bitrateKbps"), DOC(dai, node, VideoEncoder, setBitrateKbps))
        .def("setKeyframeFrequency", &VideoEncoder::setKeyframeFrequency, py::arg("freq"), DOC(dai, node, VideoEncoder, setKeyframeFrequency))
        .def("setMaxOutputFrameSize", &VideoEncoder::setMaxOutputFrameSize, py::arg("maxFrameSize"), DOC(dai, node, VideoEncoder, setMaxOutputFrameSize))
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
        .def("getWidth", [](VideoEncoder& v){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input size no longer available, it's determined when first frame arrives", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return v.getWidth();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, getWidth))
        .def("getHeight", [](VideoEncoder& v){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input size no longer available, it's determined when first frame arrives", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return v.getHeight();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, getHeight))
        .def("getSize", [](VideoEncoder& v){
            PyErr_WarnEx(PyExc_DeprecationWarning, "Input size no longer available, it's determined when first frame arrives", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return v.getSize();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, VideoEncoder, getSize))
        .def("getFrameRate", &VideoEncoder::getFrameRate, DOC(dai, node, VideoEncoder, getFrameRate))
        .def("getLossless", &VideoEncoder::getLossless, DOC(dai, node, VideoEncoder, getLossless))
        .def("getMaxOutputFrameSize", &VideoEncoder::getMaxOutputFrameSize, DOC(dai, node, VideoEncoder, getMaxOutputFrameSize))
        .def("setMisc", py::overload_cast<std::string, std::string>(&VideoEncoder::setMisc), py::arg("property"), py::arg("value"), DOC(dai, VideoEncoder, setMisc))
        .def("setMisc", py::overload_cast<std::string, int>(&VideoEncoder::setMisc), py::arg("property"), py::arg("value"), DOC(dai, VideoEncoder, setMisc, 2))
        .def("setMisc", py::overload_cast<std::string, float>(&VideoEncoder::setMisc), py::arg("property"), py::arg("value"), DOC(dai, VideoEncoder, setMisc, 3))
        .def("clearMiscProperties", &VideoEncoder::clearMiscProperties, DOC(dai, VideoEncoder, clearMiscProperties))
        .def("getMiscProperties", &VideoEncoder::getMiscProperties, DOC(dai, VideoEncoder, getMiscProperties))
    ;
    // ALIAS
    daiNodeModule.attr("VideoEncoder").attr("Properties") = videoEncoderProperties;

}
