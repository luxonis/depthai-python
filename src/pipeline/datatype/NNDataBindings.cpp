#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/NNData.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_nndata(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawNNData, RawBuffer, std::shared_ptr<RawNNData>> rawNnData(m, "RawNNData", DOC(dai, RawNNData));
    py::class_<TensorInfo> tensorInfo(m, "TensorInfo", DOC(dai, TensorInfo));
    py::enum_<TensorInfo::DataType>tensorInfoDataType(tensorInfo, "DataType");
    py::enum_<TensorInfo::StorageOrder>tensorInfoStorageOrder(tensorInfo, "StorageOrder");
    py::class_<NNData, Buffer, std::shared_ptr<NNData>> nnData(m, "NNData", DOC(dai, NNData));


    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Metadata / raw

    rawNnData
        .def(py::init<>())
        .def_readwrite("tensors", &RawNNData::tensors)
        .def_readwrite("batchSize", &RawNNData::batchSize)
        .def_property("ts",
            [](const RawNNData& o){
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0;
                return ts;
            },
            [](RawNNData& o, double ts){
                o.ts.sec = ts;
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;
            }
        )
        .def_property("tsDevice",
            [](const RawNNData& o){
                double ts = o.tsDevice.sec + o.tsDevice.nsec / 1000000000.0;
                return ts;
            },
            [](RawNNData& o, double ts){
                o.tsDevice.sec = ts;
                o.tsDevice.nsec = (ts - o.tsDevice.sec) * 1000000000.0;
            }
        )
        .def_readwrite("sequenceNum", &RawNNData::sequenceNum)
        ;

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

    tensorInfoDataType
        .value("FP16", TensorInfo::DataType::FP16)
        .value("U8F", TensorInfo::DataType::U8F)
        .value("INT", TensorInfo::DataType::INT)
        .value("FP32", TensorInfo::DataType::FP32)
        .value("I8", TensorInfo::DataType::I8)
        ;

    tensorInfoStorageOrder
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


    // Message

    nnData
        .def(py::init<>(), DOC(dai, NNData, NNData))
        // setters
        .def("setLayer", [](NNData& obj, const std::string& name, py::array_t<std::uint8_t, py::array::c_style | py::array::forcecast> data){
            std::vector<std::uint8_t> vec(data.data(), data.data() + data.size());
            obj.setLayer(name, std::move(vec));
        }, py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer))
        .def("setLayer", static_cast<NNData&(NNData::*)(const std::string&, const std::vector<int>&)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 2))
        .def("setLayer", static_cast<NNData&(NNData::*)(const std::string&, std::vector<float>)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 3))
        .def("setLayer", static_cast<NNData&(NNData::*)(const std::string&, std::vector<double>)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 4))
        .def("getLayer", &NNData::getLayer, py::arg("name"), py::arg("tensor"), DOC(dai, NNData, getLayer))
        .def("hasLayer", &NNData::hasLayer, py::arg("name"), DOC(dai, NNData, hasLayer))
        .def("getAllLayerNames", &NNData::getAllLayerNames, DOC(dai, NNData, getAllLayerNames))
        .def("getAllLayers", &NNData::getAllLayers, DOC(dai, NNData, getAllLayers))
        .def("getLayerDatatype", &NNData::getLayerDatatype, py::arg("name"), py::arg("datatype"), DOC(dai, NNData, getLayerDatatype))
        .def("getLayerUInt8", &NNData::getLayerUInt8, py::arg("name"), DOC(dai, NNData, getLayerUInt8))
        .def("getLayerFp16", &NNData::getLayerFp16, py::arg("name"), DOC(dai, NNData, getLayerFp16))
        .def("getLayerInt32", &NNData::getLayerInt32, py::arg("name"), DOC(dai, NNData, getLayerInt32))
        .def("getFirstLayerUInt8", &NNData::getFirstLayerUInt8, DOC(dai, NNData, getFirstLayerUInt8))
        .def("getFirstLayerFp16", &NNData::getFirstLayerFp16, DOC(dai, NNData, getFirstLayerFp16))
        .def("getFirstLayerInt32", &NNData::getFirstLayerInt32, DOC(dai, NNData, getFirstLayerInt32))
        .def("getTimestamp", &NNData::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &NNData::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &NNData::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setTimestamp", &NNData::setTimestamp, DOC(dai, NNData, setTimestamp))
        .def("setTimestampDevice", &NNData::setTimestampDevice, DOC(dai, NNData, setTimestampDevice))
        .def("setSequenceNum", &NNData::setSequenceNum, DOC(dai, NNData, setSequenceNum))
        ;


}
