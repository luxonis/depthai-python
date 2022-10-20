#include "VersionBindings.hpp"

// depthai
#include "depthai/device/Version.hpp"

void VersionBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Type definitions
    py::class_<Version> version(m, "Version", DOC(dai, Version));

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Bind DeviceBootloader
    version
        .def(py::init<const std::string&>(), py::arg("v"), DOC(dai, Version, Version))
        .def(py::init<unsigned, unsigned, unsigned>(), py::arg("major"), py::arg("minor"), py::arg("patch"), DOC(dai, Version, Version, 2))
        .def("__str__", &Version::toString, DOC(dai, Version, toString))
        .def("__eq__", &Version::operator==)
        .def("__lt__", &Version::operator<)
        .def("__gt__", &Version::operator>)
        .def("toStringSemver", &Version::toStringSemver, DOC(dai, Version, toStringSemver))
        .def("getBuildInfo", &Version::getBuildInfo, DOC(dai, Version, getBuildInfo))
        .def("getSemver", &Version::getSemver, DOC(dai, Version, getSemver))
        ;

}
