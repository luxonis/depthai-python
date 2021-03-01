#pragma once

// pybind
#include "pybind11_common.hpp"

struct DeviceBindings {
    static void bind(pybind11::module& m);
};
