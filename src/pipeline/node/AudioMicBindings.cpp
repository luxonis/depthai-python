#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/AudioMic.hpp"

void bind_audiomic(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<AudioMicProperties> audioMicProperties(m, "AudioMicProperties", DOC(dai, AudioMicProperties));
    auto audioMic = ADD_NODE(AudioMic);

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

    // Properties TODO
    //audioMicProperties
    //    .def_readwrite("streamName", &AudioMicProperties::streamName)
    //;

    // Node
    audioMic
        .def_readonly("out", &AudioMic::out, DOC(dai, node, AudioMic, out))
        .def_readonly("outBack", &AudioMic::outBack, DOC(dai, node, AudioMic, outBack))
        .def_readonly("inputConfig", &AudioMic::inputConfig, DOC(dai, node, AudioMic, inputConfig))
        .def_readonly("initialConfig", &AudioMic::initialConfig, DOC(dai, node, AudioMic, initialConfig))
        .def("setStreamBackMic", &AudioMic::setStreamBackMic, py::arg("enable"), DOC(dai, node, AudioMic, setStreamBackMic))
        .def("setMicAutoGain", &AudioMic::setMicAutoGain, py::arg("enable"), DOC(dai, node, AudioMic, setMicAutoGain))
        .def("setXlinkApplyMicGain", &AudioMic::setXlinkApplyMicGain, py::arg("enable"), DOC(dai, node, AudioMic, setXlinkApplyMicGain))
        .def("setXlinkSampleSizeBytes", &AudioMic::setXlinkSampleSizeBytes, py::arg("size"), DOC(dai, node, AudioMic, setXlinkSampleSizeBytes))
        ;
    // ALIAS
    daiNodeModule.attr("AudioMic").attr("Properties") = audioMicProperties;


}
