#pragma once

// pybind
#include "pybind11_common.hpp"

struct DatatypeBindings {
    static void bind(pybind11::module& m, void* pCallstack);
};
