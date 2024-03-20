#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/SpatialLocationCalculatorData.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_spatiallocationcalculatordata(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<SpatialLocationCalculatorData, Buffer, std::shared_ptr<SpatialLocationCalculatorData>> spatialLocationCalculatorData(m, "SpatialLocationCalculatorData", DOC(dai, SpatialLocationCalculatorData));
    py::class_<SpatialLocations> spatialLocations(m, "SpatialLocations", DOC(dai, SpatialLocations));

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
    spatialLocations
        .def(py::init<>())
        .def_readwrite("config", &SpatialLocations::config, DOC(dai, SpatialLocations, config))
        .def_readwrite("depthAverage", &SpatialLocations::depthAverage, DOC(dai, SpatialLocations, depthAverage))
        .def_readwrite("depthMode", &SpatialLocations::depthMode, DOC(dai, SpatialLocations, depthMode))
        .def_readwrite("depthMedian", &SpatialLocations::depthMedian, DOC(dai, SpatialLocations, depthMedian))
        .def_readwrite("depthMin", &SpatialLocations::depthMin, DOC(dai, SpatialLocations, depthMin))
        .def_readwrite("depthMax", &SpatialLocations::depthMax, DOC(dai, SpatialLocations, depthMax))
        .def_readwrite("depthAveragePixelCount", &SpatialLocations::depthAveragePixelCount, DOC(dai, SpatialLocations, depthAveragePixelCount))
        .def_readwrite("spatialCoordinates", &SpatialLocations::spatialCoordinates, DOC(dai, SpatialLocations, spatialCoordinates))
        ;

    // Message
    spatialLocationCalculatorData
        .def(py::init<>())
        .def("getSpatialLocations", &SpatialLocationCalculatorData::getSpatialLocations, DOC(dai, SpatialLocationCalculatorData, getSpatialLocations))
        .def_property("spatialLocations", [](SpatialLocationCalculatorData& loc) { return &loc.spatialLocations; }, [](SpatialLocationCalculatorData& loc, std::vector<SpatialLocations> val) { loc.spatialLocations = val; }, DOC(dai, SpatialLocationCalculatorData, spatialLocations))
        .def("getTimestamp", &SpatialLocationCalculatorData::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &SpatialLocationCalculatorData::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &SpatialLocationCalculatorData::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &SpatialLocationCalculatorData::setTimestamp, DOC(dai, SpatialLocationCalculatorData, setTimestamp))
        .def("setTimestampDevice", &SpatialLocationCalculatorData::setTimestampDevice, DOC(dai, SpatialLocationCalculatorData, setTimestampDevice))
        .def("setSequenceNum", &SpatialLocationCalculatorData::setSequenceNum, DOC(dai, SpatialLocationCalculatorData, setSequenceNum))
        ;


}
