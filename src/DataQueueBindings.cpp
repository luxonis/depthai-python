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
        .def("get", [](DataOutputQueue& obj){
          

            std::shared_ptr<ADatatype> d = nullptr;
            do {
                {          
                    // releases python GIL
                    py::gil_scoped_release release;

                    // block for 100ms                    
                    d = obj.get(milliseconds(100));
                }

                // reacquires python GIL for PyErr_CheckSignals call

                // check if interrupt triggered in between
                if (PyErr_CheckSignals() != 0) throw py::error_already_set();
            } while(d == nullptr);

            return d;
        })
        .def("has", static_cast<bool(DataOutputQueue::*)()>(&DataOutputQueue::has))
        .def("tryGet", static_cast<std::shared_ptr<ADatatype>(DataOutputQueue::*)()>(&DataOutputQueue::tryGet))
        ;

    // Bind DataInputQueue
    py::class_<DataInputQueue, std::shared_ptr<DataInputQueue>>(m, "DataInputQueue")
        .def("getName", &DataInputQueue::getName)
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