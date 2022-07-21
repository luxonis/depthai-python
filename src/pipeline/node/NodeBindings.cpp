#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"

// Libraries
#include "hedley/hedley.h"

// pybind11
#include "pybind11/stl_bind.h"

// Map of python node classes and call to pipeline to create it
std::vector<std::pair<py::handle, std::function<std::shared_ptr<dai::Node>(dai::Pipeline&, py::object class_)>>> pyNodeCreateMap;
py::handle daiNodeModule;

std::vector<std::pair<py::handle, std::function<std::shared_ptr<dai::Node>(dai::Pipeline&, py::object class_)>>> NodeBindings::getNodeCreateMap(){
    return pyNodeCreateMap;
}

// Bind map - without init method
template <typename Map, typename holder_type = std::unique_ptr<Map>, typename... Args>
py::class_<Map, holder_type> bindNodeMap(py::handle scope, const std::string &name, Args&&... args) {
    using namespace py;
    using KeyType = typename Map::key_type;
    using MappedType = typename Map::mapped_type;
    using Class_ = class_<Map, holder_type>;

    // If either type is a non-module-local bound type then make the map binding non-local as well;
    // otherwise (e.g. both types are either module-local or converting) the map will be
    // module-local.
    auto tinfo = py::detail::get_type_info(typeid(MappedType));
    bool local = !tinfo || tinfo->module_local;
    if (local) {
        tinfo = py::detail::get_type_info(typeid(KeyType));
        local = !tinfo || tinfo->module_local;
    }

    Class_ cl(scope, name.c_str(), pybind11::module_local(local), std::forward<Args>(args)...);

    // Register stream insertion operator (if possible)
    detail::map_if_insertion_operator<Map, Class_>(cl, name);

    cl.def("__bool__",
        [](const Map &m) -> bool { return !m.empty(); },
        "Check whether the map is nonempty"
    );

    cl.def("__iter__",
           [](Map &m) { return make_key_iterator(m.begin(), m.end()); },
           keep_alive<0, 1>() /* Essential: keep list alive while iterator exists */
    );

    cl.def("items",
           [](Map &m) { return make_iterator(m.begin(), m.end()); },
           keep_alive<0, 1>() /* Essential: keep list alive while iterator exists */
    );

    // Modified __getitem__. Uses operator[] underneath
    cl.def("__getitem__",
        [](Map &m, const KeyType &k) -> MappedType & {
            return m[k];
        },
        return_value_policy::reference_internal // ref + keepalive
    );

    cl.def("__contains__",
        [](Map &m, const KeyType &k) -> bool {
            auto it = m.find(k);
            if (it == m.end())
              return false;
           return true;
        }
    );

    // Assignment provided only if the type is copyable
    detail::map_assignment<Map, Class_>(cl);

    cl.def("__delitem__",
           [](Map &m, const KeyType &k) {
               auto it = m.find(k);
               if (it == m.end())
                   throw key_error();
               m.erase(it);
           }
    );

    cl.def("__len__", &Map::size);

    return cl;
}


void bind_xlinkin(pybind11::module& m, void* pCallstack);
void bind_xlinkout(pybind11::module& m, void* pCallstack);
void bind_colorcamera(pybind11::module& m, void* pCallstack);
void bind_monocamera(pybind11::module& m, void* pCallstack);
void bind_stereodepth(pybind11::module& m, void* pCallstack);
void bind_neuralnetwork(pybind11::module& m, void* pCallstack);
void bind_videoencoder(pybind11::module& m, void* pCallstack);
void bind_imagemanip(pybind11::module& m, void* pCallstack);
void bind_spiout(pybind11::module& m, void* pCallstack);
void bind_spiin(pybind11::module& m, void* pCallstack);
void bind_detectionnetwork(pybind11::module& m, void* pCallstack);
void bind_systemlogger(pybind11::module& m, void* pCallstack);
void bind_script(pybind11::module& m, void* pCallstack);
void bind_spatiallocationcalculator(pybind11::module& m, void* pCallstack);
void bind_spatialdetectionnetwork(pybind11::module& m, void* pCallstack);
void bind_objecttracker(pybind11::module& m, void* pCallstack);
void bind_imu(pybind11::module& m, void* pCallstack);
void bind_edgedetector(pybind11::module& m, void* pCallstack);
void bind_featuretracker(pybind11::module& m, void* pCallstack);
void bind_apriltag(pybind11::module& m, void* pCallstack);
void bind_detectionparser(pybind11::module& m, void* pCallstack);

