#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "pybind11_common.hpp"


#ifdef _MSC_VER 
    #define HAVE_SNPRINTF
#endif
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <boost/algorithm/string/replace.hpp>

// depthai-core
#include "depthai/device.hpp"

// project
#include "host_data_packet_bindings.hpp"
#include "nnet_packet_bindings.hpp"
#include "py_tensor_entry_container_iterator.hpp"
#include "device_bindings.hpp"




namespace py = pybind11;
PYBIND11_MODULE(depthai,m)
{

    init_binding_host_data_packet(m);
    init_binding_nnet_packet(m);
    init_binding_device(m);

    // TODO: test ownership in python

    // TODO
    //std::string _version = c_depthai_version;
    m.attr("__version__") = "0.0.1";
    //std::string _dev_version = c_depthai_dev_version;
    m.attr("__dev_version__") = "badf00d";


    // for te in nnet_packet.ENTRIES()
    py::class_<TensorEntryContainer, std::shared_ptr<TensorEntryContainer>>(m, "TensorEntryContainer")
        .def("__len__", &TensorEntryContainer::size)
        .def("__getitem__", &TensorEntryContainer::getByIndex)
        .def("__getitem__", &TensorEntryContainer::getByName)
        .def("__iter__", [](py::object s) { return PyTensorEntryContainerIterator(s.cast<TensorEntryContainer &>(), s); })
        ;

    // for e in nnet_packet.entries():
    //     e <--- (type(e) == list)
    py::class_<PyTensorEntryContainerIterator>(m, "PyTensorEntryContainerIterator")
        .def("__iter__", [](PyTensorEntryContainerIterator &it) -> PyTensorEntryContainerIterator& { return it; })
        .def("__next__", &PyTensorEntryContainerIterator::next)
        ;

    // for e in nnet_packet.entries():
    //     e[0] <--
    py::class_<TensorEntry, std::shared_ptr<TensorEntry>>(m, "TensorEntry")
        .def("__len__", &TensorEntry::getPropertiesNumber)
        .def("__getitem__", &TensorEntry::getFloat)
        .def("__getitem__", &TensorEntry::getFloatByIndex)
        ;


    // while True:
    //     nnet_packets, data_packets = p.get_available_nnet_and_data_packets()
    //     # nnet_packets: depthai.NNetPacketList
    //     # data_packets: depthai.DataPacketList

    //     for t in nnet_packets.getTensors():
    //         pass

    //     for nnet_packet in nnet_packets:
    //         # nnet_packet: depthai.NNetPacket
    //         # nnet_packet.entries(): depthai.TensorEntryContainer

    //         for e in nnet_packet.entries():
    //             # e: list
    //             # e[0]: depthai.TensorEntry
    //             # e[0][0]: float


    py::class_<HostPipeline>(m, "Pipeline")
        .def("get_available_data_packets", &HostPipeline::getAvailableDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        ;

    py::class_<CNNHostPipeline, std::shared_ptr<CNNHostPipeline>>(m, "CNNPipeline")
        .def("get_available_data_packets", &CNNHostPipeline::getAvailableDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        .def("get_available_nnet_and_data_packets", &CNNHostPipeline::getAvailableNNetAndDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        ;


    // module destructor
    
    auto cleanup_callback = []() {
        /*
        wdog_stop();
        deinit_device();
        gl_result = nullptr;
        */
    };


    //m.add_object("_cleanup", py::capsule(cleanup_callback));
}



