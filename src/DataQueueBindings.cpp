#include "DataQueueBindings.hpp"

// std
#include <chrono>

// depthai
#include "depthai/DataQueue.hpp"

void DataQueueBindings::bind(pybind11::module& m){

    using namespace dai;
    using namespace std::chrono;

    // To prevent blocking whole python interpreter, blocking functions like 'get' and 'send' 
    // are pooled with a reasonable delay and check for python interrupt signal in between.

    // Bind DataOutputQueue
    py::class_<DataOutputQueue, std::shared_ptr<DataOutputQueue>>(m, "DataOutputQueue")
        .def("get", [](DataOutputQueue& obj){
            
            std::shared_ptr<RawBuffer> d = nullptr;
            do {
                // block for 100ms
                d = obj.get(milliseconds(100));
                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();
            } while(d == nullptr);

            return d;
        })
        .def("has", static_cast<bool(DataOutputQueue::*)()>(&DataOutputQueue::has))
        .def("tryGet", static_cast<std::shared_ptr<RawBuffer>(DataOutputQueue::*)()>(&DataOutputQueue::tryGet))
        ;

    // Bind DataInputQueue
    py::class_<DataInputQueue, std::shared_ptr<DataInputQueue>>(m, "DataInputQueue")
        .def("send", [](DataInputQueue& obj, std::shared_ptr<RawBuffer> d){
            
            bool sent = false;
            do {
                // block for 100ms
                sent = obj.send(d, milliseconds(100));

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();

            } while(!sent);

            return d;
        })
        ;

}