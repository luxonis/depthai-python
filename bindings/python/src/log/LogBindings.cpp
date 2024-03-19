#include "LogBindings.hpp"

// depthai
#include "depthai/log/LogLevel.hpp"
#include "depthai/log/LogMessage.hpp"

void LogBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Bind LogLevel
    py::enum_<LogLevel> logLevel(m, "LogLevel");
    py::class_<LogMessage> logMessage(m, "LogMessage", DOC(dai, LogMessage));


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

    logMessage
        .def_readwrite("nodeIdName", &LogMessage::nodeIdName)
        .def_readwrite("level", &LogMessage::level)
        .def_readwrite("time", &LogMessage::time)
        .def_readwrite("colorRangeStart", &LogMessage::colorRangeStart)
        .def_readwrite("colorRangeEnd", &LogMessage::colorRangeEnd)
        .def_readwrite("payload", &LogMessage::payload)
    ;

}