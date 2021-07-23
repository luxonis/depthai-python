#pragma once

// pybind
#include "pybind11_common.hpp"

// depthai
#include "depthai/pipeline/Node.hpp"

struct NodeBindings : public dai::Node {
    static void bind(pybind11::module& m, void* pCallstack);
    static std::vector<std::pair<py::handle, std::function<std::shared_ptr<dai::Node>(dai::Pipeline&, py::object class_)>>> getNodeCreateMap();
};
