#include "DatatypeBindings.hpp"
#include "depthai-shared/datatype/RawMessageGroup.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/MessageGroup.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_message_group(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawMessageGroup, RawBuffer, std::shared_ptr<RawMessageGroup>> rawMessageGroup(m, "RawMessageGroup", DOC(dai, RawMessageGroup));
    py::class_<MessageGroup, Buffer, std::shared_ptr<MessageGroup>> messageGroup(m, "MessageGroup", DOC(dai, MessageGroup));

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
    rawMessageGroup
        .def(py::init<>())
        .def_readwrite("group", &RawMessageGroup::group)
        ;

    // Message
    messageGroup
        .def(py::init<>())
        .def("__getitem__", [](MessageGroup& msg, const std::string& name) {
            std::shared_ptr<ADatatype> data = nullptr;
            return msg[name];
        })
        .def("__setitem__", [](MessageGroup& msg, const std::string& name, std::shared_ptr<ADatatype> data) {
            return msg.add(name, data);
        })
        .def("isSynced", &MessageGroup::isSynced, DOC(dai, MessageGroup, isSynced))
        .def("getIntervalNs", &MessageGroup::getIntervalNs, DOC(dai, MessageGroup, getIntervalNs))
        .def("getNumMessages", &MessageGroup::getNumMessages, DOC(dai, MessageGroup, getNumMessages))
        .def("getMessageNames", &MessageGroup::getMessageNames, DOC(dai, MessageGroup, getMessageNames))
        .def("getTimestamp", &MessageGroup::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &MessageGroup::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &MessageGroup::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &MessageGroup::setTimestamp, DOC(dai, MessageGroup, setTimestamp))
        .def("setTimestampDevice", &MessageGroup::setTimestampDevice, DOC(dai, MessageGroup, setTimestampDevice))
        .def("setSequenceNum", &MessageGroup::setSequenceNum, DOC(dai, MessageGroup, setSequenceNum))
        ;

}
