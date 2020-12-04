#include "OpenVINOBindings.hpp"

// depthai
#include "depthai/openvino/OpenVINO.hpp"

void OpenVINOBindings::bind(pybind11::module& m){

    using namespace dai;

    // Bind OpenVINO
    py::class_<OpenVINO> openvino(m, "OpenVINO");
    openvino
        .def_static("getVersions", &OpenVINO::getVersions)
        .def_static("getVersionName", &OpenVINO::getVersionName)
        .def_static("parseVersionName", &OpenVINO::parseVersionName)
        .def_static("getBlobSupportedVersions", &OpenVINO::getBlobSupportedVersions)
        .def_static("getBlobLatestSupportedVersion", &OpenVINO::getBlobLatestSupportedVersion)
        .def_static("areVersionsBlobCompatible", &OpenVINO::areVersionsBlobCompatible)
    ;

    // not strongly typed enum OpenVINO::Version
    // previous step defined class 'OpenVINO' (variable 'openvino') 
    // which is used in defining the following enum (its scope)
    // and that the values are available directly under OpenVINO.VERSION_2020_1, ...
    // they are exported
    // By default, pybind creates strong typed enums, eg: OpenVINO::Version::VERSION_2020_1 
    py::enum_<OpenVINO::Version>(openvino, "Version")
        .value("VERSION_2020_1", OpenVINO::Version::VERSION_2020_1)
        .value("VERSION_2020_2", OpenVINO::Version::VERSION_2020_2)
        .value("VERSION_2020_3", OpenVINO::Version::VERSION_2020_3)
        .value("VERSION_2020_4", OpenVINO::Version::VERSION_2020_4)
        .value("VERSION_2021_1", OpenVINO::Version::VERSION_2021_1)
        .export_values()
    ;

}



