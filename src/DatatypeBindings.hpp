#pragma once

// pybind
#include "pybind11_common.hpp"

struct DatatypeBindings {
    static void addToCallstack(std::deque<StackFunction>& callstack);
};
