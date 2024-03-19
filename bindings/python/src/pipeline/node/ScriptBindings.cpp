#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Script.hpp"

void bind_script(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<ScriptProperties> scriptProperties(m, "ScriptProperties", DOC(dai, ScriptProperties));
    auto script = ADD_NODE(Script);

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
    scriptProperties
        .def_readwrite("scriptUri", &ScriptProperties::scriptUri, DOC(dai, ScriptProperties, scriptUri))
        .def_readwrite("scriptName", &ScriptProperties::scriptName, DOC(dai, ScriptProperties, scriptName))
        .def_readwrite("processor", &ScriptProperties::processor, DOC(dai, ScriptProperties, processor))
    ;

    // Node
    script
        .def_readonly("inputs", &Script::inputs)
        .def_readonly("outputs", &Script::outputs)
        .def("setScriptPath", &Script::setScriptPath, DOC(dai, node, Script, setScriptPath))
        .def("setScript", py::overload_cast<const std::string&, const std::string&>(&Script::setScript), py::arg("script"), py::arg("name") = "", DOC(dai, node, Script, setScript))
        .def("setScript", py::overload_cast<const std::vector<std::uint8_t>&, const std::string&>(&Script::setScript), py::arg("data"), py::arg("name") = "", DOC(dai, node, Script, setScript, 2))
        .def("setScriptPath", &Script::setScriptPath, py::arg("path"), py::arg("name") = "", DOC(dai, node, Script, setScriptPath))
        .def("getScriptName", &Script::getScriptName, DOC(dai, node, Script, getScriptName))
        .def("setProcessor", &Script::setProcessor, DOC(dai, node, Script, setProcessor))
        .def("getProcessor", &Script::getProcessor, DOC(dai, node, Script, getProcessor))
        ;
    daiNodeModule.attr("Script").attr("Properties") = scriptProperties;

}
