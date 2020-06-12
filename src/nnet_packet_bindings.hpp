#pragma once

//pybind11
#include "pybind11_common.hpp"

// depthai-api
#include "depthai/nnet/nnet_packet.hpp"

void init_binding_nnet_packet(pybind11::module& m);

struct PyNNetPacket : public NNetPacket {

    pybind11::array* getTensor(unsigned index);
    pybind11::array* getTensorByName(const std::string &name);
public:
    using NNetPacket::_tensors_raw_data;
    using NNetPacket::_tensor_name_to_index;


};