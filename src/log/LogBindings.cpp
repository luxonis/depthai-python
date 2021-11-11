#include "LogBindings.hpp"

// depthai
#include "depthai-shared/log/LogLevel.hpp"

void LogBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Bind LogLevel
    py::enum_<LogLevel> logLevel(m, "LogLevel");


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


    logLevel
        .value("TRACE", LogLevel::TRACE)
        .value("DEBUG", LogLevel::DEBUG)
        .value("INFO", LogLevel::INFO)
        .value("WARN", LogLevel::WARN)
        .value("ERR", LogLevel::ERR)
        .value("CRITICAL", LogLevel::CRITICAL)
        .value("OFF", LogLevel::OFF)
    ;

}