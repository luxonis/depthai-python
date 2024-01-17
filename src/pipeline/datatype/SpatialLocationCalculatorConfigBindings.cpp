#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/SpatialLocationCalculatorConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_spatiallocationcalculatorconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<SpatialLocationCalculatorConfigThresholds> spatialLocationCalculatorConfigThresholds(m, "SpatialLocationCalculatorConfigThresholds", DOC(dai, SpatialLocationCalculatorConfigThresholds));
    py::enum_<SpatialLocationCalculatorAlgorithm> spatialLocationCalculatorAlgorithm(m, "SpatialLocationCalculatorAlgorithm", DOC(dai, SpatialLocationCalculatorAlgorithm));
    py::class_<SpatialLocationCalculatorConfigData> spatialLocationCalculatorConfigData(m, "SpatialLocationCalculatorConfigData", DOC(dai, SpatialLocationCalculatorConfigData));
    py::class_<SpatialLocationCalculatorConfig, Buffer, std::shared_ptr<SpatialLocationCalculatorConfig>> spatialLocationCalculatorConfig(m, "SpatialLocationCalculatorConfig", DOC(dai, SpatialLocationCalculatorConfig));

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

    // Metadata / raw
    spatialLocationCalculatorConfigThresholds
        .def(py::init<>())
        .def_readwrite("lowerThreshold", &SpatialLocationCalculatorConfigThresholds::lowerThreshold)
        .def_readwrite("upperThreshold", &SpatialLocationCalculatorConfigThresholds::upperThreshold)
        ;

    spatialLocationCalculatorAlgorithm
        .value("AVERAGE", SpatialLocationCalculatorAlgorithm::AVERAGE)
        .value("MEAN", SpatialLocationCalculatorAlgorithm::MEAN)
        .value("MIN", SpatialLocationCalculatorAlgorithm::MIN)
        .value("MAX", SpatialLocationCalculatorAlgorithm::MAX)
        .value("MODE", SpatialLocationCalculatorAlgorithm::MODE)
        .value("MEDIAN", SpatialLocationCalculatorAlgorithm::MEDIAN)
        ;

    spatialLocationCalculatorConfigData
        .def(py::init<>())
        .def_readwrite("roi", &SpatialLocationCalculatorConfigData::roi, DOC(dai, SpatialLocationCalculatorConfigData, roi))
        .def_readwrite("depthThresholds", &SpatialLocationCalculatorConfigData::depthThresholds, DOC(dai, SpatialLocationCalculatorConfigData, depthThresholds))
        .def_readwrite("calculationAlgorithm", &SpatialLocationCalculatorConfigData::calculationAlgorithm, DOC(dai, SpatialLocationCalculatorConfigData, calculationAlgorithm))
        ;

    // Message
    spatialLocationCalculatorConfig
        .def(py::init<>())
        // setters
        .def("setROIs", &SpatialLocationCalculatorConfig::setROIs, py::arg("ROIs"), DOC(dai, SpatialLocationCalculatorConfig, setROIs))
        .def("addROI", &SpatialLocationCalculatorConfig::addROI, py::arg("ROI"), DOC(dai, SpatialLocationCalculatorConfig, addROI))
        .def("getConfigData", &SpatialLocationCalculatorConfig::getConfigData, DOC(dai, SpatialLocationCalculatorConfig, getConfigData))
        // .def("set",         &SpatialLocationCalculatorConfig::set, py::arg("config"), DOC(dai, SpatialLocationCalculatorConfig, set))
        // .def("get",         &SpatialLocationCalculatorConfig::get, DOC(dai, SpatialLocationCalculatorConfig, get))
        ;

}
