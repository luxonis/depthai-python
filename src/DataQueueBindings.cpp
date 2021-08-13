#include "DataQueueBindings.hpp"

// std
#include <chrono>

// depthai
#include "depthai/device/DataQueue.hpp"

void DataQueueBindings::bind(pybind11::module& m, void* pCallstack){
    using namespace dai;
    using namespace std::chrono;


    // Type definitions
    py::class_<DataOutputQueue, std::shared_ptr<DataOutputQueue>> dataOutputQueue(m, "DataOutputQueue", DOC(dai, DataOutputQueue));
    py::class_<DataInputQueue, std::shared_ptr<DataInputQueue>> dataInputQueue(m, "DataInputQueue", DOC(dai, DataInputQueue));


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


    // To prevent blocking whole python interpreter, blocking functions like 'get' and 'send'
    // are pooled with a reasonable delay and check for python interrupt signal in between.

    // Bind DataOutputQueue
    auto addCallbackLambda = [](DataOutputQueue& q, py::function cb) -> int {
        pybind11::module inspect_module = pybind11::module::import("inspect");
        pybind11::object result = inspect_module.attr("signature")(cb).attr("parameters");
        auto numParams = pybind11::len(result);
        if(numParams == 2){
            return q.addCallback(cb.cast<std::function<void(std::string, std::shared_ptr<ADatatype>)>>());
        } else if (numParams == 1){
            return q.addCallback(cb.cast<std::function<void(std::shared_ptr<ADatatype>)>>());
        } else if (numParams == 0){
            return q.addCallback(cb.cast<std::function<void()>>());
        } else {
            throw py::value_error("Callback must take either zero, one or two arguments");
        }
    };
    dataOutputQueue
        .def("getName", &DataOutputQueue::getName, DOC(dai, DataOutputQueue, getName))
        .def("isClosed", &DataOutputQueue::isClosed, DOC(dai, DataOutputQueue, isClosed))
        .def("close", &DataOutputQueue::close, DOC(dai, DataOutputQueue, close))

        .def("addCallback", addCallbackLambda, py::arg("callback"), DOC(dai, DataOutputQueue, addCallback))
        .def("addCallback", addCallbackLambda, py::arg("callback"), DOC(dai, DataOutputQueue, addCallback, 2))
        .def("addCallback", addCallbackLambda, py::arg("callback"), DOC(dai, DataOutputQueue, addCallback, 3))
        .def("removeCallback", &DataOutputQueue::removeCallback, py::arg("callbackId"), DOC(dai, DataOutputQueue, removeCallback))

        .def("setBlocking", &DataOutputQueue::setBlocking, py::arg("blocking"), DOC(dai, DataOutputQueue, setBlocking))
        .def("getBlocking", &DataOutputQueue::getBlocking, DOC(dai, DataOutputQueue, getBlocking))
        .def("setMaxSize", &DataOutputQueue::setMaxSize, py::arg("maxSize"), DOC(dai, DataOutputQueue, setMaxSize))
        .def("getMaxSize", &DataOutputQueue::getMaxSize, DOC(dai, DataOutputQueue, getMaxSize))
        .def("getAll", [](DataOutputQueue& obj){

            std::vector<std::shared_ptr<ADatatype>> messages;
            bool timedout = true;
            do {
                {
                    // releases python GIL
                    py::gil_scoped_release release;

                    // block for 100ms
                    messages = obj.getAll(milliseconds(100), timedout);
                }

                // reacquires python GIL for PyErr_CheckSignals call

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();

            } while(timedout); // Keep reiterating until a message is received (not timedout)

            return messages;
        }, DOC(dai, DataOutputQueue, getAll, 2))
        .def("get", [](DataOutputQueue& obj){

            std::shared_ptr<ADatatype> d = nullptr;
            bool timedout = true;
            do {
                {
                    // releases python GIL
                    py::gil_scoped_release release;

                    // block for 100ms
                    d = obj.get(milliseconds(100), timedout);
                }

                // reacquires python GIL for PyErr_CheckSignals call

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();

            } while(timedout);

            return d;
        }, DOC(dai, DataOutputQueue, get, 2))
        .def("has", static_cast<bool(DataOutputQueue::*)()>(&DataOutputQueue::has), DOC(dai, DataOutputQueue, has, 2))
        .def("tryGet", static_cast<std::shared_ptr<ADatatype>(DataOutputQueue::*)()>(&DataOutputQueue::tryGet), DOC(dai, DataOutputQueue, tryGet, 2))
        .def("tryGetAll", static_cast<std::vector<std::shared_ptr<ADatatype>>(DataOutputQueue::*)()>(&DataOutputQueue::tryGetAll), DOC(dai, DataOutputQueue, tryGetAll, 2))
        ;

    // Bind DataInputQueue
    dataInputQueue
        .def("isClosed", &DataInputQueue::isClosed, DOC(dai, DataInputQueue, isClosed))
        .def("close", &DataInputQueue::close, DOC(dai, DataInputQueue, close))
        .def("getName", &DataInputQueue::getName, DOC(dai, DataInputQueue, getName))
        .def("setBlocking", &DataInputQueue::setBlocking, py::arg("blocking"), DOC(dai, DataInputQueue, setBlocking))
        .def("getBlocking", &DataInputQueue::getBlocking, DOC(dai, DataInputQueue, getBlocking))
        .def("setMaxSize", &DataInputQueue::setMaxSize, py::arg("maxSize"), DOC(dai, DataInputQueue, setMaxSize))
        .def("getMaxSize", &DataInputQueue::getMaxSize, DOC(dai, DataInputQueue, getMaxSize))
        .def("send", [](DataInputQueue& obj, std::shared_ptr<ADatatype> d){

            bool sent = false;
            do {

                // block for 100ms
                {
                    // Release GIL, then block
                    py::gil_scoped_release release;
                    sent = obj.send(d, milliseconds(100));
                }

                // reacquires GIL as PyErr_CheckSignals requires GIL

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();

            } while(!sent);

        }, py::arg("msg"), DOC(dai, DataInputQueue, send, 2))
        .def("send", [](DataInputQueue& obj, std::shared_ptr<dai::RawBuffer> d){

            bool sent = false;
            do {
                 // block for 100ms
                {
                    // Release GIL, then block
                    py::gil_scoped_release release;
                    sent = obj.send(d, milliseconds(100));
                }
                // reacquires GIL as PyErr_CheckSignals requires GIL

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();

            } while(!sent);

        }, py::arg("rawMsg"), DOC(dai, DataInputQueue, send))
        ;

}