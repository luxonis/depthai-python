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
using TensorInfo = dai::TensorInfo;
using Detection = dai::Detection;
using Detections = dai::Detections;

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
        .def("getMetadata", &NNetPacket::getMetadata, py::return_value_policy::copy)
        .def("getOutputsList", static_cast<std::list<py::array*> (NNetPacket::*)()>(&PyNNetPacket::getOutputsList), py::return_value_policy::take_ownership)     
        .def("getOutputsDict", static_cast<std::map<std::string, py::array*> (NNetPacket::*)()>(&PyNNetPacket::getOutputsDict), py::return_value_policy::take_ownership)     
        .def("getTensorsSize", &NNetPacket::getTensorsSize, py::return_value_policy::copy)     
        .def("getOutputLayersInfo", &NNetPacket::getOutputLayersInfo, py::return_value_policy::copy)
        .def("getInputLayersInfo", &NNetPacket::getInputLayersInfo, py::return_value_policy::copy)
        .def("getDetectedObjects", &NNetPacket::getDetectedObjects)  
        ;
    
    py::class_<TensorInfo> tensorInfo(m, "TensorInfo");
    tensorInfo.def("get_dimension", &TensorInfo::get_dimension);
    tensorInfo.def_readonly("name", &TensorInfo::name);
    tensorInfo.def_readonly("order", &TensorInfo::order);
    tensorInfo.def_readonly("dimensions", &TensorInfo::dimensions);
    tensorInfo.def_readonly("strides", &TensorInfo::strides);
    tensorInfo.def_readonly("data_type", &TensorInfo::data_type);
    tensorInfo.def_readonly("offset", &TensorInfo::offset);
    tensorInfo.def_readonly("element_size", &TensorInfo::element_size);
    tensorInfo.def_readonly("index", &TensorInfo::index);
    tensorInfo.def("get_dict", [](TensorInfo &self) {
        py::dict d;
        d["name"] = self.name;
        d["order"] = self.order;
        d["dimensions"] = self.dimensions;
        d["strides"] = self.strides;
        d["data_type"] = self.data_type;
        d["offset"] = self.offset;
        d["element_size"] = self.element_size;
        d["index"] = self.index;
        return d;
    });
    tensorInfo.def("__str__", [](const TensorInfo& v) {
        std::stringstream stream;
        stream << v;
        return stream.str(); 
    });
    tensorInfo.def("__repr__", [](const TensorInfo& v) {
        std::stringstream stream;
        stream << v;
        return stream.str(); 
    });

    py::class_<TensorInfo::DataType>(tensorInfo, "DataType")
        .def("__str__", [](const TensorInfo::DataType& v) {
            return TensorInfo::type_to_string.at(v);
        })
        .def("__repr__", [](const TensorInfo::DataType& v) {
            return TensorInfo::type_to_string.at(v);
        })
        ;

    py::enum_<TensorInfo::Dimension>(tensorInfo, "Dimension")
        .value("W", TensorInfo::Dimension::W)
        .value("H", TensorInfo::Dimension::H)
        .value("C ", TensorInfo::Dimension::C)
        .value("N", TensorInfo::Dimension::N)
        .value("B", TensorInfo::Dimension::B)
        .value("WIDTH", TensorInfo::Dimension::WIDTH)
        .value("HEIGHT", TensorInfo::Dimension::HEIGHT)
        .value("CHANNEL", TensorInfo::Dimension::CHANNEL)
        .value("NUMBER", TensorInfo::Dimension::NUMBER)
        .value("BATCH", TensorInfo::Dimension::BATCH)
        ;

    py::class_<Detections, std::shared_ptr<Detections>>(m, "Detections")
        .def_readonly("size", &Detections::detection_count)
        .def("__len__",  [](const Detections &det) { return det.detection_count; })
        .def("__getitem__", [](const Detections &det, int idx) { return det.detections[idx]; })
        .def("__iter__", [](std::shared_ptr<Detections> &v)
        {
            const Detection *detection_vec = v->detections;
            return py::make_iterator(&detection_vec[0], &detection_vec[v->detection_count]);
        }, py::keep_alive<0, 1>()) /* Keep list alive while iterator is used */
        ;

    py::class_<Detection>(m, "Detection")
        .def_readonly("label", &Detection::label)
        .def_readonly("confidence", &Detection::confidence)
        .def_readonly("x_min", &Detection::x_min)
        .def_readonly("y_min", &Detection::y_min)
        .def_readonly("x_max", &Detection::x_max)
        .def_readonly("y_max", &Detection::y_max)
        .def_readonly("depth_x", &Detection::depth_x)
        .def_readonly("depth_y", &Detection::depth_y)
        .def_readonly("depth_z", &Detection::depth_z)
        .def("get_dict", [](Detection &self) {
                py::dict d;
                d["label"] = self.label;
                d["confidence"] = self.confidence;
                d["x_min"] = self.x_min;
                d["y_min"] = self.y_min;
                d["x_max"] = self.x_max;
                d["y_max"] = self.y_max;
                d["depth_x"] = self.depth_x;
                d["depth_y"] = self.depth_y;
                d["depth_z"] = self.depth_z;
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

static const std::map<TensorInfo::DataType, std::string> type_to_numpy_format = {
    {TensorInfo::DataType::_fp16,     pybind11::format_descriptor<float16>::format()},
    {TensorInfo::DataType::_u8f,      pybind11::format_descriptor<std::uint8_t>::format()},
    {TensorInfo::DataType::_int,      pybind11::format_descriptor<std::int32_t>::format()},
    {TensorInfo::DataType::_fp32,     pybind11::format_descriptor<float>::format()},
    {TensorInfo::DataType::_i8,       pybind11::format_descriptor<std::int8_t>::format()},
};

static std::string type_to_npy_format_descriptor(const TensorInfo::DataType& type)
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
    assert(!ti.dimensions.empty());
    py::array* result = nullptr;

    ssize_t              ndim    = ti.dimensions.size();
    ssize_t              element_size = TensorInfo::c_type_size.at(ti.data_type);
    std::string          numpy_format_descriptor = type_to_npy_format_descriptor(ti.data_type);
    std::vector<ssize_t> shape;
    std::vector<ssize_t> strides;
    shape.reserve(ndim);
    strides.reserve(ndim);

    for (int i = 0; i < ndim; ++i)
    {
        shape.push_back(ti.dimensions[i]);
        strides.push_back(ti.strides[i]);
    }

    try {

        result = new py::array(py::buffer_info(
                    static_cast<void*>(&data[ti.offset]),                             /* data as contiguous array  */
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
    if(_NN_config[index].contains("output_format"))
    {
        if(_NN_config[index]["output_format"] != std::string("raw"))
        {
            throw std::runtime_error("getTensor should be used only when [\"NN_config\"][\"output_format\"] is set to raw! https://docs.luxonis.com/api/#creating-blob-configuration-file");
        }
    }
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
        std::string name = getTensorName(i);
        // outputs[name.c_str()] = getTensor(i);
        outputs.insert({name,getTensor(i)});
    }
    
    return outputs;
}   

