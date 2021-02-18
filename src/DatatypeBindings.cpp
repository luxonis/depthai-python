#include "DatatypeBindings.hpp"

#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ADatatype.hpp"
#include "depthai/pipeline/datatype/Buffer.hpp"
#include "depthai/pipeline/datatype/ImgFrame.hpp"
#include "depthai/pipeline/datatype/NNData.hpp"
#include "depthai/pipeline/datatype/ImgDetections.hpp"
#include "depthai/pipeline/datatype/ImageManipConfig.hpp"
#include "depthai/pipeline/datatype/CameraControl.hpp"
#include "depthai/pipeline/datatype/SystemInformation.hpp"

// depthai-shared
#include "depthai-shared/datatype/RawBuffer.hpp"
#include "depthai-shared/datatype/RawImgFrame.hpp"
#include "depthai-shared/datatype/RawNNData.hpp"
#include "depthai-shared/datatype/RawImgDetections.hpp"
#include "depthai-shared/datatype/RawImageManipConfig.hpp"
#include "depthai-shared/datatype/RawCameraControl.hpp"
#include "depthai-shared/datatype/RawSystemInformation.hpp"


//pybind
#include <pybind11/chrono.h>


void DatatypeBindings::bind(pybind11::module& m){


    using namespace dai;

    // Bind Raw datatypes
    py::class_<RawBuffer, std::shared_ptr<RawBuffer>>(m, "RawBuffer")
        .def(py::init<>())
        .def_property("data", [](py::object &obj){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            return py::array_t<uint8_t>(a.data.size(), a.data.data(), obj);
        }, [](py::object &obj, py::array_t<std::uint8_t, py::array::c_style> array){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            a.data = {array.data(), array.data() + array.size()};
        })
        ;


    // Bind RawImgFrame
    py::class_<RawImgFrame, RawBuffer, std::shared_ptr<RawImgFrame>> rawImgFrame(m, "RawImgFrame");
    rawImgFrame
        .def(py::init<>())
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
        .value("RGB888p", RawImgFrame::Type::RGB888p)
        .value("BGR888p", RawImgFrame::Type::BGR888p)
        .value("RGB888i", RawImgFrame::Type::RGB888i)
        .value("BGR888i", RawImgFrame::Type::BGR888i)
        .value("RGBF16F16F16p", RawImgFrame::Type::RGBF16F16F16p)
        .value("BGRF16F16F16p", RawImgFrame::Type::BGRF16F16F16p)
        .value("RGBF16F16F16i", RawImgFrame::Type::RGBF16F16F16i)
        .value("BGRF16F16F16i", RawImgFrame::Type::BGRF16F16F16i)
        .value("GRAY8", RawImgFrame::Type::GRAY8)
        .value("GRAYF16", RawImgFrame::Type::GRAYF16)
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

    py::class_<ImgDetection>(m, "ImgDetection")
        .def(py::init<>())
        .def_readwrite("label", &ImgDetection::label)
        .def_readwrite("confidence", &ImgDetection::confidence)
        .def_readwrite("xmin", &ImgDetection::xmin)
        .def_readwrite("ymin", &ImgDetection::ymin)
        .def_readwrite("xmax", &ImgDetection::xmax)
        .def_readwrite("ymax", &ImgDetection::ymax)
        .def_readwrite("xdepth", &ImgDetection::xdepth)
        .def_readwrite("ydepth", &ImgDetection::ydepth)
        .def_readwrite("zdepth", &ImgDetection::zdepth)
        ;

    py::class_<RawImgDetections, RawBuffer, std::shared_ptr<RawImgDetections>> RawImgDetections(m, "RawImgDetections");
    RawImgDetections
        .def(py::init<>())
        .def_readwrite("detections", &RawImgDetections::detections)
        ;

    
    // Bind RawImageManipConfig
    py::class_<RawImageManipConfig, RawBuffer, std::shared_ptr<RawImageManipConfig>> rawImageManipConfig(m, "RawImageManipConfig");
    rawImageManipConfig
        .def(py::init<>())
        .def_readwrite("enableFormat", &RawImageManipConfig::enableFormat)
        .def_readwrite("enableResize", &RawImageManipConfig::enableResize)
        .def_readwrite("enableCrop", &RawImageManipConfig::enableCrop)
        .def_readwrite("cropConfig", &RawImageManipConfig::cropConfig)
        .def_readwrite("resizeConfig", &RawImageManipConfig::resizeConfig)
        .def_readwrite("formatConfig", &RawImageManipConfig::formatConfig)
        ;

    py::class_<RawImageManipConfig::CropRect>(rawImageManipConfig, "CropRect")
        .def(py::init<>())
        .def_readwrite("xmin", &RawImageManipConfig::CropRect::xmin)
        .def_readwrite("ymin", &RawImageManipConfig::CropRect::ymin)
        .def_readwrite("xmax", &RawImageManipConfig::CropRect::xmax)
        .def_readwrite("ymax", &RawImageManipConfig::CropRect::ymax)
        ;

    py::class_<RawImageManipConfig::CropConfig>(rawImageManipConfig, "CropConfig")
        .def(py::init<>())
        .def_readwrite("cropRect", &RawImageManipConfig::CropConfig::cropRect)
        .def_readwrite("cropRotatedRect", &RawImageManipConfig::CropConfig::cropRotatedRect)
        .def_readwrite("enableCenterCropRectangle", &RawImageManipConfig::CropConfig::enableCenterCropRectangle)
        .def_readwrite("cropRatio", &RawImageManipConfig::CropConfig::cropRatio)
        .def_readwrite("widthHeightAspectRatio", &RawImageManipConfig::CropConfig::widthHeightAspectRatio)
        .def_readwrite("enableRotatedRect", &RawImageManipConfig::CropConfig::enableRotatedRect)
        .def_readwrite("normalizedCoords", &RawImageManipConfig::CropConfig::normalizedCoords)
        ;

    py::class_<RawImageManipConfig::ResizeConfig>(rawImageManipConfig, "ResizeConfig")
        .def(py::init<>())
        .def_readwrite("width", &RawImageManipConfig::ResizeConfig::width)
        .def_readwrite("height", &RawImageManipConfig::ResizeConfig::height)
        .def_readwrite("lockAspectRatioFill", &RawImageManipConfig::ResizeConfig::lockAspectRatioFill)
        .def_readwrite("bgRed", &RawImageManipConfig::ResizeConfig::bgRed)
        .def_readwrite("bgGreen", &RawImageManipConfig::ResizeConfig::bgGreen)
        .def_readwrite("bgBlue", &RawImageManipConfig::ResizeConfig::bgBlue)
        .def_readwrite("warpFourPoints", &RawImageManipConfig::ResizeConfig::warpFourPoints)
        .def_readwrite("normalizedCoords", &RawImageManipConfig::ResizeConfig::normalizedCoords)
        .def_readwrite("enableWarp4pt", &RawImageManipConfig::ResizeConfig::enableWarp4pt)
        .def_readwrite("warpMatrix3x3", &RawImageManipConfig::ResizeConfig::warpMatrix3x3)
        .def_readwrite("enableWarpMatrix", &RawImageManipConfig::ResizeConfig::enableWarpMatrix)
        .def_readwrite("warpBorderReplicate", &RawImageManipConfig::ResizeConfig::warpBorderReplicate)
        .def_readwrite("rotationAngleDeg", &RawImageManipConfig::ResizeConfig::rotationAngleDeg)
        .def_readwrite("enableRotation", &RawImageManipConfig::ResizeConfig::enableRotation)
        ;

    py::class_<RawImageManipConfig::FormatConfig>(rawImageManipConfig, "FormatConfig")
        .def(py::init<>())
        .def_readwrite("type", &RawImageManipConfig::FormatConfig::type)
        .def_readwrite("flipHorizontal", &RawImageManipConfig::FormatConfig::flipHorizontal)
        ;


    // Bind RawCameraControl
    py::class_<RawCameraControl, RawBuffer, std::shared_ptr<RawCameraControl>> rawCameraControl(m, "RawCameraControl");
    rawCameraControl
        .def_readwrite("cmdMask", &RawCameraControl::cmdMask)
        .def_readwrite("autoFocusMode", &RawCameraControl::autoFocusMode)
        .def_readwrite("lensPosition", &RawCameraControl::lensPosition)
        // TODO add more raw types here, not directly used
        ;

    // RawCameraControl enum bindings
    // The enum fields will also be exposed in 'CameraControl', store them for later
    std::vector<const char *> camCtrlAttr;
    camCtrlAttr.push_back("AutoFocusMode");
    py::enum_<RawCameraControl::AutoFocusMode>(rawCameraControl, "AutoFocusMode")
        .value("OFF", RawCameraControl::AutoFocusMode::OFF)
        .value("AUTO", RawCameraControl::AutoFocusMode::AUTO)
        .value("MACRO", RawCameraControl::AutoFocusMode::MACRO)
        .value("CONTINUOUS_VIDEO", RawCameraControl::AutoFocusMode::CONTINUOUS_VIDEO)
        .value("CONTINUOUS_PICTURE", RawCameraControl::AutoFocusMode::CONTINUOUS_PICTURE)
        .value("EDOF", RawCameraControl::AutoFocusMode::EDOF)
    ;

    camCtrlAttr.push_back("AutoWhiteBalanceMode");
    py::enum_<RawCameraControl::AutoWhiteBalanceMode>(rawCameraControl, "AutoWhiteBalanceMode")
        .value("OFF", RawCameraControl::AutoWhiteBalanceMode::OFF)
        .value("AUTO", RawCameraControl::AutoWhiteBalanceMode::AUTO)
        .value("INCANDESCENT", RawCameraControl::AutoWhiteBalanceMode::INCANDESCENT)
        .value("FLUORESCENT", RawCameraControl::AutoWhiteBalanceMode::FLUORESCENT)
        .value("WARM_FLUORESCENT", RawCameraControl::AutoWhiteBalanceMode::WARM_FLUORESCENT)
        .value("DAYLIGHT", RawCameraControl::AutoWhiteBalanceMode::DAYLIGHT)
        .value("CLOUDY_DAYLIGHT", RawCameraControl::AutoWhiteBalanceMode::CLOUDY_DAYLIGHT)
        .value("TWILIGHT", RawCameraControl::AutoWhiteBalanceMode::TWILIGHT)
        .value("SHADE", RawCameraControl::AutoWhiteBalanceMode::SHADE)
    ;

    camCtrlAttr.push_back("SceneMode");
    py::enum_<RawCameraControl::SceneMode>(rawCameraControl, "SceneMode")
        .value("UNSUPPORTED", RawCameraControl::SceneMode::UNSUPPORTED)
        .value("FACE_PRIORITY", RawCameraControl::SceneMode::FACE_PRIORITY)
        .value("ACTION", RawCameraControl::SceneMode::ACTION)
        .value("PORTRAIT", RawCameraControl::SceneMode::PORTRAIT)
        .value("LANDSCAPE", RawCameraControl::SceneMode::LANDSCAPE)
        .value("NIGHT", RawCameraControl::SceneMode::NIGHT)
        .value("NIGHT_PORTRAIT", RawCameraControl::SceneMode::NIGHT_PORTRAIT)
        .value("THEATRE", RawCameraControl::SceneMode::THEATRE)
        .value("BEACH", RawCameraControl::SceneMode::BEACH)
        .value("SNOW", RawCameraControl::SceneMode::SNOW)
        .value("SUNSET", RawCameraControl::SceneMode::SUNSET)
        .value("STEADYPHOTO", RawCameraControl::SceneMode::STEADYPHOTO)
        .value("FIREWORKS", RawCameraControl::SceneMode::FIREWORKS)
        .value("SPORTS", RawCameraControl::SceneMode::SPORTS)
        .value("PARTY", RawCameraControl::SceneMode::PARTY)
        .value("CANDLELIGHT", RawCameraControl::SceneMode::CANDLELIGHT)
        .value("BARCODE", RawCameraControl::SceneMode::BARCODE)
    ;

    camCtrlAttr.push_back("AntiBandingMode");
    py::enum_<RawCameraControl::AntiBandingMode>(rawCameraControl, "AntiBandingMode")
        .value("OFF", RawCameraControl::AntiBandingMode::OFF)
        .value("MAINS_50_HZ", RawCameraControl::AntiBandingMode::MAINS_50_HZ)
        .value("MAINS_60_HZ", RawCameraControl::AntiBandingMode::MAINS_60_HZ)
        .value("AUTO", RawCameraControl::AntiBandingMode::AUTO)
    ;

    camCtrlAttr.push_back("EffectMode");
    py::enum_<RawCameraControl::EffectMode>(rawCameraControl, "EffectMode")
        .value("OFF", RawCameraControl::EffectMode::OFF)
        .value("MONO", RawCameraControl::EffectMode::MONO)
        .value("NEGATIVE", RawCameraControl::EffectMode::NEGATIVE)
        .value("SOLARIZE", RawCameraControl::EffectMode::SOLARIZE)
        .value("SEPIA", RawCameraControl::EffectMode::SEPIA)
        .value("POSTERIZE", RawCameraControl::EffectMode::POSTERIZE)
        .value("WHITEBOARD", RawCameraControl::EffectMode::WHITEBOARD)
        .value("BLACKBOARD", RawCameraControl::EffectMode::BLACKBOARD)
        .value("AQUA", RawCameraControl::EffectMode::AQUA)
    ;

    // Bind RawSystemInformation
    py::class_<RawSystemInformation, RawBuffer, std::shared_ptr<RawSystemInformation>> rawSystemInformation(m, "RawSystemInformation");
    rawSystemInformation
        .def(py::init<>())
        .def_readwrite("ddrMemoryUsage", &RawSystemInformation::ddrMemoryUsage)
        .def_readwrite("leonCssMemoryUsage", &RawSystemInformation::leonCssMemoryUsage)
        .def_readwrite("leonMssMemoryUsage", &RawSystemInformation::leonMssMemoryUsage)
        .def_readwrite("leonCssCpuUsage", &RawSystemInformation::leonCssCpuUsage)
        .def_readwrite("leonMssCpuUsage", &RawSystemInformation::leonMssCpuUsage)
        .def_readwrite("chipTemperature", &RawSystemInformation::chipTemperature)
        ;


    // Bind non-raw 'helper' datatypes
    py::class_<ADatatype, std::shared_ptr<ADatatype>>(m, "ADatatype")
        .def("getRaw", &ADatatype::getRaw);

    py::class_<Buffer, ADatatype, std::shared_ptr<Buffer>>(m, "Buffer")
        .def(py::init<>())

        // obj is "Python" object, which we used then to bind the numpy arrays lifespan to
        .def("getData", [](py::object &obj){
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            dai::Buffer &a = obj.cast<dai::Buffer&>();
            return py::array_t<uint8_t>(a.getData().size(), a.getData().data(), obj);
        })
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

        /* TODO(themarpe) - Convinience function which constructs array with correct datatype, shape, ...
        .def("getFrame", [](py::object &obj){
            // obj is "Python" object, which we used then to bind the numpy view lifespan to
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            dai::ImgFrame &a = obj.cast<dai::ImgFrame&>();
            
            // shape
            std::vector<std::size_t> shape = {a.getData().size()};

            // TODO
            switch(a.getType()){
                case dai::RawImgFrame::Type::BITSTREAM :
                    //shape = {return py::array_t<uint8_t>(a.getData().size(), a.getData().data(), obj);
                break;

                case dai::RawImgFrame::Type::RGB888 :
                    shape = {a.getWidth(), a.getHeight(), 3};
                    //return py::array_t<uint8_t>(py::array::ShapeContainer(a.getWidth(), a.getHeight(), 3), a.getData().data(), obj);
                break;

            }
            

            return py::array_t<uint8_t>(shape, a.getData().data(), obj);
        })
        */

        // setters
        .def("setTimestamp", &ImgFrame::setTimestamp)
        .def("setInstanceNum", &ImgFrame::setInstanceNum)
        .def("setCategory", &ImgFrame::setCategory)
        .def("setSequenceNum", &ImgFrame::setSequenceNum)
        .def("setWidth", &ImgFrame::setWidth, py::arg("width"), DOC(dai, ImgFrame, setWidth))
        .def("setHeight", &ImgFrame::setHeight)
        .def("setType", &ImgFrame::setType)
        ;

    py::class_<Timestamp>(m, "Timestamp")
        .def(py::init<>())
        .def_readwrite("sec", &Timestamp::sec)
        .def_readwrite("nsec", &Timestamp::nsec)
        ;

    py::class_<Point2f>(m, "Point2f")
        .def(py::init<>())
        .def_readwrite("x", &Point2f::x)
        .def_readwrite("y", &Point2f::y)
        ;

    py::class_<Size2f>(m, "Size2f")
        .def(py::init<>())
        .def_readwrite("width", &Size2f::width)
        .def_readwrite("height", &Size2f::height)
        ;

    py::class_<RotatedRect>(m, "RotatedRect")
        .def(py::init<>())
        .def_readwrite("center", &RotatedRect::center)
        .def_readwrite("size", &RotatedRect::size)
        .def_readwrite("angle", &RotatedRect::angle)
        ;

    // Bind NNData
    py::class_<NNData, Buffer, std::shared_ptr<NNData>>(m, "NNData")
        .def(py::init<>())
        // setters
        .def("setLayer", [](NNData& obj, const std::string& key, py::array_t<std::uint8_t, py::array::c_style | py::array::forcecast> array){
            std::vector<std::uint8_t> vec(array.data(), array.data() + array.size());
            obj.setLayer(key, std::move(vec));
        })
        .def("setLayer", (void(NNData::*)(const std::string&, std::vector<std::uint8_t>))&NNData::setLayer)
        .def("setLayer", (void(NNData::*)(const std::string&, const std::vector<int>&))&NNData::setLayer)
        .def("setLayer", (void(NNData::*)(const std::string&, std::vector<float>))&NNData::setLayer)
        .def("setLayer", (void(NNData::*)(const std::string&, std::vector<double>))&NNData::setLayer)
        .def("getLayer", &NNData::getLayer)
        .def("hasLayer", &NNData::hasLayer)
        .def("getAllLayerNames", &NNData::getAllLayerNames)
        .def("getAllLayers", &NNData::getAllLayers)
        .def("getLayerDatatype", &NNData::getLayerDatatype)
        .def("getLayerUInt8", &NNData::getLayerUInt8, py::arg("name"))
        .def("getLayerFp16", &NNData::getLayerFp16, py::arg("name"))
        .def("getLayerInt32", &NNData::getLayerInt32, py::arg("name"))
        .def("getFirstLayerUInt8", &NNData::getFirstLayerUInt8)
        .def("getFirstLayerFp16", &NNData::getFirstLayerFp16)
        .def("getFirstLayerInt32", &NNData::getFirstLayerInt32)
        ;

    // Bind ImgDetections
    py::class_<ImgDetections, Buffer, std::shared_ptr<ImgDetections>>(m, "ImgDetections")
        .def(py::init<>())
        .def_property("detections", [](ImgDetections& det) { return &det.detections; }, [](ImgDetections& det, std::vector<ImgDetection> val) { det.detections = val; } )
        ;

     // Bind ImageManipConfig
    py::class_<ImageManipConfig, Buffer, std::shared_ptr<ImageManipConfig>>(m, "ImageManipConfig")
        .def(py::init<>())
        // setters
        .def("setCropRect", &ImageManipConfig::setCropRect)
        .def("setCropRotatedRect", &ImageManipConfig::setCropRotatedRect)
        .def("setCenterCrop", &ImageManipConfig::setCenterCrop)
        .def("setWarpTransformFourPoints", &ImageManipConfig::setWarpTransformFourPoints)
        .def("setWarpTransformMatrix3x3", &ImageManipConfig::setWarpTransformMatrix3x3)
        .def("setWarpBorderFillColor", &ImageManipConfig::setWarpBorderFillColor)
        .def("setWarpBorderReplicatePixels", &ImageManipConfig::setWarpBorderReplicatePixels)
        .def("setRotationDegrees", &ImageManipConfig::setRotationDegrees)
        .def("setRotationRadians", &ImageManipConfig::setRotationRadians)
        .def("setResize", &ImageManipConfig::setResize)
        .def("setResizeThumbnail", &ImageManipConfig::setResizeThumbnail)
        .def("setFrameType", &ImageManipConfig::setFrameType)
        .def("setHorizontalFlip", &ImageManipConfig::setHorizontalFlip)
        .def("setReusePreviousImage", &ImageManipConfig::setReusePreviousImage)
        .def("setSkipCurrentImage", &ImageManipConfig::setSkipCurrentImage)

        // getters
        .def("getCropXMin", &ImageManipConfig::getCropXMin)
        .def("getCropYMin", &ImageManipConfig::getCropYMin)
        .def("getCropXMax", &ImageManipConfig::getCropXMax)
        .def("getCropYMax", &ImageManipConfig::getCropYMax)
        .def("getResizeWidth", &ImageManipConfig::getResizeWidth)
        .def("getResizeHeight", &ImageManipConfig::getResizeHeight)
        .def("isResizeThumbnail", &ImageManipConfig::isResizeThumbnail)
        ;

    // Bind CameraControl
    py::class_<CameraControl, Buffer, std::shared_ptr<CameraControl>>(m, "CameraControl")
        .def(py::init<>())
        // setters
        .def("setCaptureStill", &CameraControl::setCaptureStill)
        .def("setStartStreaming", &CameraControl::setStartStreaming)
        .def("setStopStreaming", &CameraControl::setStopStreaming)
        .def("setAutoFocusMode", &CameraControl::setAutoFocusMode)
        .def("setAutoFocusTrigger", &CameraControl::setAutoFocusTrigger)
        .def("setAutoFocusRegion", &CameraControl::setAutoFocusRegion)
        .def("setManualFocus", &CameraControl::setManualFocus)
        .def("setAutoExposureEnable", &CameraControl::setAutoExposureEnable)
        .def("setAutoExposureLock", &CameraControl::setAutoExposureLock)
        .def("setAutoExposureRegion", &CameraControl::setAutoExposureRegion)
        .def("setAutoExposureCompensation", &CameraControl::setAutoExposureCompensation)
        .def("setAntiBandingMode", &CameraControl::setAntiBandingMode)
        .def("setManualExposure", &CameraControl::setManualExposure)
        .def("setAutoWhiteBalanceMode", &CameraControl::setAutoWhiteBalanceMode)
        .def("setAutoWhiteBalanceLock", &CameraControl::setAutoWhiteBalanceLock)
        .def("setBrightness", &CameraControl::setBrightness)
        .def("setContrast", &CameraControl::setContrast)
        .def("setSaturation", &CameraControl::setSaturation)
        .def("setSharpness", &CameraControl::setSharpness)
        .def("setNoiseReductionStrength", &CameraControl::setNoiseReductionStrength)
        .def("setLumaDenoise", &CameraControl::setLumaDenoise)
        .def("setChromaDenoise", &CameraControl::setChromaDenoise)
        .def("setSceneMode", &CameraControl::setSceneMode)
        .def("setEffectMode", &CameraControl::setEffectMode)
        // getters
        .def("getCaptureStill", &CameraControl::getCaptureStill)
        ;
    // Add also enum attributes from RawCameraControl
    for (const auto& a : camCtrlAttr) {
        m.attr("CameraControl").attr(a) = m.attr("RawCameraControl").attr(a);
    }

    // Bind SystemInformation
    py::class_<SystemInformation, Buffer, std::shared_ptr<SystemInformation>>(m, "SystemInformation")
        .def(py::init<>())
        .def_property("ddrMemoryUsage", [](SystemInformation& i) { return &i.ddrMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.ddrMemoryUsage = val; } )
        .def_property("leonCssMemoryUsage", [](SystemInformation& i) { return &i.leonCssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonCssMemoryUsage = val; } )
        .def_property("leonMssMemoryUsage", [](SystemInformation& i) { return &i.leonMssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonMssMemoryUsage = val; } )
        .def_property("leonCssCpuUsage", [](SystemInformation& i) { return &i.leonCssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonCssCpuUsage = val; } )
        .def_property("leonMssCpuUsage", [](SystemInformation& i) { return &i.leonMssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonMssCpuUsage = val; } )
        .def_property("chipTemperature", [](SystemInformation& i) { return &i.chipTemperature; }, [](SystemInformation& i, ChipTemperature val) { i.chipTemperature = val; } )
        ;

}
