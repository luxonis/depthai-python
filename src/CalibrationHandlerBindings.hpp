#pragma once

// pybind
#include "pybind11_common.hpp"

struct CalibrationHandlerBindings {
    static void bind(pybind11::module& m);
};
