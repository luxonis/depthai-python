#pragma once

//pybind11
#include "pybind11_common.hpp"

// depthai-api
#include "depthai/device.hpp"

void init_binding_device(pybind11::module& m);