#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/OccupancyPool.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_occupancypool(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawOccupancyPool, RawBuffer, std::shared_ptr<RawOccupancyPool>> rawOccupancyPool(m, "RawOccupancyPool", DOC(dai, RawOccupancyPool));
    py::class_<OccupancyPool, Buffer, std::shared_ptr<OccupancyPool>> occupancyPool(m, "OccupancyPool", DOC(dai, OccupancyPool));

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
    rawOccupancyPool
        .def(py::init<>())
        .def_readwrite("occupancyPool", &RawOccupancyPool::occupancyPool)
        .def_readwrite("occupancyPool3d", &RawOccupancyPool::occupancyPool3d)
        .def_readwrite("sequenceNum", &RawOccupancyPool::sequenceNum)
        .def_readwrite("ts", &RawOccupancyPool::ts)
        .def_readwrite("tsDevice", &RawOccupancyPool::tsDevice)
        ;

    // Message
    occupancyPool
        .def(py::init<>())
        .def_property("occupancyPool", [](OccupancyPool& x) { return &x.occupancyPool; }, [](OccupancyPool& x, std::vector<std::vector<int>> val) { x.occupancyPool = val; }, DOC(dai, OccupancyPool, occupancyPool))
        .def_property("occupancyPool3d", [](OccupancyPool& x) { return &x.occupancyPool3d; }, [](OccupancyPool& x, std::vector<std::vector<std::vector<int>>> val) { x.occupancyPool3d = val; }, DOC(dai, OccupancyPool, occupancyPool3d))
        .def("getTimestamp", &OccupancyPool::getTimestamp, DOC(dai, OccupancyPool, getTimestamp))
        .def("getTimestampDevice", &OccupancyPool::getTimestampDevice, DOC(dai, OccupancyPool, getTimestampDevice))
        .def("getSequenceNum", &OccupancyPool::getSequenceNum, DOC(dai, OccupancyPool, getSequenceNum))
        .def("setTimestamp", &OccupancyPool::setTimestamp, DOC(dai, OccupancyPool, setTimestamp))
        .def("setTimestampDevice", &OccupancyPool::setTimestampDevice, DOC(dai, OccupancyPool, setTimestampDevice))
        .def("setSequenceNum", &OccupancyPool::setSequenceNum, DOC(dai, OccupancyPool, setSequenceNum))
        ;

}
