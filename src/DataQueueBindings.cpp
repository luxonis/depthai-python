#include "DataQueueBindings.hpp"

// std
#include <chrono>

// depthai
#include "depthai/device/DataQueue.hpp"

void DataQueueBindings::bind(pybind11::module& m){

    using namespace dai;
    using namespace std::chrono;

    // To prevent blocking whole python interpreter, blocking functions like 'get' and 'send' 
    // are pooled with a reasonable delay and check for python interrupt signal in between.

    // Bind DataOutputQueue
    py::class_<DataOutputQueue, std::shared_ptr<DataOutputQueue>>(m, "DataOutputQueue")
        .def("getName", &DataOutputQueue::getName)

        .def("addCallback", static_cast<int(DataOutputQueue::*)(std::function<void(std::string, std::shared_ptr<ADatatype>)>)>(&DataOutputQueue::addCallback), py::arg("callback"))
        .def("addCallback", static_cast<int(DataOutputQueue::*)(std::function<void(std::shared_ptr<ADatatype>)>)>(&DataOutputQueue::addCallback), py::arg("callback"))
        .def("addCallback", static_cast<int(DataOutputQueue::*)(std::function<void()>)>(&DataOutputQueue::addCallback), py::arg("callback"))
        .def("removeCallback", &DataOutputQueue::removeCallback, py::arg("callbackId"))

        .def("setBlocking", &DataOutputQueue::setBlocking, py::arg("blocking"))
        .def("getBlocking", &DataOutputQueue::getBlocking)
        .def("setMaxSize", &DataOutputQueue::setMaxSize, py::arg("maxSize"))
        .def("getMaxSize", &DataOutputQueue::getMaxSize)
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
        })
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
        })
        .def("has", static_cast<bool(DataOutputQueue::*)()>(&DataOutputQueue::has))
        .def("tryGet", static_cast<std::shared_ptr<ADatatype>(DataOutputQueue::*)()>(&DataOutputQueue::tryGet))
        .def("tryGetAll", static_cast<std::vector<std::shared_ptr<ADatatype>>(DataOutputQueue::*)()>(&DataOutputQueue::tryGetAll))
        ;

    // Bind DataInputQueue
    py::class_<DataInputQueue, std::shared_ptr<DataInputQueue>>(m, "DataInputQueue")
        .def("getName", &DataInputQueue::getName)
        .def("setBlocking", &DataInputQueue::setBlocking, py::arg("blocking"))
        .def("getBlocking", &DataInputQueue::getBlocking)
        .def("setMaxSize", &DataInputQueue::setMaxSize, py::arg("maxSize"))
        .def("getMaxSize", &DataInputQueue::getMaxSize)
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

        })
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

        })
        ;

}