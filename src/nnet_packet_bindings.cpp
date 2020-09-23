#include "nnet_packet_bindings.hpp"

//std
#include <iostream>
#include <list>
#include <memory>

//project
#include "host_data_packet_bindings.hpp"
#include "depthai-shared/cnn_info.hpp"

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
        .def("__getitem__", static_cast<py::array* (NNetPacket::*)(unsigned)>(&PyNNetPacket::getTensor), py::return_value_policy::take_ownership)
        .def("__getitem__", static_cast<py::array* (NNetPacket::*)(const std::string&)>(&PyNNetPacket::getTensorByName), py::return_value_policy::take_ownership)
        // .def("entries", &NNetPacket::getTensorEntryContainer, py::return_value_policy::copy)
        .def("getMetadata", &NNetPacket::getMetadata, py::return_value_policy::copy)
        .def("getOutputsList", static_cast<std::list<py::array*> (NNetPacket::*)()>(&PyNNetPacket::getOutputsList), py::return_value_policy::take_ownership)     
        .def("getOutputsDict", static_cast<std::map<std::string, py::array*> (NNetPacket::*)()>(&PyNNetPacket::getOutputsDict), py::return_value_policy::take_ownership)     
        .def("getTensorsSize", &NNetPacket::getTensorsSize, py::return_value_policy::copy)     
        .def("getOutputLayersInfo", &NNetPacket::getOutputLayersInfo, py::return_value_policy::copy)
        .def("getInputLayersInfo", &NNetPacket::getInputLayersInfo, py::return_value_policy::copy)
        .def("getDetectedObjects", &NNetPacket::getDetectedObjects)  
        ;
    
    py::class_<TensorDataType>(m, "TensorDataType")
        .def("__str__", [](const TensorDataType& v) {
            return type_to_string.at(v);
        })
        .def("__repr__", [](const TensorDataType& v) {
            return type_to_string.at(v);
        })
        ;

    py::enum_<dimension>(m, "dimension")
        .value("W", dimension::W)
        .value("H", dimension::H)
        .value("C ", dimension::C)
        .value("N", dimension::N)
        .value("B", dimension::B)
        .value("WIDTH", dimension::WIDTH)
        .value("HEIGHT", dimension::HEIGHT)
        .value("CHANNEL", dimension::CHANNEL)
        .value("NUMBER", dimension::NUMBER)
        .value("BATCH", dimension::BATCH)
        .export_values()
        ;

    py::class_<TensorInfo>(m, "TensorInfo")
        .def("get_dimension", &TensorInfo::get_dimension)
        .def_readonly("tensor_name", &TensorInfo::tensor_name)
        .def_readonly("tensor_dimensions", &TensorInfo::tensor_dimensions)
        .def_readonly("tensor_strides", &TensorInfo::tensor_strides)
        .def_readonly("tensor_data_type", &TensorInfo::tensor_data_type)
        .def_readonly("tensor_offset", &TensorInfo::tensor_offset)
        .def_readonly("tensor_element_size", &TensorInfo::tensor_element_size)
        .def_readonly("tensor_idx", &TensorInfo::tensor_idx)
        .def("get_dict", []() {
            py::dict d;
            d["tensor_name"] = &TensorInfo::tensor_name;
            d["tensor_dimensions"] = &TensorInfo::tensor_dimensions;
            d["tensor_strides"] = &TensorInfo::tensor_strides;
            d["tensor_data_type"] = &TensorInfo::tensor_data_type;
            d["tensor_offset"] = &TensorInfo::tensor_offset;
            d["tensor_element_size"] = &TensorInfo::tensor_element_size;
            d["tensor_idx"] = &TensorInfo::tensor_idx;
            return d;
        })
        .def("__str__", [](const TensorInfo& v) {
            std::stringstream stream;
            stream << v;
            return stream.str(); 
        })
        .def("__repr__", [](const TensorInfo& v) {
            std::stringstream stream;
            stream << v;
            return stream.str(); 
        })
        ;

    py::class_<detection_out_t, std::shared_ptr<detection_out_t>>(m, "Detections")
        .def_readonly("size", &detection_out_t::detection_count)
        .def("__len__",  [](const detection_out_t &det) { return det.detection_count; })
        .def("__getitem__", [](const detection_out_t &det, int idx) { return det.detections[idx]; })
        .def("__iter__", [](std::shared_ptr<detection_out_t> &v)
        {
            const detection_t *detection_vec = v->detections;
            return py::make_iterator(&detection_vec[0], &detection_vec[v->detection_count]);
        }, py::keep_alive<0, 1>()) /* Keep list alive while iterator is used */
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




// TODO - zero copy
//https://github.com/pybind/pybind11/issues/323#issuecomment-575717041
//https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
static py::array* _getTensorPythonNumpyArray(unsigned char *data, TensorInfo ti)
{
    assert(!ti.tensor_dimensions.empty());
    py::array* result = nullptr;

    ssize_t              ndim    = ti.tensor_dimensions.size();
    ssize_t              element_size = c_type_size.at(ti.tensor_data_type);
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
    unsigned char * data = _tensors_raw_data->data->data();
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


std::list<py::array*> PyNNetPacket::getOutputsList() {

    std::list<py::array*> outputList;
    for (size_t i = 0; i < _tensors_info.size(); ++i)
    {
        outputList.push_back(getTensor(i));
    }
    return outputList;
}


std::map<std::string, py::array*> PyNNetPacket::getOutputsDict() {
    std::map<std::string, py::array*> outputs;
    for (size_t i = 0; i < _tensors_info.size(); ++i)
    {
        std::string tensor_name = getTensorName(i);
        // outputs[tensor_name.c_str()] = getTensor(i);
        outputs.insert({tensor_name,getTensor(i)});
    }
    
    return outputs;
}   

