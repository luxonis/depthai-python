#include "OpenVINOBindings.hpp"

// depthai
#include "depthai/openvino/OpenVINO.hpp"

void OpenVINOBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<OpenVINO> openvino(m, "OpenVINO", DOC(dai, OpenVINO));
    py::enum_<OpenVINO::Version> openvinoVersion(openvino, "Version", DOC(dai, OpenVINO, Version));
    py::class_<OpenVINO::Blob> openvinoBlob(openvino, "Blob", DOC(dai, OpenVINO, Blob));


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
        .value("VERSION_2022_1", OpenVINO::Version::VERSION_2022_1)
        .value("VERSION_UNIVERSAL", OpenVINO::Version::VERSION_UNIVERSAL)
        .export_values()
    ;
    // DEFAULT_VERSION binding
    openvino.attr("DEFAULT_VERSION") = dai::OpenVINO::DEFAULT_VERSION;

    // Bind OpenVINO::Blob
    openvinoBlob
        .def(py::init<std::vector<uint8_t>>(), DOC(dai, OpenVINO, Blob, Blob))
        .def(py::init<dai::Path>(), DOC(dai, OpenVINO, Blob, Blob, 2))
        .def_readwrite("version", &OpenVINO::Blob::version, DOC(dai, OpenVINO, Blob, version))
        .def_readwrite("networkInputs", &OpenVINO::Blob::networkInputs, DOC(dai, OpenVINO, Blob, networkInputs))
        .def_readwrite("networkOutputs", &OpenVINO::Blob::networkOutputs, DOC(dai, OpenVINO, Blob, networkOutputs))
        .def_readwrite("stageCount", &OpenVINO::Blob::stageCount, DOC(dai, OpenVINO, Blob, stageCount))
        .def_readwrite("numShaves", &OpenVINO::Blob::numShaves, DOC(dai, OpenVINO, Blob, numShaves))
        .def_readwrite("numSlices", &OpenVINO::Blob::numSlices, DOC(dai, OpenVINO, Blob, numSlices))
        .def_readwrite("data", &OpenVINO::Blob::data, DOC(dai, OpenVINO, Blob, data))
    ;

}



