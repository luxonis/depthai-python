#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/AudioProc.hpp"

void bind_audioproc(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<AudioProcProperties> audioProcProperties(m, "AudioProcProperties", DOC(dai, AudioProcProperties));
    auto audioProc = ADD_NODE(AudioProc);

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
    //audioProcProperties
    //    .def_readwrite("streamName", &AudioProcProperties::streamName)
    //;

    // Node
    audioProc
        .def_readonly("input", &AudioProc::input, DOC(dai, node, AudioProc, input))
        .def_readonly("reference", &AudioProc::reference, DOC(dai, node, AudioProc, reference))
        .def_readonly("out", &AudioProc::out, DOC(dai, node, AudioProc, out))
        .def("setNumFramesPool", &AudioProc::setNumFramesPool, py::arg("frames"), DOC(dai, node, AudioProc, setNumFramesPool))
        .def("getNumFramesPool", &AudioProc::getNumFramesPool, DOC(dai, node, AudioProc, getNumFramesPool))
        ;
    // ALIAS
    daiNodeModule.attr("AudioProc").attr("Properties") = audioProcProperties;


}
