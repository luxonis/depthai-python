#pragma once

// This must be kept first for MSVC 2015.
// Do not remove the empty line between the #includes.
#include <pybind11/pybind11.h>

#include <pybind11/eval.h>

#if defined(_MSC_VER) && _MSC_VER < 1910
// We get some really long type names here which causes MSVC 2015 to emit warnings
#    pragma warning(                                                                              \
        disable : 4503) // warning C4503: decorated name length exceeded, name was truncated
#endif

namespace py = pybind11;
using namespace pybind11::literals;

class test_initializer {
    using Initializer = void (*)(py::module_ &);

public:
    test_initializer(Initializer init);
    test_initializer(const char *submodule_name, Initializer init);
};

#define TEST_SUBMODULE(name, variable)                                                            \
    void test_submodule_##name(py::module_ &);                                                    \
    test_initializer name(#name, test_submodule_##name);                                          \
    void test_submodule_##name(py::module_ &(variable))
