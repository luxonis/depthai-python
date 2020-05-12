#include "host_data_packet_bindings.hpp"

//std
#include <iostream>

// Binding for HostDataPacket
namespace py = pybind11;


void init_binding_host_data_packet(pybind11::module& m){
    
    // for PACKET in data_packets:
    py::class_<HostDataPacket, std::shared_ptr<HostDataPacket>>(m, "DataPacket")
        .def_readonly("stream_name", &HostDataPacket::stream_name)
        .def("size", &HostDataPacket::size)
        .def("getData", static_cast<py::array* (HostDataPacket::*)()>(&PyHostDataPacket::getPythonNumpyArray), py::return_value_policy::take_ownership)
        .def("getDataAsStr", &HostDataPacket::getDataAsString, py::return_value_policy::take_ownership)
        .def("getMetadata", &HostDataPacket::getMetadata)
        ;


    // FrameMetadata struct binding
    py::class_<FrameMetadata>(m, "FrameMetadata")
        .def(py::init<>())
        .def("getTimestamp", &FrameMetadata::getTimestamp)
        .def("getFrameType", &FrameMetadata::getFrameType)
        .def("getFrameWidth", &FrameMetadata::getFrameWidth)
        .def("getFrameHeight", &FrameMetadata::getFrameHeight)
        .def("getFrameBytesPP", &FrameMetadata::getFrameBytesPP)
        .def("getStride", &FrameMetadata::getStride)
        .def("getCategory", &FrameMetadata::getCategory)
        .def("getInstanceNum", &FrameMetadata::getInstanceNum)
        .def("getSequenceNum", &FrameMetadata::getSequenceNum)
        ;


}


py::array* PyHostDataPacket::getPythonNumpyArray()
{
    assert(!dimensions.empty());
    assert(!data.empty());

    Timer t;

    py::array* result = nullptr;

    py::gil_scoped_acquire acquire;

    ssize_t              ndim    = dimensions.size();
    std::vector<ssize_t> shape;
    std::vector<ssize_t> strides;

    auto size_div = std::accumulate(std::begin(dimensions), std::end(dimensions), 1, std::multiplies<int>());
    for (int i = 0; i < dimensions.size(); ++i)
    {
        shape.push_back(dimensions[i]);

        size_div /= dimensions[i];
        strides.push_back(size_div);
    }

    try {
        // TODO: unite code in if blocks
        if (elem_size == 1)
        {
            result = new py::array(py::buffer_info(
                        data.data(),                             /* data as contiguous array  */
                        sizeof(unsigned char),                   /* size of one scalar        */
                        py::format_descriptor<unsigned char>::format(), /* data type          */
                        ndim,                                    /* number of dimensions      */
                        shape,                                   /* shape of the matrix       */
                        strides                                  /* strides for each axis     */
                    ));
        }
        else if (elem_size == 2)
        {
            result = new py::array(py::buffer_info(
                        data.data(),                             /* data as contiguous array  */
                        sizeof(std::uint16_t),                          /* size of one scalar        */
                        py::format_descriptor<std::uint16_t>::format(), /* data type          */
                        2, //ndim,                                    /* number of dimensions      */
                        {720, 1280}, //shape,                                   /* shape of the matrix       */
                        {1280*2, 1*2} //strides                                  /* strides for each axis     */
                    ));
        }
        else
        {
            assert(false);
        }
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        result = new py::array(py::dtype("f"), {1}, {});
    }

    //py::gil_scoped_release release; // REUIRED ???

    // std::cout << "===> c++ getPythonNumpyArray " << t.ellapsed_us() << " us\n";

    return result;
}
