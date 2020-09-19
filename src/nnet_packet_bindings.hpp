#pragma once

//pybind11
#include "pybind11_common.hpp"

// depthai-api
#include "depthai/nnet/nnet_packet.hpp"

void init_binding_nnet_packet(pybind11::module& m);

namespace py = pybind11;

struct PyNNetPacket : public NNetPacket {

    py::object getDetectedObject(int detected_nr);
    py::array* getTensor(unsigned index);
    pybind11::array* getTensorByName(const std::string &name);
    std::list<py::array*> getOutputsList();
    std::unordered_map<std::string, py::array*> getOutputsDict();
    
public:
    using NNetPacket::_tensors_raw_data;
    using NNetPacket::_tensor_name_to_index;


};