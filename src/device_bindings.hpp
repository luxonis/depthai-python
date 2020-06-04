#pragma once

//pybind11
#include "pybind11_common.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

// depthai-api
#include "depthai/device.hpp"

void init_binding_device(pybind11::module& m);