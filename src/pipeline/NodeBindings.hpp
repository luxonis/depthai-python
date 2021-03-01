#pragma once

// pybind
#include "pybind11_common.hpp"

// depthai
#include "depthai/pipeline/Node.hpp"

struct NodeBindings : public dai::Node {
    static void bind(pybind11::module& m);
};
