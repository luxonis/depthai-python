#pragma once

// pybind
#include "pybind11_common.hpp"

struct DeviceBootloaderBindings {
    static void bind(pybind11::module& m, void* pCallstack);
};
