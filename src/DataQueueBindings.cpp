#include "DataQueueBindings.hpp"

// depthai-shared
#include "depthai/DataQueue.hpp"

void DataQueueBindings::bind(pybind11::module& m){

    using namespace dai;

    // Bind DataOutputQueue
    py::class_<DataOutputQueue, std::shared_ptr<DataOutputQueue>>(m, "DataOutputQueue")
        .def("get", static_cast<std::shared_ptr<RawBuffer>(DataOutputQueue::*)()>(&DataOutputQueue::get) )
        .def("has", static_cast<bool(DataOutputQueue::*)()>(&DataOutputQueue::has))
        .def("tryGet", static_cast<std::shared_ptr<RawBuffer>(DataOutputQueue::*)()>(&DataOutputQueue::tryGet))
        ;

    // Bind DataInputQueue
    py::class_<DataInputQueue, std::shared_ptr<DataInputQueue>>(m, "DataInputQueue")
        .def("send", &DataInputQueue::send)
        .def("sendAsync", &DataInputQueue::sendAsync)
        ;

}