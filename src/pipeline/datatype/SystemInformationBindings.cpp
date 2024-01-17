#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/SystemInformation.hpp"
#include "depthai/pipeline/datatype/SystemInformationS3.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_systeminformation(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawSystemInformation, RawBuffer, std::shared_ptr<RawSystemInformation>> rawSystemInformation(m, "RawSystemInformation", DOC(dai, RawSystemInformation));
    py::class_<SystemInformation, Buffer, std::shared_ptr<SystemInformation>> systemInformation(m, "SystemInformation", DOC(dai, SystemInformation));

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

    // // Metadata / raw
    // rawSystemInformation
    //     .def(py::init<>())
    //     .def_readwrite("ddrMemoryUsage", &RawSystemInformation::ddrMemoryUsage)
    //     .def_readwrite("cmxMemoryUsage", &RawSystemInformation::cmxMemoryUsage)
    //     .def_readwrite("leonCssMemoryUsage", &RawSystemInformation::leonCssMemoryUsage)
    //     .def_readwrite("leonMssMemoryUsage", &RawSystemInformation::leonMssMemoryUsage)
    //     .def_readwrite("leonCssCpuUsage", &RawSystemInformation::leonCssCpuUsage)
    //     .def_readwrite("leonMssCpuUsage", &RawSystemInformation::leonMssCpuUsage)
    //     .def_readwrite("chipTemperature", &RawSystemInformation::chipTemperature)
    //     ;

    // Message
    systemInformation
        .def(py::init<>())
        .def_property("ddrMemoryUsage", [](SystemInformation& i) { return &i.ddrMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.ddrMemoryUsage = val; } )
        .def_property("cmxMemoryUsage", [](SystemInformation& i) { return &i.cmxMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.cmxMemoryUsage = val; } )
        .def_property("leonCssMemoryUsage", [](SystemInformation& i) { return &i.leonCssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonCssMemoryUsage = val; } )
        .def_property("leonMssMemoryUsage", [](SystemInformation& i) { return &i.leonMssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonMssMemoryUsage = val; } )
        .def_property("leonCssCpuUsage", [](SystemInformation& i) { return &i.leonCssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonCssCpuUsage = val; } )
        .def_property("leonMssCpuUsage", [](SystemInformation& i) { return &i.leonMssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonMssCpuUsage = val; } )
        .def_property("chipTemperature", [](SystemInformation& i) { return &i.chipTemperature; }, [](SystemInformation& i, ChipTemperature val) { i.chipTemperature = val; } )
        ;

}

void bind_systeminformationS3(pybind11::module& m, void* pCallstack) {
    using namespace dai;

    // py::class_<RawSystemInformationS3, RawBuffer, std::shared_ptr<RawSystemInformationS3>> rawSystemInformationS3(
        // m, "RawSystemInformationS3", DOC(dai, RawSystemInformationS3));
    py::class_<SystemInformationS3, Buffer, std::shared_ptr<SystemInformationS3>> systemInformationS3(m, "SystemInformationS3", DOC(dai, SystemInformationS3));

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

    // // Metadata / raw
    // rawSystemInformationS3.def(py::init<>())
    //     .def_readwrite("ddrMemoryUsage", &RawSystemInformationS3::ddrMemoryUsage)
    //     .def_readwrite("cpuAvgUsage", &RawSystemInformationS3::cpuAvgUsage)
    //     .def_readwrite("chipTemperature", &RawSystemInformationS3::chipTemperature)
    //     .def_readwrite("cpuUsages", &RawSystemInformationS3::cpuUsages);

    // Message
    systemInformationS3.def(py::init<>())
        .def_property(
            "ddrMemoryUsage", [](SystemInformationS3& i) { return &i.ddrMemoryUsage; }, [](SystemInformationS3& i, MemoryInfo val) { i.ddrMemoryUsage = val; })
        .def_property(
            "cpuAvgUsage", [](SystemInformationS3& i) { return &i.cpuAvgUsage; }, [](SystemInformationS3& i, CpuUsage val) { i.cpuAvgUsage = val; })
        .def_property("cpuUsages", [](SystemInformationS3& i) { return &i.cpuUsages; }, [](SystemInformationS3& i, std::vector<CpuUsage> val) { i.cpuUsages = val; })
        .def_property(
            "chipTemperature",
            [](SystemInformationS3& i) { return &i.chipTemperature; },
            [](SystemInformationS3& i, ChipTemperatureS3 val) { i.chipTemperature = val; });
}