void NodeBindings::addToCallstack(std::deque<StackFunction>& callstack) {
    // Bind Node et al
    callstack.push_front(NodeBindings::bind);

    // Bind all other nodes
    callstack.push_front(bind_xlinkin);
    callstack.push_front(bind_xlinkout);
    callstack.push_front(bind_colorcamera);
    callstack.push_front(bind_monocamera);
    callstack.push_front(bind_stereodepth);
    callstack.push_front(bind_neuralnetwork);
    callstack.push_front(bind_videoencoder);
    callstack.push_front(bind_imagemanip);
    callstack.push_front(bind_spiout);
    callstack.push_front(bind_spiin);
    callstack.push_front(bind_detectionnetwork);
    callstack.push_front(bind_systemlogger);
    callstack.push_front(bind_script);
    callstack.push_front(bind_spatiallocationcalculator);
    callstack.push_front(bind_spatialdetectionnetwork);
    callstack.push_front(bind_objecttracker);
    callstack.push_front(bind_imu);
    callstack.push_front(bind_edgedetector);
    callstack.push_front(bind_featuretracker);
    callstack.push_front(bind_apriltag);
    callstack.push_front(bind_detectionparser);
}

void NodeBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;
    //// Bindings for actual nodes
    // Move properties into nodes and nodes under 'node' submodule
    daiNodeModule = m.def_submodule("node");

    // Properties
    py::class_<Node, std::shared_ptr<Node>> pyNode(m, "Node", DOC(dai, Node));
    py::class_<Node::Input> pyInput(pyNode, "Input", DOC(dai, Node, Input));
    py::enum_<Node::Input::Type> nodeInputType(pyInput, "Type");
    py::class_<Node::Output> pyOutput(pyNode, "Output", DOC(dai, Node, Output));
    py::enum_<Node::Output::Type> nodeOutputType(pyOutput, "Type");
    py::class_<Properties, std::shared_ptr<Properties>> pyProperties(m, "Properties", DOC(dai, Properties));


    // Node::Id bindings
    py::class_<Node::Id>(pyNode, "Id", "Node identificator. Unique for every node on a single Pipeline");
    // Node::Connection bindings
    py::class_<Node::Connection> nodeConnection(pyNode, "Connection", DOC(dai, Node, Connection));
    // Node::InputMap bindings
    bindNodeMap<Node::InputMap>(pyNode, "InputMap");
    // Node::OutputMap bindings
    bindNodeMap<Node::OutputMap>(pyNode, "OutputMap");


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


    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    // Node Bindings after properties, so types are resolved
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Base 'Node' class binding

    // Node::Input bindings
    nodeInputType
        .value("SReceiver", Node::Input::Type::SReceiver)
        .value("MReceiver", Node::Input::Type::MReceiver)
    ;
    pyInput
        .def_property("group", [](Node::Input& obj) { return obj.group; }, [](Node::Input& obj, std::string group) { obj.group = group; })
        .def_property("name", [](Node::Input& obj) { return obj.name; }, [](Node::Input& obj, std::string name) { obj.name = name; })
        .def_property("type", [](Node::Input& obj) { return obj.type; }, [](Node::Input& obj, Node::Input::Type type) { obj.type = type; })
        .def("setBlocking", &Node::Input::setBlocking, py::arg("blocking"), DOC(dai, Node, Input, setBlocking))
        .def("getBlocking", &Node::Input::getBlocking, DOC(dai, Node, Input, getBlocking))
        .def("setQueueSize", &Node::Input::setQueueSize, py::arg("size"), DOC(dai, Node, Input, setQueueSize))
        .def("getQueueSize", &Node::Input::getQueueSize, DOC(dai, Node, Input, getQueueSize))
        .def_property("waitForMessage", [](Node::Input& obj) { return obj.type; }, [](Node::Input& obj, bool waitForMessage) { obj.waitForMessage = waitForMessage; })
        .def("setWaitForMessage", &Node::Input::setWaitForMessage, py::arg("waitForMessage"), DOC(dai, Node, Input, setWaitForMessage))
        .def("getWaitForMessage", &Node::Input::getWaitForMessage, DOC(dai, Node, Input, getWaitForMessage))
        .def("setReusePreviousMessage", &Node::Input::setReusePreviousMessage, py::arg("reusePreviousMessage"), DOC(dai, Node, Input, setReusePreviousMessage))
        .def("getReusePreviousMessage", &Node::Input::getReusePreviousMessage, DOC(dai, Node, Input, getReusePreviousMessage))
    ;

    // Node::Output bindings
    nodeOutputType
        .value("MSender", Node::Output::Type::MSender)
        .value("SSender", Node::Output::Type::SSender)
    ;
    pyOutput
        .def_property("group", [](Node::Output& obj) -> std::string& { return obj.group; }, [](Node::Output& obj, std::string group) { obj.group = group; })
        .def_property("name", [](Node::Output& obj) -> std::string& { return obj.name; }, [](Node::Output& obj, std::string name) { obj.name = name; })
        .def_property("type", [](Node::Output& obj) -> Node::Output::Type& { return obj.type; }, [](Node::Output& obj, Node::Output::Type type) { obj.type = type; })
        .def("canConnect", &Node::Output::canConnect, py::arg("input"), DOC(dai, Node, Output, canConnect))
        .def("link", &Node::Output::link, py::arg("input"), DOC(dai, Node, Output, link))
        .def("unlink", &Node::Output::unlink, py::arg("input"), DOC(dai, Node, Output, unlink))
        .def("getConnections", &Node::Output::getConnections, DOC(dai, Node, Output, getConnections))
    ;


    nodeConnection
        .def_property("outputId", [](Node::Connection& conn) -> Node::Id& { return conn.outputId; }, [](Node::Connection& conn, Node::Id id) {conn.outputId = id; }, DOC(dai, Node, Connection, outputId))
        .def_property("outputName", [](Node::Connection& conn) -> std::string& { return conn.outputName; }, [](Node::Connection& conn, std::string name) {conn.outputName = name; }, DOC(dai, Node, Connection, outputName))
        .def_property("outputGroup", [](Node::Connection& conn) -> std::string& { return conn.outputGroup; }, [](Node::Connection& conn, std::string group) {conn.outputGroup = group; }, DOC(dai, Node, Connection, outputGroup))
        .def_property("inputId", [](Node::Connection& conn) -> Node::Id& { return conn.inputId; }, [](Node::Connection& conn, Node::Id id) {conn.inputId = id; }, DOC(dai, Node, Connection, inputId))
        .def_property("inputName", [](Node::Connection& conn) -> std::string& { return conn.inputName; }, [](Node::Connection& conn, std::string name) {conn.inputName = name; }, DOC(dai, Node, Connection, inputName))
        .def_property("inputGroup", [](Node::Connection& conn) -> std::string& { return conn.inputGroup; }, [](Node::Connection& conn, std::string group) {conn.inputGroup = group; }, DOC(dai, Node, Connection, inputGroup))
    ;

    pyNode
        .def_readonly("id", &Node::id, DOC(dai, Node, id))
        .def("getName", &Node::getName, DOC(dai, Node, getName))
        .def("getOutputs", &Node::getOutputs, DOC(dai, Node, getOutputs))
        .def("getInputs", &Node::getInputs, DOC(dai, Node, getInputs))
        .def("getOutputRefs", static_cast<std::vector< Node::Output*> (Node::*)()>(&Node::getOutputRefs), DOC(dai, Node, getOutputRefs), py::return_value_policy::reference_internal)
        .def("getInputRefs", static_cast<std::vector<Node::Input*> (Node::*)()>(&Node::getInputRefs), DOC(dai, Node, getInputRefs), py::return_value_policy::reference_internal)
        .def("getOutputRefs", static_cast<std::vector<const Node::Output*> (Node::*)() const>(&Node::getOutputRefs), DOC(dai, Node, getOutputRefs), py::return_value_policy::reference_internal)
        .def("getInputRefs", static_cast<std::vector<const Node::Input*> (Node::*)() const>(&Node::getInputRefs), DOC(dai, Node, getInputRefs), py::return_value_policy::reference_internal)
        .def("getParentPipeline", py::overload_cast<>(&Node::getParentPipeline), DOC(dai, Node, getParentPipeline))
        .def("getParentPipeline", py::overload_cast<>(&Node::getParentPipeline, py::const_), DOC(dai, Node, getParentPipeline))
        .def("getAssetManager", static_cast<const AssetManager& (Node::*)() const>(&Node::getAssetManager), py::return_value_policy::reference_internal, DOC(dai, Node, getAssetManager))
        .def("getAssetManager", static_cast<AssetManager& (Node::*)()>(&Node::getAssetManager), py::return_value_policy::reference_internal, DOC(dai, Node, getAssetManager))
        .def_property("properties", [](Node& n) -> const Properties& { return n.properties; }, [](Node& n, const Properties& p) { n.properties = p; }, DOC(dai, Node, properties), py::return_value_policy::reference_internal)
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

}

