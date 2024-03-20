#pragma once

// pybind
#include "pybind11_common.hpp"

// depthai
#include "depthai/pipeline/Node.hpp"

struct NodeBindings {
    static void addToCallstack(std::deque<StackFunction>& callstack);
    static std::vector<std::pair<py::handle, std::function<std::shared_ptr<dai::Node>(dai::Pipeline&, py::object class_)>>> getNodeCreateMap();
 private:
    static void bind(pybind11::module& m, void* pCallstack);
};
