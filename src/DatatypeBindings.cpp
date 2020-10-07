#include "DatatypeBindings.hpp"

#include <unordered_map>
#include <memory>

// depthai-shared
#include "depthai-shared/datatype/RawBuffer.hpp"
#include "depthai-shared/datatype/ImgFrame.hpp"
#include "depthai-shared/datatype/NNTensor.hpp"


//pybind
#include <pybind11/stl_bind.h>


void DatatypeBindings::bind(pybind11::module& m){


    using namespace dai;

    // Bind RawBuffer

    /* No copy, numpy way
    static std::unordered_map<void*, std::shared_ptr<RawBuffer>> pointers;
    py::class_<RawBuffer, std::shared_ptr<RawBuffer>>(m, "RawBuffer")
        .def(py::init<>())
        .def_property_readonly("data", 
            [](std::shared_ptr<RawBuffer>& p){

                auto size = p->data.size();
                auto data = p->data.data();

                // store this usage
                pointers[p.get()] = p;
                // Create a deleter
                auto capsule = py::capsule(p.get(), [](void *m) { pointers.erase(m); });
                
                // return numpy array which points to this data
                return py::array(size, data, capsule);
            }
        )
        ;
    */

    // No copy, opaque std::vector<std::uint8_t> way
    py::bind_vector<std::vector<std::uint8_t>>(m, "VectorByte");
    py::class_<RawBuffer, std::shared_ptr<RawBuffer>>(m, "RawBuffer")
        .def(py::init<>())
        .def_readwrite("data", &RawBuffer::data)
        ;



    // Bind ImgFrame
    py::class_<ImgFrame, RawBuffer, std::shared_ptr<ImgFrame>> imgFrame(m, "ImgFrame");
    imgFrame
        .def_readwrite("fb", &ImgFrame::fb)
        .def_readwrite("category", &ImgFrame::category)
        .def_readwrite("instanceNum", &ImgFrame::instanceNum)
        .def_readwrite("sequenceNum", &ImgFrame::sequenceNum)
        .def_property("ts",
            [](const ImgFrame& o){ 
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0; 
                return ts; 
            },
            [](ImgFrame& o, double ts){ 
                o.ts.sec = ts; 
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;   
            }  
        )
        ;


    py::enum_<ImgFrame::Type>(imgFrame, "Type")
        .value("YUV422i", ImgFrame::Type::YUV422i)
        .value("YUV444p", ImgFrame::Type::YUV444p)
        .value("YUV420p", ImgFrame::Type::YUV420p)
        .value("YUV422p", ImgFrame::Type::YUV422p)
        .value("YUV400p", ImgFrame::Type::YUV400p)
        .value("RGBA8888", ImgFrame::Type::RGBA8888)
        .value("RGB161616", ImgFrame::Type::RGB161616)
        .value("RGB888", ImgFrame::Type::RGB888)
        .value("LUT2", ImgFrame::Type::LUT2)
        .value("LUT4", ImgFrame::Type::LUT4)
        .value("LUT16", ImgFrame::Type::LUT16)
        .value("RAW16", ImgFrame::Type::RAW16)
        .value("RAW14", ImgFrame::Type::RAW14)
        .value("RAW12", ImgFrame::Type::RAW12)
        .value("RAW10", ImgFrame::Type::RAW10)
        .value("RAW8", ImgFrame::Type::RAW8)
        .value("PACK10", ImgFrame::Type::PACK10)
        .value("PACK12", ImgFrame::Type::PACK12)
        .value("YUV444i", ImgFrame::Type::YUV444i)
        .value("NV12", ImgFrame::Type::NV12)
        .value("NV21", ImgFrame::Type::NV21)
        .value("BITSTREAM", ImgFrame::Type::BITSTREAM)
        .value("HDR", ImgFrame::Type::HDR)
        .value("NONE", ImgFrame::Type::NONE)
        .export_values()
        ;

    py::class_<ImgFrame::Specs>(imgFrame, "Specs")
        .def_readwrite("type", &ImgFrame::Specs::type)
        .def_readwrite("width", &ImgFrame::Specs::width)
        .def_readwrite("height", &ImgFrame::Specs::height)
        .def_readwrite("stride", &ImgFrame::Specs::stride)
        .def_readwrite("bytesPP", &ImgFrame::Specs::bytesPP)
        .def_readwrite("p1Offset", &ImgFrame::Specs::p1Offset)
        .def_readwrite("p2Offset", &ImgFrame::Specs::p2Offset)
        .def_readwrite("p3Offset", &ImgFrame::Specs::p3Offset)
        ;   



    // NNTensor
    py::class_<NNTensor, RawBuffer, std::shared_ptr<NNTensor>> nnTensor(m, "NNTensor");
    nnTensor
        .def_readwrite("tensors", &NNTensor::tensors)
        .def_readwrite("batchSize", &NNTensor::batchSize)
        ;

    py::class_<TensorInfo> tensorInfo(m, "TensorInfo");
    tensorInfo
        .def_readwrite("order", &TensorInfo::order)
        .def_readwrite("dataType", &TensorInfo::dataType)
        .def_readwrite("numDimensions", &TensorInfo::numDimensions)
        .def_readwrite("dims", &TensorInfo::dims)
        .def_readwrite("strides", &TensorInfo::strides)
        .def_readwrite("name", &TensorInfo::name)
        .def_readwrite("offset", &TensorInfo::offset)
        ;

    py::enum_<TensorInfo::DataType>(tensorInfo, "DataType")
        .value("FP16", TensorInfo::DataType::FP16)
        .value("U8F", TensorInfo::DataType::U8F)
        .value("INT", TensorInfo::DataType::INT)
        .value("FP32", TensorInfo::DataType::FP32)
        .value("I8", TensorInfo::DataType::I8)
        .export_values()
        ;
        
    py::enum_<TensorInfo::StorageOrder>(tensorInfo, "StorageOrder")
        .value("NHWC", TensorInfo::StorageOrder::NHWC)
        .value("NHCW", TensorInfo::StorageOrder::NHCW)
        .value("NCHW", TensorInfo::StorageOrder::NCHW)
        .value("HWC", TensorInfo::StorageOrder::HWC)
        .value("CHW", TensorInfo::StorageOrder::CHW)
        .value("WHC", TensorInfo::StorageOrder::WHC)
        .value("HCW", TensorInfo::StorageOrder::HCW)
        .value("WCH", TensorInfo::StorageOrder::WCH)
        .value("CWH", TensorInfo::StorageOrder::CWH)
        .value("NC", TensorInfo::StorageOrder::NC)
        .value("CN", TensorInfo::StorageOrder::CN)
        .value("C", TensorInfo::StorageOrder::C)
        .value("H", TensorInfo::StorageOrder::H)
        .value("W", TensorInfo::StorageOrder::W)
        .export_values()
        ;


}