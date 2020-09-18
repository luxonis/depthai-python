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
        .def("get_tensor", static_cast<py::array* (NNetPacket::*)(unsigned)>(&PyNNetPacket::getTensor), py::return_value_policy::take_ownership)
        .def("get_tensor", static_cast<py::array* (NNetPacket::*)(const std::string&)>(&PyNNetPacket::getTensorByName), py::return_value_policy::take_ownership)
        // .def("entries", &NNetPacket::getTensorEntryContainer, py::return_value_policy::copy)
        .def("getMetadata", &NNetPacket::getMetadata, py::return_value_policy::copy)
        .def("getOutputsList", static_cast<py::list (NNetPacket::*)()>(&PyNNetPacket::getOutputsList), py::return_value_policy::copy)     
        .def("getOutputsDict", static_cast<py::dict (NNetPacket::*)()>(&PyNNetPacket::getOutputsDict), py::return_value_policy::copy)     
        .def("getTensorsSize", &NNetPacket::getTensorsSize, py::return_value_policy::copy)     
        .def("getDetectionCount", &NNetPacket::getDetectionCount, py::return_value_policy::copy)
        .def("getDetectedObject",  static_cast<py::object (NNetPacket::*)(int)>(&PyNNetPacket::getDetectedObject), py::return_value_policy::copy)
        ;
    
    py::class_<detection_t>(m, "Detection")
    .def_readonly("label", &detection_t::label)
    .def_readonly("confidence", &detection_t::confidence)
    .def_readonly("x_min", &detection_t::x_min)
    .def_readonly("y_min", &detection_t::y_min)
    .def_readonly("x_max", &detection_t::x_max)
    .def_readonly("y_max", &detection_t::y_max)
    .def_readonly("depth_x", &detection_t::depth_x)
    .def_readonly("depth_y", &detection_t::depth_y)
    .def_readonly("depth_z", &detection_t::depth_z)
    .def("get_dict", []() {
            py::dict d;
            d["label"] = &detection_t::label;
            d["confidence"] = &detection_t::confidence;
            d["x_min"] = &detection_t::x_min;
            d["y_min"] = &detection_t::y_min;
            d["x_max"] = &detection_t::x_max;
            d["y_max"] = &detection_t::y_max;
            d["depth_x"] = &detection_t::depth_x;
            d["depth_y"] = &detection_t::depth_y;
            d["depth_z"] = &detection_t::depth_z;
            return d;
        })
    ;

}




namespace pybind11 { namespace detail {
template <>
    struct npy_format_descriptor<float16> {
    static std::string format() {
        // https://docs.python.org/3/library/struct.html#format-characters
        return "e";
    }   
    };
}} // namespace pybind11::detail

const std::map<TensorDataType, std::string> type_to_numpy_format = {
    {TensorDataType::_fp16,     pybind11::format_descriptor<float16>::format()},
    {TensorDataType::_u8f,      pybind11::format_descriptor<std::uint8_t>::format()},
    {TensorDataType::_int,      pybind11::format_descriptor<std::int32_t>::format()},
    {TensorDataType::_fp32,     pybind11::format_descriptor<float>::format()},
    {TensorDataType::_i8,       pybind11::format_descriptor<std::int8_t>::format()},
};

std::string type_to_npy_format_descriptor(const TensorDataType& type)
{
    auto it = type_to_numpy_format.find(type);
    assert(it != type_to_numpy_format.end());
    return it->second;
};


py::object PyNNetPacket::getDetectedObject(int detected_nr)
{
    unsigned char * data = _tensors_raw_data->data.data();
    detection_out_t * detections = (detection_out_t *)data;
    assert(detected_nr < detections->detection_count);
    return py::cast<detection_t>(detections->detections[detected_nr]);
}

// TODO - zero copy
//https://github.com/pybind/pybind11/issues/323#issuecomment-575717041
//https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
static py::array* _getTensorPythonNumpyArray(unsigned char *data, TensorInfo ti)
{
    assert(!ti.tensor_dimensions.empty());
    py::array* result = nullptr;

    ssize_t              ndim    = ti.tensor_dimensions.size();
    ssize_t              element_size = size_of_type(ti.tensor_data_type);
    std::string          numpy_format_descriptor = type_to_npy_format_descriptor(ti.tensor_data_type);
    std::vector<ssize_t> shape;
    std::vector<ssize_t> strides;
    shape.reserve(ndim);
    strides.reserve(ndim);

    for (int i = 0; i < ndim; ++i)
    {
        shape.push_back(ti.tensor_dimensions[i]);
        strides.push_back(ti.tensor_strides[i]);
    }

    try {

        result = new py::array(py::buffer_info(
                    static_cast<void*>(&data[ti.tensor_offset]),                             /* data as contiguous array  */
                    element_size,                          /* size of one scalar        */
                    numpy_format_descriptor,         /* data type          */
                    ndim, //ndim,                                    /* number of dimensions      */
                    shape, //shape,                                   /* shape of the matrix       */
                    strides //strides                                  /* strides for each axis     */
                ));
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        result = nullptr;
    }
    
    return result;
}

py::array* PyNNetPacket::getTensor(unsigned index)
{
    assert(index < _tensors_info.size());
    TensorInfo ti = _tensors_info[index];
    unsigned char * data = _tensors_raw_data->data.data();
    return _getTensorPythonNumpyArray(data, ti);
}

py::array* PyNNetPacket::getTensorByName(const std::string &name)
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


py::list PyNNetPacket::getOutputsList() {
    py::list outputList;
    for (size_t i = 0; i < _tensors_info.size(); ++i)
    {
        outputList.append(getTensor(i));
    }
    return outputList;
}


py::dict PyNNetPacket::getOutputsDict() {
    py::dict outputs;
    for (size_t i = 0; i < _tensors_info.size(); ++i)
    {
        std::string tensor_name = getTensorName(i);
        outputs[tensor_name.c_str()] = getTensor(i);
    }
    
    return outputs;
}   

