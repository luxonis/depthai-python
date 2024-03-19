#pragma once

// pybind
#include "pybind11_common.hpp"

// Libraries
#include "hedley/hedley.h"

// pybind11
#include "pybind11/stl_bind.h"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/DeviceNode.hpp"

// Map of python node classes and call to pipeline to create it
extern std::vector<std::pair<py::handle, std::function<std::shared_ptr<dai::Node>(dai::Pipeline&, py::object class_)>>> pyNodeCreateMap;
extern py::handle daiNodeModule;

template<typename T, typename DERIVED = dai::DeviceNode>
py::class_<T> addNode(const char* name, const char* docstring = nullptr){
    auto node = py::class_<T, DERIVED, std::shared_ptr<T>>(daiNodeModule, name, docstring);
    pyNodeCreateMap.push_back(std::make_pair(node, [](dai::Pipeline& p, py::object class_){
        return p.create<T>();
    }));
    return node;
}

template<typename T, typename DERIVED = dai::DeviceNode>
py::class_<T> addNodeAbstract(const char* name, const char* docstring = nullptr){
    auto node = py::class_<T, DERIVED, std::shared_ptr<T>>(daiNodeModule, name, docstring);
    pyNodeCreateMap.push_back(std::make_pair(node, [](dai::Pipeline& p, py::object class_) -> std::shared_ptr<dai::Node> {
        throw std::invalid_argument(std::string(py::str(class_)) + " is an abstract node. Choose an appropriate derived node instead");
        return nullptr;
    }));
    return node;
}

// Macro helpers
#define ADD_NODE(NodeName) addNode<NodeName>(#NodeName, DOC(dai, node, NodeName))
#define ADD_NODE_DERIVED(NodeName, Derived) addNode<NodeName, Derived>(#NodeName, DOC(dai, node, NodeName))
#define ADD_NODE_ABSTRACT(NodeName) addNodeAbstract<NodeName>(#NodeName, DOC(dai, node, NodeName))
#define ADD_NODE_DERIVED_ABSTRACT(NodeName, Derived) addNodeAbstract<NodeName, Derived>(#NodeName, DOC(dai, node, NodeName))
#define ADD_NODE_DOC(NodeName, docstring) addNode<NodeName>(#NodeName, docstring)
#define ADD_NODE_DERIVED_DOC(NodeName, Derived, docstring) addNode<NodeName, Derived>(#NodeName, docstring)
