#include "NodeBindings.hpp"
#include "Common.hpp"

//#include "depthai/pipeline/Pipeline.hpp"
//#include "depthai/pipeline/Node.hpp"
//#include "depthai/pipeline/node/Script.hpp"
#include "depthai/pipeline/HostNode.hpp"

using namespace dai;

// TODO(Zimamazim) Should be replaced with CRTP
//class PyHostNode : public NodeCRTP<HostNode, PyHostNode> {
class PyHostNode : public HostNode {
public:
    using HostNode::HostNode;

    void run() override {
        PYBIND11_OVERRIDE_PURE(
                void,
                HostNode,
                run);
    }

    // TODO(Zimamazim) Should be replaced with CRTP
    const char* getName() const override {
	return "PLACEHOLDER";
    }

    // TODO(Zimamazim) Should be replaced with CRTP
    std::unique_ptr<Node> clone() const override {
        return std::make_unique<PyHostNode>(*this);
    }
};

class Pokus {
public:
	virtual void run() = 0;	
};

class PyPokus : public Pokus {
public:
	//using Pokus::Pokus;
	void run() override {
        PYBIND11_OVERRIDE_PURE(
			void,
			Pokus,
			run
			);}

};

void bind_hostnode(pybind11::module& m, void* pCallstack){

    //using namespace dai::node;

    // Node and Properties declare upfront
    //py::class_<ScriptProperties> scriptProperties(m, "ScriptProperties", DOC(dai, ScriptProperties));
    auto hostNode = py::class_<HostNode, PyHostNode, /*ThreadedNode,*/ std::shared_ptr<HostNode>>(m, "HostNode", DOC(dai, node, HostNode));
    auto pokusNode = py::class_<Pokus, PyPokus, std::shared_ptr<Pokus>>(m, "Pokus");

    //TODO(Zimamazim): Understand why shared_ptr<HostNode> is used
    //TODO(Zimamazim): Understand why callstack is used

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

    pokusNode
	.def(py::init<>())
	.def("run", &Pokus::run)
    ;
    //m.def("pokus", [](Pokus *pokus){ pokus->run(); });
    m.def("pokus", [](py::object o){ 
	py::cast<std::shared_ptr<Pokus>>(o())->run();
    });
    hostNode
        .def(py::init<>())
        .def("run", &HostNode::run)
    ;

    m.def("testHost", [](py::object o){

	py::cast<std::shared_ptr<HostNode>>(o())->run();
        //return py::module_::import("builtins").attr("issubclass")(o, hostNode);
        //return py::cast<std::string>(o.attr("__base__").attr("__name__")) == "HostNode";
        });
/*
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
*/
}
