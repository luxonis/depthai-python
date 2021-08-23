#include "OpenVINOBindings.hpp"

// depthai
#include "depthai/openvino/OpenVINO.hpp"

void OpenVINOBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<OpenVINO> openvino(m, "OpenVINO", DOC(dai, OpenVINO));
    py::enum_<OpenVINO::Version> openvinoVersion(openvino, "Version", DOC(dai, OpenVINO, Version));



    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////



    // Bind OpenVINO
    openvino
        .def_static("getVersions", &OpenVINO::getVersions, DOC(dai, OpenVINO, getVersions))
        .def_static("getVersionName", &OpenVINO::getVersionName, py::arg("version"), DOC(dai, OpenVINO, getVersionName))
        .def_static("parseVersionName", &OpenVINO::parseVersionName, py::arg("versionString"), DOC(dai, OpenVINO, parseVersionName))
        .def_static("getBlobSupportedVersions", &OpenVINO::getBlobSupportedVersions, py::arg("majorVersion"), py::arg("majorVersion"), DOC(dai, OpenVINO, getBlobSupportedVersions))
        .def_static("getBlobLatestSupportedVersion", &OpenVINO::getBlobLatestSupportedVersion, py::arg("majorVersion"), py::arg("majorVersion"), DOC(dai, OpenVINO, getBlobLatestSupportedVersion))
        .def_static("areVersionsBlobCompatible", &OpenVINO::areVersionsBlobCompatible, py::arg("v1"), py::arg("v2"), DOC(dai, OpenVINO, areVersionsBlobCompatible))
    ;

    // not strongly typed enum OpenVINO::Version
    // previous step defined class 'OpenVINO' (variable 'openvino')
    // which is used in defining the following enum (its scope)
    // and that the values are available directly under OpenVINO.VERSION_2021_4, ...
    // they are exported
    // By default, pybind creates strong typed enums, eg: OpenVINO::Version::VERSION_2021_4

    openvinoVersion
        .value("VERSION_2020_3", OpenVINO::Version::VERSION_2020_3)
        .value("VERSION_2020_4", OpenVINO::Version::VERSION_2020_4)
        .value("VERSION_2021_1", OpenVINO::Version::VERSION_2021_1)
        .value("VERSION_2021_2", OpenVINO::Version::VERSION_2021_2)
        .value("VERSION_2021_3", OpenVINO::Version::VERSION_2021_3)
        .value("VERSION_2021_4", OpenVINO::Version::VERSION_2021_4)
        .export_values()
    ;

}



