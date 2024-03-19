/*
    tests/pybind11_tests.cpp -- pybind example plugin

    Copyright (c) 2016 Wenzel Jakob <wenzel.jakob@epfl.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#include "depthai_pybind11_tests.hpp"

#include <functional>
#include <list>

/*
For testing purposes, we define a static global variable here in a function that each individual
test .cpp calls with its initialization lambda.  It's convenient here because we can just not
compile some test files to disable/ignore some of the test code.

It is NOT recommended as a way to use pybind11 in practice, however: the initialization order will
be essentially random, which is okay for our test scripts (there are no dependencies between the
individual pybind11 test .cpp files), but most likely not what you want when using pybind11
productively.

Instead, see the "How can I reduce the build time?" question in the "Frequently asked questions"
section of the documentation for good practice on splitting binding code over multiple files.
*/
std::list<std::function<void(py::module_ &)>> &initializers() {
    static std::list<std::function<void(py::module_ &)>> inits;
    return inits;
}

test_initializer::test_initializer(Initializer init) {
    initializers().emplace_back(init);
}

test_initializer::test_initializer(const char *submodule_name, Initializer init) {
    initializers().emplace_back([=](py::module_ &parent) {
        auto m = parent.def_submodule(submodule_name);
        init(m);
    });
}

PYBIND11_MODULE(depthai_pybind11_tests, m) {
    m.doc() = "depthai pybind11 test module";

#if !defined(NDEBUG)
    m.attr("debug_enabled") = true;
#else
    m.attr("debug_enabled") = false;
#endif

    for (const auto &initializer : initializers())
        initializer(m);
}
