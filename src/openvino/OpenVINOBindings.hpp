#pragma once

// pybind
#include "pybind11_common.hpp"

struct OpenVINOBindings {
    static void bind(pybind11::module& m);
};