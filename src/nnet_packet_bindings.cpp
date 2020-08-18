#include "nnet_packet_bindings.hpp"

//std
#include <iostream>
#include <list>
#include <memory>

//project
#include "host_data_packet_bindings.hpp"

// pybind11
#include "pybind11_common.hpp"

namespace py = pybind11;


PYBIND11_MAKE_OPAQUE(std::list<std::shared_ptr<NNetPacket>>);

void init_binding_nnet_packet(pybind11::module& m){
    
 // NNET_PACKETS, data_packets = p.get_available_nnet_and_data_packets()
    py::class_<std::list<std::shared_ptr<NNetPacket>>>(m, "NNetPacketList")
        .def(py::init<>())
        .def("__len__",  [](const std::list<std::shared_ptr<NNetPacket>> &v) { return v.size(); })
        .def("__iter__", [](std::list<std::shared_ptr<NNetPacket>> &v)
        {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep list alive while iterator is used */
        ;


    // for NNET_PACKET in nnet_packets:
    py::class_<NNetPacket, std::shared_ptr<NNetPacket>>(m, "NNetPacket")
        .def("get_tensor", static_cast<py::array* (NNetPacket::*)(unsigned)>(&PyNNetPacket::getTensor), py::return_value_policy::copy)
        .def("get_tensor", static_cast<py::array* (NNetPacket::*)(const std::string&)>(&PyNNetPacket::getTensorByName), py::return_value_policy::copy)
        .def("entries", &NNetPacket::getTensorEntryContainer, py::return_value_policy::copy)
        .def("getMetadata", &NNetPacket::getMetadata, py::return_value_policy::copy)
        ;

}

pybind11::array* PyNNetPacket::getTensor(unsigned index)
{
    auto ptr = std::static_pointer_cast<PyHostDataPacket>(_tensors_raw_data[index]);
    return ptr->getPythonNumpyArray();
}

pybind11::array* PyNNetPacket::getTensorByName(const std::string &name)
{
    auto it = _tensor_name_to_index.find(name);
    if (it == _tensor_name_to_index.end())
    {
        return nullptr;
    }
    else
    {
        return getTensor(it->second);
    }
}
