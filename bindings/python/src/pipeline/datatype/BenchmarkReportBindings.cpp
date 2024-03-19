#include <memory>

#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"

// depthai
#include "depthai/pipeline/datatype/BenchmarkReport.hpp"

void bind_benchmarkreport(pybind11::module& m, void* pCallstack) {
    using namespace dai;

    // py::class_<RawBenchmarkReport, RawBuffer, std::shared_ptr<RawBenchmarkReport>> rawBenchmarkReport(m, "RawBenchmarkReport", DOC(dai, RawBenchmarkReport));
    py::class_<BenchmarkReport, Buffer, std::shared_ptr<BenchmarkReport>> benchmarkReport(m, "BenchmarkReport", DOC(dai, BenchmarkReport));

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*)pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Message
    benchmarkReport.def(py::init<>())
        .def_property_readonly("fps", [](BenchmarkReport& i) { return &i.fps; })
        .def_property_readonly("timeTotal", [](BenchmarkReport& i) { return &i.timeTotal; })
        .def_property_readonly("numMessagesReceived", [](BenchmarkReport& i) { return &i.numMessagesReceived; })
        .def_property_readonly("latencies", [](BenchmarkReport& i) { return &i.latencies; })
        .def_property_readonly("averageLatency", [](BenchmarkReport& i) { return &i.averageLatency; });
}
