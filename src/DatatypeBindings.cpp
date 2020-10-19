#include "DatatypeBindings.hpp"

#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ADatatype.hpp"
#include "depthai/pipeline/datatype/Buffer.hpp"
#include "depthai/pipeline/datatype/ImgFrame.hpp"
#include "depthai/pipeline/datatype/NNData.hpp"

// depthai-shared
#include "depthai-shared/datatype/RawBuffer.hpp"
#include "depthai-shared/datatype/RawImgFrame.hpp"
#include "depthai-shared/datatype/RawNNData.hpp"


//pybind
//#include <pybind11/stl_bind.h>


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


    // Bind Raw datatypes

    //py::bind_vector<std::vector<std::uint8_t>>(m, "VectorByte");
    py::class_<RawBuffer, std::shared_ptr<RawBuffer>>(m, "RawBuffer")
        .def(py::init<>())
        .def_readwrite("data", &RawBuffer::data)
        ;



    // Bind ImgFrame
    py::class_<RawImgFrame, RawBuffer, std::shared_ptr<RawImgFrame>> rawImgFrame(m, "RawImgFrame");
    rawImgFrame
        .def_readwrite("fb", &RawImgFrame::fb)
        .def_readwrite("category", &RawImgFrame::category)
        .def_readwrite("instanceNum", &RawImgFrame::instanceNum)
        .def_readwrite("sequenceNum", &RawImgFrame::sequenceNum)
        .def_property("ts",
            [](const RawImgFrame& o){ 
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0; 
                return ts; 
            },
            [](RawImgFrame& o, double ts){ 
                o.ts.sec = ts; 
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;   
            }  
        )
        ;


    py::enum_<RawImgFrame::Type>(rawImgFrame, "Type")
        .value("YUV422i", RawImgFrame::Type::YUV422i)
        .value("YUV444p", RawImgFrame::Type::YUV444p)
        .value("YUV420p", RawImgFrame::Type::YUV420p)
        .value("YUV422p", RawImgFrame::Type::YUV422p)
        .value("YUV400p", RawImgFrame::Type::YUV400p)
        .value("RGBA8888", RawImgFrame::Type::RGBA8888)
        .value("RGB161616", RawImgFrame::Type::RGB161616)
        .value("RGB888", RawImgFrame::Type::RGB888)
        .value("LUT2", RawImgFrame::Type::LUT2)
        .value("LUT4", RawImgFrame::Type::LUT4)
        .value("LUT16", RawImgFrame::Type::LUT16)
        .value("RAW16", RawImgFrame::Type::RAW16)
        .value("RAW14", RawImgFrame::Type::RAW14)
        .value("RAW12", RawImgFrame::Type::RAW12)
        .value("RAW10", RawImgFrame::Type::RAW10)
        .value("RAW8", RawImgFrame::Type::RAW8)
        .value("PACK10", RawImgFrame::Type::PACK10)
        .value("PACK12", RawImgFrame::Type::PACK12)
        .value("YUV444i", RawImgFrame::Type::YUV444i)
        .value("NV12", RawImgFrame::Type::NV12)
        .value("NV21", RawImgFrame::Type::NV21)
        .value("BITSTREAM", RawImgFrame::Type::BITSTREAM)
        .value("HDR", RawImgFrame::Type::HDR)
        .value("NONE", RawImgFrame::Type::NONE)
        ;

    py::class_<RawImgFrame::Specs>(rawImgFrame, "Specs")
        .def_readwrite("type", &RawImgFrame::Specs::type)
        .def_readwrite("width", &RawImgFrame::Specs::width)
        .def_readwrite("height", &RawImgFrame::Specs::height)
        .def_readwrite("stride", &RawImgFrame::Specs::stride)
        .def_readwrite("bytesPP", &RawImgFrame::Specs::bytesPP)
        .def_readwrite("p1Offset", &RawImgFrame::Specs::p1Offset)
        .def_readwrite("p2Offset", &RawImgFrame::Specs::p2Offset)
        .def_readwrite("p3Offset", &RawImgFrame::Specs::p3Offset)
        ;   



    // NNData
    py::class_<RawNNData, RawBuffer, std::shared_ptr<RawNNData>> rawNnData(m, "RawNNData");
    rawNnData
        .def(py::init<>())
        .def_readwrite("tensors", &RawNNData::tensors)
        .def_readwrite("batchSize", &RawNNData::batchSize)
        ;

    py::class_<TensorInfo> tensorInfo(m, "TensorInfo");
    tensorInfo
        .def(py::init<>())
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
        ;


    // Bind non-raw 'helper' datatypes
    py::class_<ADatatype, std::shared_ptr<ADatatype>>(m, "ADatatype")
        .def("getRaw", &ADatatype::getRaw);

    py::class_<Buffer, ADatatype, std::shared_ptr<Buffer>>(m, "Buffer")
        .def(py::init<>())
        .def("getData", &Buffer::getData)
        .def("setData", &Buffer::setData)
        ;

    // Bind ImgFrame
    py::class_<ImgFrame, Buffer, std::shared_ptr<ImgFrame>>(m, "ImgFrame")
        .def(py::init<>())
        // getters
        .def("getTimestamp", &ImgFrame::getTimestamp)
        .def("getInstanceNum", &ImgFrame::getInstanceNum)
        .def("getCategory", &ImgFrame::getCategory)
        .def("getSequenceNum", &ImgFrame::getSequenceNum)
        .def("getWidth", &ImgFrame::getWidth)
        .def("getHeight", &ImgFrame::getHeight)
        .def("getType", &ImgFrame::getType)

        // setters
        .def("setTimestamp", &ImgFrame::setTimestamp)
        .def("setInstanceNum", &ImgFrame::setInstanceNum)
        .def("setCategory", &ImgFrame::setCategory)
        .def("setSequenceNum", &ImgFrame::setSequenceNum)
        .def("setWidth", &ImgFrame::setWidth)
        .def("setHeight", &ImgFrame::setHeight)
        .def("setType", &ImgFrame::setType)
        ;

    // Bind NNData
    py::class_<NNData, Buffer, std::shared_ptr<NNData>>(m, "NNData")
        .def(py::init<>())
        // setters
        .def("setLayer", py::overload_cast<const std::string&, std::vector<std::uint8_t>>(&NNData::setLayer))
        .def("setLayer", py::overload_cast<const std::string&, const std::vector<int>&>(&NNData::setLayer))
        .def("setLayer", py::overload_cast<const std::string&, std::vector<float>>(&NNData::setLayer))
        .def("setLayer", py::overload_cast<const std::string&, std::vector<double>>(&NNData::setLayer))
        .def("getLayer", &NNData::getLayer)
        .def("hasLayer", &NNData::hasLayer)
        .def("getAllLayerNames", &NNData::getAllLayerNames)
        .def("getAllLayers", &NNData::getAllLayers)
        .def("getLayerDatatype", &NNData::getLayerDatatype)
        .def("getLayerUInt8", &NNData::getLayerUInt8)
        .def("getLayerFp16", &NNData::getLayerFp16)
        .def("getFirstLayerUInt8", &NNData::getFirstLayerUInt8)
        .def("getFirstLayerFp16", &NNData::getFirstLayerFp16)
        ;
    




}