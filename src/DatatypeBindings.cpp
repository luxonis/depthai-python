#include "DatatypeBindings.hpp"

#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ADatatype.hpp"
#include "depthai/pipeline/datatype/Buffer.hpp"
#include "depthai/pipeline/datatype/ImgFrame.hpp"
#include "depthai/pipeline/datatype/NNData.hpp"
#include "depthai/pipeline/datatype/ImgDetections.hpp"
#include "depthai/pipeline/datatype/SpatialImgDetections.hpp"
#include "depthai/pipeline/datatype/ImageManipConfig.hpp"
#include "depthai/pipeline/datatype/CameraControl.hpp"
#include "depthai/pipeline/datatype/SystemInformation.hpp"
#include "depthai/pipeline/datatype/SpatialLocationCalculatorData.hpp"
#include "depthai/pipeline/datatype/SpatialLocationCalculatorConfig.hpp"
#include "depthai/pipeline/datatype/Tracklets.hpp"
#include "depthai/pipeline/datatype/IMUData.hpp"
#include "depthai/pipeline/datatype/StereoDepthConfig.hpp"
#include "depthai/pipeline/datatype/EdgeDetectorConfig.hpp"
#include "depthai/pipeline/datatype/TrackedFeatures.hpp"
#include "depthai/pipeline/datatype/FeatureTrackerConfig.hpp"

// depthai-shared
#include "depthai-shared/datatype/RawBuffer.hpp"
#include "depthai-shared/datatype/RawImgFrame.hpp"
#include "depthai-shared/datatype/RawNNData.hpp"
#include "depthai-shared/datatype/RawImgDetections.hpp"
#include "depthai-shared/datatype/RawSpatialImgDetections.hpp"
#include "depthai-shared/datatype/RawImageManipConfig.hpp"
#include "depthai-shared/datatype/RawCameraControl.hpp"
#include "depthai-shared/datatype/RawSystemInformation.hpp"
#include "depthai-shared/datatype/RawSpatialLocationCalculatorConfig.hpp"
#include "depthai-shared/datatype/RawSpatialLocations.hpp"
#include "depthai-shared/datatype/RawTracklets.hpp"
#include "depthai-shared/datatype/RawIMUData.hpp"
#include "depthai-shared/datatype/RawStereoDepthConfig.hpp"
#include "depthai-shared/datatype/RawEdgeDetectorConfig.hpp"
#include "depthai-shared/datatype/RawFeatureTrackerConfig.hpp"
#include "depthai-shared/datatype/RawTrackedFeatures.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void DatatypeBindings::bind(pybind11::module& m, void* pCallstack){


    using namespace dai;

    py::class_<RawBuffer, std::shared_ptr<RawBuffer>> rawBuffer(m, "RawBuffer", DOC(dai, RawBuffer));
    py::class_<RawImgFrame, RawBuffer, std::shared_ptr<RawImgFrame>> rawImgFrame(m, "RawImgFrame", DOC(dai, RawImgFrame));
    py::enum_<RawImgFrame::Type> rawImgFrameType(rawImgFrame, "Type");
    py::class_<RawImgFrame::Specs> rawImgFrameSpecs(rawImgFrame, "Specs", DOC(dai, RawImgFrame, Specs));
    py::class_<RawNNData, RawBuffer, std::shared_ptr<RawNNData>> rawNnData(m, "RawNNData", DOC(dai, RawNNData));
    py::class_<TensorInfo> tensorInfo(m, "TensorInfo", DOC(dai, TensorInfo));
    py::enum_<TensorInfo::DataType>tensorInfoDataType(tensorInfo, "DataType");
    py::enum_<TensorInfo::StorageOrder>tensorInfoStorageOrder(tensorInfo, "StorageOrder");
    py::class_<ImgDetection> imgDetection(m, "ImgDetection", DOC(dai, ImgDetection));
    py::class_<SpatialImgDetection, ImgDetection> spatialImgDetection(m, "SpatialImgDetection", DOC(dai, SpatialImgDetection));
    py::class_<RawImgDetections, RawBuffer, std::shared_ptr<RawImgDetections>> rawImgDetections(m, "RawImgDetections", DOC(dai, RawImgDetections));
    py::class_<RawSpatialImgDetections, RawBuffer, std::shared_ptr<RawSpatialImgDetections>> rawSpatialImgDetections(m, "RawSpatialImgDetections", DOC(dai, RawSpatialImgDetections));
    py::class_<RawImageManipConfig, RawBuffer, std::shared_ptr<RawImageManipConfig>> rawImageManipConfig(m, "RawImageManipConfig", DOC(dai, RawImageManipConfig));
    py::class_<RawImageManipConfig::CropRect> rawImageManipConfigCropRect(rawImageManipConfig, "CropRect", DOC(dai, RawImageManipConfig, CropRect));
    py::class_<RawImageManipConfig::CropConfig> rawImageManipCropConfig(rawImageManipConfig, "CropConfig", DOC(dai, RawImageManipConfig, CropConfig));
    py::class_<RawImageManipConfig::ResizeConfig>rawImageManipConfigResizeConfig(rawImageManipConfig, "ResizeConfig", DOC(dai, RawImageManipConfig, ResizeConfig));
    py::class_<RawImageManipConfig::FormatConfig> rawImageManipConfigFormatConfig(rawImageManipConfig, "FormatConfig", DOC(dai, RawImageManipConfig, FormatConfig));
    py::class_<RawCameraControl, RawBuffer, std::shared_ptr<RawCameraControl>> rawCameraControl(m, "RawCameraControl", DOC(dai, RawCameraControl));
    py::class_<Tracklet> tracklet(m, "Tracklet", DOC(dai, Tracklet));
    py::enum_<Tracklet::TrackingStatus> trackletTrackingStatus(tracklet, "TrackingStatus", DOC(dai, Tracklet, TrackingStatus));
    py::class_<RawTracklets, RawBuffer, std::shared_ptr<RawTracklets>> rawTacklets(m, "RawTracklets", DOC(dai, RawTracklets));
    py::class_<IMUReport, std::shared_ptr<IMUReport>> imuReport(m, "IMUReport", DOC(dai, IMUReport));
    py::enum_<IMUReport::Accuracy> imuReportAccuracy(imuReport, "Accuracy");
    py::class_<IMUReportAccelerometer, IMUReport, std::shared_ptr<IMUReportAccelerometer>> imuReportAccelerometer(m, "IMUReportAccelerometer", DOC(dai, IMUReportAccelerometer));
    py::class_<IMUReportGyroscope, IMUReport, std::shared_ptr<IMUReportGyroscope>> imuReportGyroscope(m, "IMUReportGyroscope", DOC(dai, IMUReportGyroscope));
    py::class_<IMUReportMagneticField, IMUReport, std::shared_ptr<IMUReportMagneticField>> imuReportMagneticField(m, "IMUReportMagneticField", DOC(dai, IMUReportMagneticField));
    py::class_<IMUReportRotationVectorWAcc, IMUReport, std::shared_ptr<IMUReportRotationVectorWAcc>> imuReportRotationVectorWAcc(m, "IMUReportRotationVectorWAcc", DOC(dai, IMUReportRotationVectorWAcc));
    py::class_<IMUPacket> imuPacket(m, "IMUPacket", DOC(dai, IMUPacket));
    py::class_<RawIMUData, RawBuffer, std::shared_ptr<RawIMUData>> rawIMUPackets(m, "RawIMUData", DOC(dai, RawIMUData));
    py::enum_<RawCameraControl::AutoFocusMode> rawCameraControlAutoFocusMode(rawCameraControl, "AutoFocusMode", DOC(dai, RawCameraControl, AutoFocusMode));
    py::enum_<RawCameraControl::AutoWhiteBalanceMode> rawCameraControlAutoWhiteBalanceMode(rawCameraControl, "AutoWhiteBalanceMode", DOC(dai, RawCameraControl, AutoWhiteBalanceMode));
    py::enum_<RawCameraControl::SceneMode> rawCameraControlSceneMode(rawCameraControl, "SceneMode", DOC(dai, RawCameraControl, SceneMode));
    py::enum_<RawCameraControl::AntiBandingMode> rawCameraControlAntiBandingMode(rawCameraControl, "AntiBandingMode", DOC(dai, RawCameraControl, AntiBandingMode));
    py::enum_<RawCameraControl::EffectMode> rawCameraControlEffectMode(rawCameraControl, "EffectMode", DOC(dai, RawCameraControl, EffectMode));
    py::class_<RawSystemInformation, RawBuffer, std::shared_ptr<RawSystemInformation>> rawSystemInformation(m, "RawSystemInformation", DOC(dai, RawSystemInformation));
    py::class_<ADatatype, std::shared_ptr<ADatatype>> adatatype(m, "ADatatype", DOC(dai, ADatatype));
    py::class_<Buffer, ADatatype, std::shared_ptr<Buffer>> buffer(m, "Buffer", DOC(dai, Buffer));
    py::class_<ImgFrame, Buffer, std::shared_ptr<ImgFrame>> imgFrame(m, "ImgFrame", DOC(dai, ImgFrame));
    py::class_<RotatedRect> rotatedRect(m, "RotatedRect", DOC(dai, RotatedRect));
    py::class_<NNData, Buffer, std::shared_ptr<NNData>> nnData(m, "NNData", DOC(dai, NNData));
    py::class_<ImgDetections, Buffer, std::shared_ptr<ImgDetections>> imgDetections(m, "ImgDetections", DOC(dai, ImgDetections));
    py::class_<SpatialImgDetections, Buffer, std::shared_ptr<SpatialImgDetections>> spatialImgDetections(m, "SpatialImgDetections", DOC(dai, SpatialImgDetections));
    py::class_<ImageManipConfig, Buffer, std::shared_ptr<ImageManipConfig>> imageManipConfig(m, "ImageManipConfig", DOC(dai, ImageManipConfig));
    py::class_<CameraControl, Buffer, std::shared_ptr<CameraControl>> cameraControl(m, "CameraControl", DOC(dai, CameraControl));
    py::class_<SystemInformation, Buffer, std::shared_ptr<SystemInformation>> systemInformation(m, "SystemInformation", DOC(dai, SystemInformation));
    py::class_<SpatialLocations> spatialLocations(m, "SpatialLocations", DOC(dai, SpatialLocations));
    py::class_<Rect> rect(m, "Rect", DOC(dai, Rect));
    py::class_<SpatialLocationCalculatorConfigThresholds> spatialLocationCalculatorConfigThresholds(m, "SpatialLocationCalculatorConfigThresholds", DOC(dai, SpatialLocationCalculatorConfigThresholds));
    py::enum_<SpatialLocationCalculatorAlgorithm> spatialLocationCalculatorAlgorithm(m, "SpatialLocationCalculatorAlgorithm", DOC(dai, SpatialLocationCalculatorAlgorithm));
    py::class_<SpatialLocationCalculatorConfigData> spatialLocationCalculatorConfigData(m, "SpatialLocationCalculatorConfigData", DOC(dai, SpatialLocationCalculatorConfigData));
    py::class_<SpatialLocationCalculatorData, Buffer, std::shared_ptr<SpatialLocationCalculatorData>> spatialLocationCalculatorData(m, "SpatialLocationCalculatorData", DOC(dai, SpatialLocationCalculatorData));
    py::class_<SpatialLocationCalculatorConfig, Buffer, std::shared_ptr<SpatialLocationCalculatorConfig>> spatialLocationCalculatorConfig(m, "SpatialLocationCalculatorConfig", DOC(dai, SpatialLocationCalculatorConfig));
    py::class_<Tracklets, Buffer, std::shared_ptr<Tracklets>> tracklets(m, "Tracklets", DOC(dai, Tracklets));
    py::class_<IMUData, Buffer, std::shared_ptr<IMUData>> imuData(m, "IMUData", DOC(dai, IMUData));
    // Stereo depth
    py::class_<RawStereoDepthConfig, RawBuffer, std::shared_ptr<RawStereoDepthConfig>> rawStereoDepthConfig(m, "RawStereoDepthConfig", DOC(dai, RawStereoDepthConfig));
    py::enum_<MedianFilter> medianFilter(m, "MedianFilter", DOC(dai, MedianFilter));
    py::class_<RawStereoDepthConfig::AlgorithmControl> algorithmControl(rawStereoDepthConfig, "AlgorithmControl", DOC(dai, RawStereoDepthConfig, AlgorithmControl));
    py::class_<RawStereoDepthConfig::PostProcessing> postProcessing(rawStereoDepthConfig, "PostProcessing", DOC(dai, RawStereoDepthConfig, PostProcessing));
    py::class_<RawStereoDepthConfig::CostAggregation> costAggregation(rawStereoDepthConfig, "CostAggregation", DOC(dai, RawStereoDepthConfig, CostAggregation));
    py::class_<RawStereoDepthConfig::CostMatching> costMatching(rawStereoDepthConfig, "CostMatching", DOC(dai, RawStereoDepthConfig, CostMatching));
    py::class_<RawStereoDepthConfig::CostMatching::LinearEquationParameters> costMatchingLinearEquationParameters(costMatching, "LinearEquationParameters", DOC(dai, RawStereoDepthConfig, CostMatching, LinearEquationParameters));
    py::enum_<RawStereoDepthConfig::CostMatching::DisparityWidth> costMatchingDisparityWidth(costMatching, "DisparityWidth", DOC(dai, RawStereoDepthConfig, CostMatching, DisparityWidth));
    py::class_<RawStereoDepthConfig::CensusTransform> censusTransform(rawStereoDepthConfig, "CensusTransform", DOC(dai, RawStereoDepthConfig, CensusTransform));
    py::enum_<RawStereoDepthConfig::CensusTransform::KernelSize> censusTransformKernelSize(censusTransform, "KernelSize", DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize));
    py::class_<StereoDepthConfig, Buffer, std::shared_ptr<StereoDepthConfig>> stereoDepthConfig(m, "StereoDepthConfig", DOC(dai, StereoDepthConfig));
    // Edge detector
    py::class_<EdgeDetectorConfigData> edgeDetectorConfigData(m, "EdgeDetectorConfigData", DOC(dai, EdgeDetectorConfigData));
    py::class_<RawEdgeDetectorConfig, RawBuffer, std::shared_ptr<RawEdgeDetectorConfig>> rawEdgeDetectorConfig(m, "RawEdgeDetectorConfig", DOC(dai, RawEdgeDetectorConfig));
    py::class_<EdgeDetectorConfig, Buffer, std::shared_ptr<EdgeDetectorConfig>> edgeDetectorConfig(m, "EdgeDetectorConfig", DOC(dai, EdgeDetectorConfig));
    // Feature tracker
    py::class_<RawTrackedFeatures, RawBuffer, std::shared_ptr<RawTrackedFeatures>> rawTrackedFeatures(m, "RawTrackedFeatures", DOC(dai, RawTrackedFeatures));
    py::class_<TrackedFeature> trackedFeature(m, "TrackedFeature", DOC(dai, TrackedFeature));
    py::class_<RawFeatureTrackerConfig, RawBuffer, std::shared_ptr<RawFeatureTrackerConfig>> rawFeatureTrackerConfig(m, "RawFeatureTrackerConfig", DOC(dai, RawFeatureTrackerConfig));
    py::class_<RawFeatureTrackerConfig::CornerDetector> cornerDetector(rawFeatureTrackerConfig, "CornerDetector", DOC(dai, RawFeatureTrackerConfig, CornerDetector));
    py::enum_<RawFeatureTrackerConfig::CornerDetector::Type> cornerDetectorType(cornerDetector, "Type", DOC(dai, RawFeatureTrackerConfig, CornerDetector, Type));
    py::class_<RawFeatureTrackerConfig::CornerDetector::Thresholds> cornerDetectorThresholds(cornerDetector, "Thresholds", DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds));
    py::class_<RawFeatureTrackerConfig::MotionEstimator> motionEstimator(rawFeatureTrackerConfig, "MotionEstimator", DOC(dai, RawFeatureTrackerConfig, MotionEstimator));
    py::enum_<RawFeatureTrackerConfig::MotionEstimator::Type> motionEstimatorType(motionEstimator, "Type", DOC(dai, RawFeatureTrackerConfig, MotionEstimator, Type));
    py::class_<RawFeatureTrackerConfig::MotionEstimator::OpticalFlow> motionEstimatorOpticalFlow(motionEstimator, "OpticalFlow", DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow));

    py::class_<RawFeatureTrackerConfig::FeatureMaintainer> featureMaintainer(rawFeatureTrackerConfig, "FeatureMaintainer", DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer));
    py::class_<FeatureTrackerConfig, Buffer, std::shared_ptr<FeatureTrackerConfig>> featureTrackerConfig(m, "FeatureTrackerConfig", DOC(dai, FeatureTrackerConfig));


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


    rawBuffer
        .def(py::init<>())
        .def_property("data", [](py::object &obj){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            return py::array_t<uint8_t>(a.data.size(), a.data.data(), obj);
        }, [](py::object &obj, py::array_t<std::uint8_t, py::array::c_style> array){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            a.data = {array.data(), array.data() + array.size()};
        })
        ;



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
        .def_property("tsDevice",
            [](const RawImgFrame& o){
                double ts = o.tsDevice.sec + o.tsDevice.nsec / 1000000000.0;
                return ts;
            },
            [](RawImgFrame& o, double ts){
                o.tsDevice.sec = ts;
                o.tsDevice.nsec = (ts - o.tsDevice.sec) * 1000000000.0;
            }
        )
        ;


    rawImgFrameType
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

    rawImgFrameSpecs
        .def(py::init<>())
        .def_readwrite("type", &RawImgFrame::Specs::type)
        .def_readwrite("width", &RawImgFrame::Specs::width)
        .def_readwrite("height", &RawImgFrame::Specs::height)
        .def_readwrite("stride", &RawImgFrame::Specs::stride)
        .def_readwrite("bytesPP", &RawImgFrame::Specs::bytesPP)
        .def_readwrite("p1Offset", &RawImgFrame::Specs::p1Offset)
        .def_readwrite("p2Offset", &RawImgFrame::Specs::p2Offset)
        .def_readwrite("p3Offset", &RawImgFrame::Specs::p3Offset)
        ;


    rawNnData
        .def(py::init<>())
        .def_readwrite("tensors", &RawNNData::tensors)
        .def_readwrite("batchSize", &RawNNData::batchSize)
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

    imgDetection
        .def(py::init<>())
        .def_readwrite("label", &ImgDetection::label)
        .def_readwrite("confidence", &ImgDetection::confidence)
        .def_readwrite("xmin", &ImgDetection::xmin)
        .def_readwrite("ymin", &ImgDetection::ymin)
        .def_readwrite("xmax", &ImgDetection::xmax)
        .def_readwrite("ymax", &ImgDetection::ymax)
        ;


    spatialImgDetection
        .def(py::init<>())
        .def_readwrite("spatialCoordinates", &SpatialImgDetection::spatialCoordinates)
        ;

    rawImgDetections
        .def(py::init<>())
        .def_readwrite("detections", &RawImgDetections::detections)
        ;

    rawSpatialImgDetections
        .def(py::init<>())
        .def_readwrite("detections", &RawSpatialImgDetections::detections)
        ;

    // Bind RawImageManipConfig
    rawImageManipConfig
        .def(py::init<>())
        .def_readwrite("enableFormat", &RawImageManipConfig::enableFormat)
        .def_readwrite("enableResize", &RawImageManipConfig::enableResize)
        .def_readwrite("enableCrop", &RawImageManipConfig::enableCrop)
        .def_readwrite("cropConfig", &RawImageManipConfig::cropConfig)
        .def_readwrite("resizeConfig", &RawImageManipConfig::resizeConfig)
        .def_readwrite("formatConfig", &RawImageManipConfig::formatConfig)
        ;

    rawImageManipConfigCropRect
        .def(py::init<>())
        .def_readwrite("xmin", &RawImageManipConfig::CropRect::xmin)
        .def_readwrite("ymin", &RawImageManipConfig::CropRect::ymin)
        .def_readwrite("xmax", &RawImageManipConfig::CropRect::xmax)
        .def_readwrite("ymax", &RawImageManipConfig::CropRect::ymax)
        ;

    rawImageManipCropConfig
        .def(py::init<>())
        .def_readwrite("cropRect", &RawImageManipConfig::CropConfig::cropRect)
        .def_readwrite("cropRotatedRect", &RawImageManipConfig::CropConfig::cropRotatedRect)
        .def_readwrite("enableCenterCropRectangle", &RawImageManipConfig::CropConfig::enableCenterCropRectangle)
        .def_readwrite("cropRatio", &RawImageManipConfig::CropConfig::cropRatio)
        .def_readwrite("widthHeightAspectRatio", &RawImageManipConfig::CropConfig::widthHeightAspectRatio)
        .def_readwrite("enableRotatedRect", &RawImageManipConfig::CropConfig::enableRotatedRect)
        .def_readwrite("normalizedCoords", &RawImageManipConfig::CropConfig::normalizedCoords)
        ;

    rawImageManipConfigResizeConfig
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
        .def_readwrite("keepAspectRatio", &RawImageManipConfig::ResizeConfig::keepAspectRatio)
        ;

    rawImageManipConfigFormatConfig
        .def(py::init<>())
        .def_readwrite("type", &RawImageManipConfig::FormatConfig::type)
        .def_readwrite("flipHorizontal", &RawImageManipConfig::FormatConfig::flipHorizontal)
        ;


    // Bind RawCameraControl
    rawCameraControl
        .def(py::init<>())
        .def_readwrite("cmdMask", &RawCameraControl::cmdMask)
        .def_readwrite("autoFocusMode", &RawCameraControl::autoFocusMode)
        .def_readwrite("lensPosition", &RawCameraControl::lensPosition)
        // TODO add more raw types here, not directly used
        ;

    tracklet
        .def(py::init<>())
        .def_readwrite("roi", &Tracklet::roi)
        .def_readwrite("id", &Tracklet::id)
        .def_readwrite("label", &Tracklet::label)
        .def_readwrite("status", &Tracklet::status)
        .def_readwrite("srcImgDetection", &Tracklet::srcImgDetection)
        .def_readwrite("spatialCoordinates", &Tracklet::spatialCoordinates)
        ;

    trackletTrackingStatus
        .value("NEW", Tracklet::TrackingStatus::NEW)
        .value("TRACKED", Tracklet::TrackingStatus::TRACKED)
        .value("LOST", Tracklet::TrackingStatus::LOST)
        .value("REMOVED", Tracklet::TrackingStatus::REMOVED)
        ;

    // Bind RawTracklets
    rawTacklets
        .def(py::init<>())
        .def_readwrite("tracklets", &RawTracklets::tracklets)
        ;


    imuReport
        .def(py::init<>())
        .def_readwrite("sequence", &IMUReport::sequence)
        .def_readwrite("accuracy", &IMUReport::accuracy)
        .def_readwrite("timestamp", &IMUReport::timestamp)
        ;


    imuReportAccuracy
        .value("UNRELIABLE", IMUReport::Accuracy::UNRELIABLE)
        .value("LOW", IMUReport::Accuracy::LOW)
        .value("MEDIUM", IMUReport::Accuracy::MEDIUM)
        .value("HIGH", IMUReport::Accuracy::HIGH)
        ;

    imuReportAccelerometer
        .def(py::init<>())
        .def_readwrite("x", &IMUReportAccelerometer::x)
        .def_readwrite("y", &IMUReportAccelerometer::y)
        .def_readwrite("z", &IMUReportAccelerometer::z)
        ;

    imuReportGyroscope
        .def(py::init<>())
        .def_readwrite("x", &IMUReportGyroscope::x)
        .def_readwrite("y", &IMUReportGyroscope::y)
        .def_readwrite("z", &IMUReportGyroscope::z)
        ;

    imuReportMagneticField
        .def(py::init<>())
        .def_readwrite("x", &IMUReportMagneticField::x)
        .def_readwrite("y", &IMUReportMagneticField::y)
        .def_readwrite("z", &IMUReportMagneticField::z)
        ;

    imuReportRotationVectorWAcc
        .def(py::init<>())
        .def_readwrite("i", &IMUReportRotationVectorWAcc::i)
        .def_readwrite("j", &IMUReportRotationVectorWAcc::j)
        .def_readwrite("k", &IMUReportRotationVectorWAcc::k)
        .def_readwrite("real", &IMUReportRotationVectorWAcc::real)
        .def_readwrite("rotationVectorAccuracy", &IMUReportRotationVectorWAcc::rotationVectorAccuracy)
        ;

#if 0
    py::class_<IMUReportRotationVector, IMUReport, std::shared_ptr<IMUReportRotationVector>>(m, "IMUReportRotationVector", DOC(dai, IMUReportRotationVector))
        .def(py::init<>())
        .def_readwrite("i", &IMUReportRotationVector::i)
        .def_readwrite("j", &IMUReportRotationVector::j)
        .def_readwrite("k", &IMUReportRotationVector::k)
        .def_readwrite("real", &IMUReportRotationVector::real)
        ;

    py::class_<IMUReportGyroscopeUncalibrated, IMUReport, std::shared_ptr<IMUReportGyroscopeUncalibrated>>(m, "IMUReportGyroscopeUncalibrated", DOC(dai, IMUReportGyroscopeUncalibrated))
        .def(py::init<>())
        .def_readwrite("x", &IMUReportGyroscopeUncalibrated::x)
        .def_readwrite("y", &IMUReportGyroscopeUncalibrated::y)
        .def_readwrite("z", &IMUReportGyroscopeUncalibrated::z)
        .def_readwrite("biasX", &IMUReportGyroscopeUncalibrated::biasX)
        .def_readwrite("biasY", &IMUReportGyroscopeUncalibrated::biasY)
        .def_readwrite("biasZ", &IMUReportGyroscopeUncalibrated::biasZ)
        ;

    py::class_<IMUReportMagneticFieldUncalibrated, IMUReport, std::shared_ptr<IMUReportMagneticFieldUncalibrated>>(m, "IMUReportMagneticFieldUncalibrated", DOC(dai, IMUReportMagneticFieldUncalibrated))
        .def(py::init<>())
        .def_readwrite("x", &IMUReportMagneticFieldUncalibrated::x)
        .def_readwrite("y", &IMUReportMagneticFieldUncalibrated::y)
        .def_readwrite("z", &IMUReportMagneticFieldUncalibrated::z)
        .def_readwrite("biasX", &IMUReportMagneticFieldUncalibrated::biasX)
        .def_readwrite("biasY", &IMUReportMagneticFieldUncalibrated::biasY)
        .def_readwrite("biasZ", &IMUReportMagneticFieldUncalibrated::biasZ)
        ;

    py::class_<IMUReportGyroIntegratedRV, IMUReport, std::shared_ptr<IMUReportGyroIntegratedRV>>(m, "IMUReportGyroIntegratedRV", DOC(dai, IMUReportGyroIntegratedRV))
        .def(py::init<>())
        .def_readwrite("i", &IMUReportGyroIntegratedRV::i)
        .def_readwrite("j", &IMUReportGyroIntegratedRV::j)
        .def_readwrite("k", &IMUReportGyroIntegratedRV::k)
        .def_readwrite("real", &IMUReportGyroIntegratedRV::real)
        .def_readwrite("angVelX", &IMUReportGyroIntegratedRV::angVelX)
        .def_readwrite("angVelY", &IMUReportGyroIntegratedRV::angVelY)
        .def_readwrite("angVelZ", &IMUReportGyroIntegratedRV::angVelZ)
        ;

#endif

    imuPacket
        .def(py::init<>())
        .def_readwrite("acceleroMeter", &IMUPacket::acceleroMeter)
        .def_readwrite("gyroscope", &IMUPacket::gyroscope)
        .def_readwrite("magneticField", &IMUPacket::magneticField)
        .def_readwrite("rotationVector", &IMUPacket::rotationVector)
#if 0
        .def_readwrite("rawAcceleroMeter", &IMUPacket::rawAcceleroMeter)
        .def_readwrite("linearAcceleroMeter", &IMUPacket::linearAcceleroMeter)
        .def_readwrite("gravity", &IMUPacket::gravity)
        .def_readwrite("rawGyroscope", &IMUPacket::rawGyroscope)
        .def_readwrite("gyroscopeUncalibrated", &IMUPacket::gyroscopeUncalibrated)
        .def_readwrite("rawMagneticField", &IMUPacket::rawMagneticField)
        .def_readwrite("magneticFieldUncalibrated", &IMUPacket::magneticFieldUncalibrated)
        .def_readwrite("gameRotationVector", &IMUPacket::gameRotationVector)
        .def_readwrite("geoMagRotationVector", &IMUPacket::geoMagRotationVector)
        .def_readwrite("arvrStabilizedRotationVector", &IMUPacket::arvrStabilizedRotationVector)
        .def_readwrite("arvrStabilizedGameRotationVector", &IMUPacket::arvrStabilizedGameRotationVector)
        .def_readwrite("gyroIntegratedRotationVector", &IMUPacket::gyroIntegratedRotationVector)
#endif
        ;


    // Bind RawIMUData
    rawIMUPackets
        .def(py::init<>())
        .def_readwrite("packets", &RawIMUData::packets)
        ;

    // RawCameraControl enum bindings
    // The enum fields will also be exposed in 'CameraControl', store them for later
    std::vector<const char *> camCtrlAttr;
    camCtrlAttr.push_back("AutoFocusMode");
    rawCameraControlAutoFocusMode
        .value("OFF", RawCameraControl::AutoFocusMode::OFF)
        .value("AUTO", RawCameraControl::AutoFocusMode::AUTO)
        .value("MACRO", RawCameraControl::AutoFocusMode::MACRO)
        .value("CONTINUOUS_VIDEO", RawCameraControl::AutoFocusMode::CONTINUOUS_VIDEO)
        .value("CONTINUOUS_PICTURE", RawCameraControl::AutoFocusMode::CONTINUOUS_PICTURE)
        .value("EDOF", RawCameraControl::AutoFocusMode::EDOF)
    ;

    camCtrlAttr.push_back("AutoWhiteBalanceMode");
    rawCameraControlAutoWhiteBalanceMode
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
    rawCameraControlSceneMode
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
    rawCameraControlAntiBandingMode
        .value("OFF", RawCameraControl::AntiBandingMode::OFF)
        .value("MAINS_50_HZ", RawCameraControl::AntiBandingMode::MAINS_50_HZ)
        .value("MAINS_60_HZ", RawCameraControl::AntiBandingMode::MAINS_60_HZ)
        .value("AUTO", RawCameraControl::AntiBandingMode::AUTO)
    ;

    camCtrlAttr.push_back("EffectMode");
    rawCameraControlEffectMode
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
    rawSystemInformation
        .def(py::init<>())
        .def_readwrite("ddrMemoryUsage", &RawSystemInformation::ddrMemoryUsage)
        .def_readwrite("cmxMemoryUsage", &RawSystemInformation::cmxMemoryUsage)
        .def_readwrite("leonCssMemoryUsage", &RawSystemInformation::leonCssMemoryUsage)
        .def_readwrite("leonMssMemoryUsage", &RawSystemInformation::leonMssMemoryUsage)
        .def_readwrite("leonCssCpuUsage", &RawSystemInformation::leonCssCpuUsage)
        .def_readwrite("leonMssCpuUsage", &RawSystemInformation::leonMssCpuUsage)
        .def_readwrite("chipTemperature", &RawSystemInformation::chipTemperature)
        ;


    // Bind non-raw 'helper' datatypes
    adatatype
        .def("getRaw", &ADatatype::getRaw);


    buffer
        .def(py::init<>(), DOC(dai, Buffer, Buffer))

        // obj is "Python" object, which we used then to bind the numpy arrays lifespan to
        .def("getData", [](py::object &obj){
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            dai::Buffer &a = obj.cast<dai::Buffer&>();
            return py::array_t<uint8_t>(a.getData().size(), a.getData().data(), obj);
        }, DOC(dai, Buffer, getData))
        .def("setData", &Buffer::setData, DOC(dai, Buffer, setData))
        .def("setData", [](Buffer& buffer, py::array_t<std::uint8_t, py::array::c_style | py::array::forcecast> array){
            buffer.getData().clear();
            buffer.getData().insert(buffer.getData().begin(), array.data(), array.data() + array.nbytes());
        }, DOC(dai, Buffer, setData))
        ;

    // Bind ImgFrame
    imgFrame
        .def(py::init<>())
        // getters
        .def("getTimestamp", &ImgFrame::getTimestamp, DOC(dai, ImgFrame, getTimestamp))
        .def("getTimestampDevice", &ImgFrame::getTimestampDevice, DOC(dai, ImgFrame, getTimestampDevice))
        .def("getInstanceNum", &ImgFrame::getInstanceNum, DOC(dai, ImgFrame, getInstanceNum))
        .def("getCategory", &ImgFrame::getCategory, DOC(dai, ImgFrame, getCategory))
        .def("getSequenceNum", &ImgFrame::getSequenceNum, DOC(dai, ImgFrame, getSequenceNum))
        .def("getWidth", &ImgFrame::getWidth, DOC(dai, ImgFrame, getWidth))
        .def("getHeight", &ImgFrame::getHeight, DOC(dai, ImgFrame, getHeight))
        .def("getType", &ImgFrame::getType, DOC(dai, ImgFrame, getType))

        // OpenCV Support section
        .def("setFrame", [](dai::ImgFrame& frm, py::array arr){
             // Try importing 'numpy' module
            py::module numpy;
            try {
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'setFrame' requires 'numpy' module");
            }

            py::array contiguous = numpy.attr("ascontiguousarray")(arr);
            frm.getData().resize(contiguous.nbytes());
            memcpy(frm.getData().data(), contiguous.data(), contiguous.nbytes());

        }, py::arg("array"), "Copies array bytes to ImgFrame buffer")
        .def("getFrame", [](py::object &obj, bool copy){

            // Try importing 'numpy' module
            py::module numpy;
            try {
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'getFrame' requires 'numpy' module");
            }

            // obj is "Python" object, which we used then to bind the numpy view lifespan to
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            auto& img = obj.cast<dai::ImgFrame&>();

            // shape
            bool valid = img.getWidth() > 0 && img.getHeight() > 0;
            std::vector<std::size_t> shape = {img.getData().size()};
            py::dtype dtype = py::dtype::of<uint8_t>();

            switch(img.getType()){

                case ImgFrame::Type::RGB888i :
                case ImgFrame::Type::BGR888i :
                    // HWC
                    shape = {img.getHeight(), img.getWidth(), 3};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::RGB888p :
                case ImgFrame::Type::BGR888p :
                    // CHW
                    shape = {3, img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::YUV420p:
                case ImgFrame::Type::NV12:
                case ImgFrame::Type::NV21:
                    // Height 1.5x actual size
                    shape = {img.getHeight() * 3 / 2, img.getWidth()};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::RAW8:
                case ImgFrame::Type::GRAY8:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::GRAYF16:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::RAW16:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint16_t>();
                break;

                case ImgFrame::Type::RGBF16F16F16i:
                case ImgFrame::Type::BGRF16F16F16i:
                    shape = {img.getHeight(), img.getWidth(), 3};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::RGBF16F16F16p:
                case ImgFrame::Type::BGRF16F16F16p:
                    shape = {3, img.getHeight(), img.getWidth()};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::BITSTREAM :
                default:
                    shape = {img.getData().size()};
                    dtype = py::dtype::of<uint8_t>();
                    break;
            }

            // Check if enough data
            long actualSize = img.getData().size();
            long requiredSize = dtype.itemsize();
            for(const auto& dim : shape) requiredSize *= dim;
            if(actualSize < requiredSize){
                throw std::runtime_error("ImgFrame doesn't have enough data to encode specified frame, required " + std::to_string(requiredSize)
                        + ", actual " + std::to_string(actualSize) + ". Maybe metadataOnly transfer was made?");
            } else if(actualSize > requiredSize) {
                // FIXME check build on Windows
                // spdlog::warn("ImgFrame has excess data: actual {}, expected {}", actualSize, requiredSize);
            }
            if(img.getWidth() <= 0 || img.getHeight() <= 0){
                throw std::runtime_error("ImgFrame size invalid (width: " + std::to_string(img.getWidth()) + ", height: " + std::to_string(img.getHeight()) + ")");
            }

            if(copy){
                py::array a(dtype, shape);
                std::memcpy(a.mutable_data(), img.getData().data(), std::min( (long) (img.getData().size()), (long) (a.nbytes())));
                return a;
            } else {
                return py::array(dtype, shape, img.getData().data(), obj);
            }

        }, py::arg("copy") = false, "Returns numpy array with shape as specified by width, height and type")

        .def("getCvFrame", [](py::object &obj){
            using namespace pybind11::literals;

            // Try importing 'cv2' module
            py::module cv2;
            py::module numpy;
            try {
                cv2 = py::module::import("cv2");
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'getCvFrame' requires 'cv2' module (opencv-python package)");
            }

            // ImgFrame
            auto& img = obj.cast<dai::ImgFrame&>();

            // Get numpy frame (python object) by calling getFrame
            auto frame = obj.attr("getFrame")();

            // Convert numpy array to bgr frame using cv2 module
            switch(img.getType()) {

                case ImgFrame::Type::BGR888p:
                    return numpy.attr("ascontiguousarray")(frame.attr("transpose")(1, 2, 0));
                    break;

                case ImgFrame::Type::BGR888i:
                    return frame.attr("copy")();
                    break;

                case ImgFrame::Type::RGB888p:
                    // Transpose to RGB888i then convert to BGR
                    return cv2.attr("cvtColor")(frame.attr("transpose")(1, 2, 0), cv2.attr("COLOR_RGB2BGR"));
                    break;

                case ImgFrame::Type::RGB888i:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_RGB2BGR"));
                    break;

                case ImgFrame::Type::YUV420p:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_YUV2BGR_IYUV"));
                    break;

                case ImgFrame::Type::NV12:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_YUV2BGR_NV12"));
                    break;

                case ImgFrame::Type::NV21:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_YUV2BGR_NV21"));
                    break;

                case ImgFrame::Type::RAW8:
                case ImgFrame::Type::RAW16:
                case ImgFrame::Type::GRAY8:
                case ImgFrame::Type::GRAYF16:
                default:
                    return frame.attr("copy")();
                    break;
            }

            // Default case
            return frame.attr("copy")();

        }, "Returns BGR or grayscale frame compatible with use in other opencv functions")

        // setters
        .def("setTimestamp", &ImgFrame::setTimestamp, py::arg("timestamp"), DOC(dai, ImgFrame, setTimestamp))
        .def("setInstanceNum", &ImgFrame::setInstanceNum, py::arg("instance"), DOC(dai, ImgFrame, setInstanceNum))
        .def("setCategory", &ImgFrame::setCategory, py::arg("category"), DOC(dai, ImgFrame, setCategory))
        .def("setSequenceNum", &ImgFrame::setSequenceNum, py::arg("seq"), DOC(dai, ImgFrame, setSequenceNum))
        .def("setWidth", &ImgFrame::setWidth, py::arg("width"), DOC(dai, ImgFrame, setWidth))
        .def("setHeight", &ImgFrame::setHeight, py::arg("height"), DOC(dai, ImgFrame, setHeight))
        .def("setSize", static_cast<void(ImgFrame::*)(unsigned int, unsigned int)>(&ImgFrame::setSize), py::arg("width"), py::arg("height"), DOC(dai, ImgFrame, setSize))
        .def("setSize", static_cast<void(ImgFrame::*)(std::tuple<unsigned int, unsigned int>)>(&ImgFrame::setSize), py::arg("sizer"), DOC(dai, ImgFrame, setSize, 2))
        .def("setType", &ImgFrame::setType, py::arg("type"), DOC(dai, ImgFrame, setType))
        ;
    // add aliases dai.ImgFrame.Type and dai.ImgFrame.Specs
    m.attr("ImgFrame").attr("Type") = m.attr("RawImgFrame").attr("Type");
    m.attr("ImgFrame").attr("Specs") = m.attr("RawImgFrame").attr("Specs");

    rotatedRect
        .def(py::init<>())
        .def_readwrite("center", &RotatedRect::center)
        .def_readwrite("size", &RotatedRect::size)
        .def_readwrite("angle", &RotatedRect::angle)
        ;

    // Bind NNData

    nnData
        .def(py::init<>(), DOC(dai, NNData, NNData))
        // setters
        .def("setLayer", [](NNData& obj, const std::string& name, py::array_t<std::uint8_t, py::array::c_style | py::array::forcecast> data){
            std::vector<std::uint8_t> vec(data.data(), data.data() + data.size());
            obj.setLayer(name, std::move(vec));
        }, py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer))
        .def("setLayer", static_cast<void(NNData::*)(const std::string&, const std::vector<int>&)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 2))
        .def("setLayer", static_cast<void(NNData::*)(const std::string&, std::vector<float>)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 3))
        .def("setLayer", static_cast<void(NNData::*)(const std::string&, std::vector<double>)>(&NNData::setLayer), py::arg("name"), py::arg("data"), DOC(dai, NNData, setLayer, 4))
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
        ;

    // Bind ImgDetections

    imgDetections
        .def(py::init<>(), DOC(dai, ImgDetections, ImgDetections))
        .def_property("detections", [](ImgDetections& det) { return &det.detections; }, [](ImgDetections& det, std::vector<ImgDetection> val) { det.detections = val; }, DOC(dai, ImgDetections, detections))
        ;

    // Bind SpatialImgDetections

    spatialImgDetections
        .def(py::init<>())
        .def_property("detections", [](SpatialImgDetections& det) { return &det.detections; }, [](SpatialImgDetections& det, std::vector<SpatialImgDetection> val) { det.detections = val; })
        ;

     // Bind ImageManipConfig

    imageManipConfig
        .def(py::init<>())
        // setters
        .def("setCropRect", static_cast<void(ImageManipConfig::*)(float, float, float, float)>(&ImageManipConfig::setCropRect), py::arg("xmin"), py::arg("ymin"), py::arg("xmax"), py::arg("xmax"), DOC(dai, ImageManipConfig, setCropRect))
        .def("setCropRect", static_cast<void(ImageManipConfig::*)(std::tuple<float, float, float, float>)>(&ImageManipConfig::setCropRect), py::arg("coordinates"), DOC(dai, ImageManipConfig, setCropRect, 2))
        .def("setCropRotatedRect", &ImageManipConfig::setCropRotatedRect, py::arg("rr"), py::arg("normalizedCoords") = true, DOC(dai, ImageManipConfig, setCropRotatedRect))
        .def("setCenterCrop", &ImageManipConfig::setCenterCrop, py::arg("ratio"), py::arg("whRatio")=1.0f, DOC(dai, ImageManipConfig, setCenterCrop))
        .def("setWarpTransformFourPoints", &ImageManipConfig::setWarpTransformFourPoints, py::arg("pt"), py::arg("normalizedCoords"), DOC(dai, ImageManipConfig, setWarpTransformFourPoints))
        .def("setWarpTransformMatrix3x3", &ImageManipConfig::setWarpTransformMatrix3x3, py::arg("mat"), DOC(dai, ImageManipConfig, setWarpTransformMatrix3x3))
        .def("setWarpBorderReplicatePixels", &ImageManipConfig::setWarpBorderReplicatePixels, DOC(dai, ImageManipConfig, setWarpBorderReplicatePixels))
        .def("setWarpBorderFillColor", &ImageManipConfig::setWarpBorderFillColor, py::arg("red"), py::arg("green"), py::arg("blue"), DOC(dai, ImageManipConfig, setWarpBorderFillColor))
        .def("setRotationDegrees", &ImageManipConfig::setRotationDegrees, py::arg("deg"), DOC(dai, ImageManipConfig, setRotationDegrees))
        .def("setRotationRadians", &ImageManipConfig::setRotationRadians, py::arg("rad"), DOC(dai, ImageManipConfig, setRotationRadians))
        .def("setResize", static_cast<void(ImageManipConfig::*)(int, int)>(&ImageManipConfig::setResize), py::arg("w"), py::arg("h"), DOC(dai, ImageManipConfig, setResize))
        .def("setResize", static_cast<void(ImageManipConfig::*)(std::tuple<int, int>)>(&ImageManipConfig::setResize), py::arg("size"), DOC(dai, ImageManipConfig, setResize, 2))
        .def("setResizeThumbnail", static_cast<void(ImageManipConfig::*)(int, int, int, int, int)>(&ImageManipConfig::setResizeThumbnail), py::arg("w"), py::arg("h"), py::arg("bgRed")=0, py::arg("bgGreen")=0, py::arg("bgBlue")=0, DOC(dai, ImageManipConfig, setResizeThumbnail))
        .def("setResizeThumbnail", static_cast<void(ImageManipConfig::*)(std::tuple<int, int>, int, int, int)>(&ImageManipConfig::setResizeThumbnail), py::arg("size"), py::arg("bgRed")=0, py::arg("bgGreen")=0, py::arg("bgBlue")=0, DOC(dai, ImageManipConfig, setResizeThumbnail, 2))
        .def("setFrameType", &ImageManipConfig::setFrameType, py::arg("name"), DOC(dai, ImageManipConfig, setFrameType))
        .def("setHorizontalFlip", &ImageManipConfig::setHorizontalFlip, py::arg("flip"), DOC(dai, ImageManipConfig, setHorizontalFlip))
        .def("setReusePreviousImage", &ImageManipConfig::setReusePreviousImage, py::arg("reuse"), DOC(dai, ImageManipConfig, setReusePreviousImage))
        .def("setSkipCurrentImage", &ImageManipConfig::setSkipCurrentImage, py::arg("skip"), DOC(dai, ImageManipConfig, setSkipCurrentImage))
        .def("setKeepAspectRatio", &ImageManipConfig::setKeepAspectRatio, py::arg("keep"), DOC(dai, ImageManipConfig, setKeepAspectRatio))

        // getters
        .def("getCropXMin", &ImageManipConfig::getCropXMin, DOC(dai, ImageManipConfig, getCropXMin))
        .def("getCropYMin", &ImageManipConfig::getCropYMin, DOC(dai, ImageManipConfig, getCropYMin))
        .def("getCropXMax", &ImageManipConfig::getCropXMax, DOC(dai, ImageManipConfig, getCropXMax))
        .def("getCropYMax", &ImageManipConfig::getCropYMax, DOC(dai, ImageManipConfig, getCropYMax))
        .def("getResizeWidth", &ImageManipConfig::getResizeWidth, DOC(dai, ImageManipConfig, getResizeWidth))
        .def("getResizeHeight", &ImageManipConfig::getResizeHeight, DOC(dai, ImageManipConfig, getResizeHeight))
        .def("getCropConfig", &ImageManipConfig::getCropConfig, DOC(dai, ImageManipConfig, getCropConfig))
        .def("getResizeConfig", &ImageManipConfig::getResizeConfig, DOC(dai, ImageManipConfig, getResizeConfig))
        .def("getFormatConfig", &ImageManipConfig::getFormatConfig, DOC(dai, ImageManipConfig, getFormatConfig))
        .def("isResizeThumbnail", &ImageManipConfig::isResizeThumbnail, DOC(dai, ImageManipConfig, isResizeThumbnail))
        ;

    // Bind CameraControl

    cameraControl
        .def(py::init<>(), DOC(dai, CameraControl, CameraControl))
        // setters
        .def("setCaptureStill", &CameraControl::setCaptureStill, py::arg("capture"), DOC(dai, CameraControl, setCaptureStill))
        .def("setStartStreaming", &CameraControl::setStartStreaming, DOC(dai, CameraControl, setStartStreaming))
        .def("setStopStreaming", &CameraControl::setStopStreaming, DOC(dai, CameraControl, setStopStreaming))
        .def("setAutoFocusMode", &CameraControl::setAutoFocusMode, py::arg("mode"), DOC(dai, CameraControl, setAutoFocusMode))
        .def("setAutoFocusTrigger", &CameraControl::setAutoFocusTrigger, DOC(dai, CameraControl, setAutoFocusTrigger))
        .def("setAutoFocusRegion", &CameraControl::setAutoFocusRegion, py::arg("startX"), py::arg("startY"), py::arg("width"), py::arg("height"), DOC(dai, CameraControl, setAutoFocusRegion))
        .def("setManualFocus", &CameraControl::setManualFocus, py::arg("lensPosition"), DOC(dai, CameraControl, setManualFocus))
        .def("setAutoExposureEnable", &CameraControl::setAutoExposureEnable, DOC(dai, CameraControl, setAutoExposureEnable))
        .def("setAutoExposureLock", &CameraControl::setAutoExposureLock, py::arg("lock"), DOC(dai, CameraControl, setAutoExposureLock))
        .def("setAutoExposureRegion", &CameraControl::setAutoExposureRegion, py::arg("startX"), py::arg("startY"), py::arg("width"), py::arg("height"), DOC(dai, CameraControl, setAutoExposureRegion))
        .def("setAutoExposureCompensation", &CameraControl::setAutoExposureCompensation, py::arg("compensation"), DOC(dai, CameraControl, setAutoExposureCompensation))
        .def("setAntiBandingMode", &CameraControl::setAntiBandingMode, py::arg("mode"), DOC(dai, CameraControl, setAntiBandingMode))
        .def("setManualExposure", &CameraControl::setManualExposure, py::arg("exposureTimeUs"), py::arg("sensitivityIso"), DOC(dai, CameraControl, setManualExposure))
        .def("setAutoWhiteBalanceMode", &CameraControl::setAutoWhiteBalanceMode, py::arg("mode"), DOC(dai, CameraControl, setAutoWhiteBalanceMode))
        .def("setAutoWhiteBalanceLock", &CameraControl::setAutoWhiteBalanceLock, py::arg("lock"), DOC(dai, CameraControl, setAutoWhiteBalanceLock))
        .def("setBrightness", &CameraControl::setBrightness, py::arg("value"), DOC(dai, CameraControl, setBrightness))
        .def("setContrast", &CameraControl::setContrast, py::arg("value"), DOC(dai, CameraControl, setContrast))
        .def("setSaturation", &CameraControl::setSaturation, py::arg("value"), DOC(dai, CameraControl, setSaturation))
        .def("setSharpness", &CameraControl::setSharpness, py::arg("value"), DOC(dai, CameraControl, setSharpness))
        .def("setLumaDenoise", &CameraControl::setLumaDenoise, py::arg("value"), DOC(dai, CameraControl, setLumaDenoise))
        .def("setChromaDenoise", &CameraControl::setChromaDenoise, py::arg("value"), DOC(dai, CameraControl, setChromaDenoise))
        .def("setSceneMode", &CameraControl::setSceneMode, py::arg("mode"), DOC(dai, CameraControl, setSceneMode))
        .def("setEffectMode", &CameraControl::setEffectMode, py::arg("mode"), DOC(dai, CameraControl, setEffectMode))
        // getters
        .def("getCaptureStill", &CameraControl::getCaptureStill, DOC(dai, CameraControl, getCaptureStill))
        ;
    // Add also enum attributes from RawCameraControl
    for (const auto& a : camCtrlAttr) {
        m.attr("CameraControl").attr(a) = m.attr("RawCameraControl").attr(a);
    }

    // Bind SystemInformation

    systemInformation
        .def(py::init<>())
        .def_property("ddrMemoryUsage", [](SystemInformation& i) { return &i.ddrMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.ddrMemoryUsage = val; } )
        .def_property("cmxMemoryUsage", [](SystemInformation& i) { return &i.cmxMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.cmxMemoryUsage = val; } )
        .def_property("leonCssMemoryUsage", [](SystemInformation& i) { return &i.leonCssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonCssMemoryUsage = val; } )
        .def_property("leonMssMemoryUsage", [](SystemInformation& i) { return &i.leonMssMemoryUsage; }, [](SystemInformation& i, MemoryInfo val) { i.leonMssMemoryUsage = val; } )
        .def_property("leonCssCpuUsage", [](SystemInformation& i) { return &i.leonCssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonCssCpuUsage = val; } )
        .def_property("leonMssCpuUsage", [](SystemInformation& i) { return &i.leonMssCpuUsage; }, [](SystemInformation& i, CpuUsage val) { i.leonMssCpuUsage = val; } )
        .def_property("chipTemperature", [](SystemInformation& i) { return &i.chipTemperature; }, [](SystemInformation& i, ChipTemperature val) { i.chipTemperature = val; } )
        ;


    spatialLocations
        .def(py::init<>())
        .def_readwrite("config", &SpatialLocations::config, DOC(dai, SpatialLocations, config))
        .def_readwrite("depthAverage", &SpatialLocations::depthAverage, DOC(dai, SpatialLocations, depthAverage))
        .def_readwrite("depthMin", &SpatialLocations::depthMin, DOC(dai, SpatialLocations, depthMin))
        .def_readwrite("depthMax", &SpatialLocations::depthMax, DOC(dai, SpatialLocations, depthMax))
        .def_readwrite("depthAveragePixelCount", &SpatialLocations::depthAveragePixelCount, DOC(dai, SpatialLocations, depthAveragePixelCount))
        .def_readwrite("spatialCoordinates", &SpatialLocations::spatialCoordinates, DOC(dai, SpatialLocations, spatialCoordinates))
        ;



    rect
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def(py::init<Point2f, Point2f>())
        .def(py::init<Point2f, Size2f>())

        .def("topLeft", &Rect::topLeft, DOC(dai, Rect, topLeft))
        .def("bottomRight", &Rect::bottomRight, DOC(dai, Rect, bottomRight))
        .def("size", &Rect::size, DOC(dai, Rect, size))
        .def("area", &Rect::area, DOC(dai, Rect, area))
        .def("empty", &Rect::empty, DOC(dai, Rect, empty))
        .def("contains", &Rect::contains, DOC(dai, Rect, contains))
        .def("isNormalized", &Rect::isNormalized, DOC(dai, Rect, isNormalized))
        .def("denormalize", &Rect::denormalize, py::arg("width"), py::arg("height"), DOC(dai, Rect, denormalize))
        .def("normalize", &Rect::normalize, py::arg("width"), py::arg("height"), DOC(dai, Rect, normalize))
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("width", &Rect::width)
        .def_readwrite("height", &Rect::height)
        ;


    spatialLocationCalculatorConfigThresholds
        .def(py::init<>())
        .def_readwrite("lowerThreshold", &SpatialLocationCalculatorConfigThresholds::lowerThreshold)
        .def_readwrite("upperThreshold", &SpatialLocationCalculatorConfigThresholds::upperThreshold)
        ;

    spatialLocationCalculatorAlgorithm
        .value("AVERAGE", SpatialLocationCalculatorAlgorithm::AVERAGE)
        .value("MIN", SpatialLocationCalculatorAlgorithm::MIN)
        .value("MAX", SpatialLocationCalculatorAlgorithm::MAX)
        ;

    spatialLocationCalculatorConfigData
        .def(py::init<>())
        .def_readwrite("roi", &SpatialLocationCalculatorConfigData::roi, DOC(dai, SpatialLocationCalculatorConfigData, roi))
        .def_readwrite("depthThresholds", &SpatialLocationCalculatorConfigData::depthThresholds, DOC(dai, SpatialLocationCalculatorConfigData, depthThresholds))
        .def_readwrite("calculationAlgorithm", &SpatialLocationCalculatorConfigData::calculationAlgorithm, DOC(dai, SpatialLocationCalculatorConfigData, calculationAlgorithm))
        ;

    // Bind SpatialLocationCalculatorData

    spatialLocationCalculatorData
        .def(py::init<>())
        .def("getSpatialLocations", &SpatialLocationCalculatorData::getSpatialLocations, DOC(dai, SpatialLocationCalculatorData, getSpatialLocations))
        .def_property("spatialLocations", [](SpatialLocationCalculatorData& loc) { return &loc.spatialLocations; }, [](SpatialLocationCalculatorData& loc, std::vector<SpatialLocations> val) { loc.spatialLocations = val; }, DOC(dai, SpatialLocationCalculatorData, spatialLocations))

        ;

    // SpatialLocationCalculatorConfig (after ConfigData)

    spatialLocationCalculatorConfig
        .def(py::init<>())
        // setters
        .def("setROIs", &SpatialLocationCalculatorConfig::setROIs, py::arg("ROIs"), DOC(dai, SpatialLocationCalculatorConfig, setROIs))
        .def("addROI", &SpatialLocationCalculatorConfig::addROI, py::arg("ROI"), DOC(dai, SpatialLocationCalculatorConfig, addROI))
        .def("getConfigData", &SpatialLocationCalculatorConfig::getConfigData, DOC(dai, SpatialLocationCalculatorConfig, getConfigData))
        ;

    // Tracklets (after ConfigData)

    tracklets
        .def(py::init<>())
        .def_property("tracklets", [](Tracklets& track) { return &track.tracklets; }, [](Tracklets& track, std::vector<Tracklet> val) { track.tracklets = val; }, DOC(dai, Tracklets, tracklets))
        ;



    imuData
        .def(py::init<>())
        .def_property("packets", [](IMUData& imuDta) { return &imuDta.packets; }, [](IMUData& imuDta, std::vector<IMUPacket> val) { imuDta.packets = val; }, DOC(dai, IMUData, packets))
        ;

    // StereoDepth config

    // MedianFilter
    medianFilter
        .value("MEDIAN_OFF", MedianFilter::MEDIAN_OFF)
        .value("KERNEL_3x3", MedianFilter::KERNEL_3x3)
        .value("KERNEL_5x5", MedianFilter::KERNEL_5x5)
        .value("KERNEL_7x7", MedianFilter::KERNEL_7x7)
        ;
    m.attr("StereoDepthProperties").attr("MedianFilter") = medianFilter;

    algorithmControl
        .def(py::init<>())
        .def_readwrite("enableLeftRightCheck", &RawStereoDepthConfig::AlgorithmControl::enableLeftRightCheck, DOC(dai, RawStereoDepthConfig, AlgorithmControl, enableLeftRightCheck))
        .def_readwrite("enableSubpixel", &RawStereoDepthConfig::AlgorithmControl::enableSubpixel, DOC(dai, RawStereoDepthConfig, AlgorithmControl, enableSubpixel))
        .def_readwrite("leftRightCheckThreshold", &RawStereoDepthConfig::AlgorithmControl::leftRightCheckThreshold, DOC(dai, RawStereoDepthConfig, AlgorithmControl, leftRightCheckThreshold))
        .def_readwrite("subpixelFractionalBits", &RawStereoDepthConfig::AlgorithmControl::subpixelFractionalBits, DOC(dai, RawStereoDepthConfig, AlgorithmControl, subpixelFractionalBits))
        ;

    postProcessing
        .def(py::init<>())
        .def_readwrite("median", &RawStereoDepthConfig::PostProcessing::median, DOC(dai, RawStereoDepthConfig, PostProcessing, median))
        .def_readwrite("bilateralSigmaValue", &RawStereoDepthConfig::PostProcessing::bilateralSigmaValue, DOC(dai, RawStereoDepthConfig, PostProcessing, bilateralSigmaValue))
        ;

    // KernelSize
    censusTransformKernelSize
        .value("AUTO", RawStereoDepthConfig::CensusTransform::KernelSize::AUTO)
        .value("KERNEL_5x5", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_5x5)
        .value("KERNEL_7x7", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x7)
        .value("KERNEL_7x9", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x9)
        ;

    censusTransform
        .def(py::init<>())
        .def_readwrite("kernelSize", &RawStereoDepthConfig::CensusTransform::kernelSize, DOC(dai, RawStereoDepthConfig, CensusTransform, kernelSize))
        .def_readwrite("kernelMask", &RawStereoDepthConfig::CensusTransform::kernelMask, DOC(dai, RawStereoDepthConfig, CensusTransform, kernelMask))
        .def_readwrite("enableMeanMode", &RawStereoDepthConfig::CensusTransform::enableMeanMode, DOC(dai, RawStereoDepthConfig, CensusTransform, enableMeanMode))
        .def_readwrite("threshold", &RawStereoDepthConfig::CensusTransform::threshold, DOC(dai, RawStereoDepthConfig, CensusTransform, threshold))
        ;

    costMatchingLinearEquationParameters
        .def(py::init<>())
        .def_readwrite("alpha", &RawStereoDepthConfig::CostMatching::LinearEquationParameters::alpha, DOC(dai, RawStereoDepthConfig, CostMatching, LinearEquationParameters, alpha))
        .def_readwrite("beta", &RawStereoDepthConfig::CostMatching::LinearEquationParameters::beta, DOC(dai, RawStereoDepthConfig, CostMatching, LinearEquationParameters, beta))
        .def_readwrite("threshold", &RawStereoDepthConfig::CostMatching::LinearEquationParameters::threshold, DOC(dai, RawStereoDepthConfig, CostMatching, LinearEquationParameters, threshold))
        ;

    // Disparity width
    costMatchingDisparityWidth
        .value("DISPARITY_64", RawStereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_64)
        .value("DISPARITY_96", RawStereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_96)
        ;

    costMatching
        .def(py::init<>())
        .def_readwrite("disparityWidth", &RawStereoDepthConfig::CostMatching::disparityWidth, DOC(dai, RawStereoDepthConfig, CostMatching, disparityWidth))
        .def_readwrite("enableCompanding", &RawStereoDepthConfig::CostMatching::enableCompanding, DOC(dai, RawStereoDepthConfig, CostMatching, enableCompanding))
        .def_readwrite("invalidDisparityValue", &RawStereoDepthConfig::CostMatching::invalidDisparityValue, DOC(dai, RawStereoDepthConfig, CostMatching, invalidDisparityValue))
        .def_readwrite("confidenceThreshold", &RawStereoDepthConfig::CostMatching::confidenceThreshold, DOC(dai, RawStereoDepthConfig, CostMatching, confidenceThreshold))
        .def_readwrite("linearEquationParameters", &RawStereoDepthConfig::CostMatching::linearEquationParameters, DOC(dai, RawStereoDepthConfig, CostMatching, linearEquationParameters))
        ;

    costAggregation
        .def(py::init<>())
        .def_readwrite("divisionFactor", &RawStereoDepthConfig::CostAggregation::divisionFactor, DOC(dai, RawStereoDepthConfig, CostAggregation, divisionFactor))
        .def_readwrite("horizontalPenaltyCosts", &RawStereoDepthConfig::CostAggregation::horizontalPenaltyCosts, DOC(dai, RawStereoDepthConfig, CostAggregation, horizontalPenaltyCosts))
        .def_readwrite("verticalPenaltyCosts", &RawStereoDepthConfig::CostAggregation::verticalPenaltyCosts, DOC(dai, RawStereoDepthConfig, CostAggregation, verticalPenaltyCosts))
        ;

    rawStereoDepthConfig
        .def(py::init<>())
        .def_readwrite("algorithmControl", &RawStereoDepthConfig::algorithmControl,  DOC(dai, RawStereoDepthConfig, algorithmControl))
        .def_readwrite("postProcessing", &RawStereoDepthConfig::postProcessing,  DOC(dai, RawStereoDepthConfig, postProcessing))
        .def_readwrite("censusTransform", &RawStereoDepthConfig::censusTransform,  DOC(dai, RawStereoDepthConfig, censusTransform))
        .def_readwrite("costMatching", &RawStereoDepthConfig::costMatching,  DOC(dai, RawStereoDepthConfig, costMatching))
        .def_readwrite("costAggregation", &RawStereoDepthConfig::costAggregation,  DOC(dai, RawStereoDepthConfig, costAggregation))
        ;
    // legacy
    m.attr("RawStereoDepthConfig").attr("MedianFilter") = medianFilter;



    stereoDepthConfig
        .def(py::init<>())
        .def("setConfidenceThreshold",  &StereoDepthConfig::setConfidenceThreshold, py::arg("confThr"), DOC(dai, StereoDepthConfig, setConfidenceThreshold))
        .def("setMedianFilter",         &StereoDepthConfig::setMedianFilter, py::arg("median"), DOC(dai, StereoDepthConfig, setMedianFilter))
        .def("setBilateralFilterSigma", &StereoDepthConfig::setBilateralFilterSigma, py::arg("sigma"), DOC(dai, StereoDepthConfig, setBilateralFilterSigma))
        .def("setLeftRightCheckThreshold", &StereoDepthConfig::setLeftRightCheckThreshold, py::arg("sigma"), DOC(dai, StereoDepthConfig, setLeftRightCheckThreshold))
        .def("getConfidenceThreshold",  &StereoDepthConfig::getConfidenceThreshold, DOC(dai, StereoDepthConfig, getConfidenceThreshold))
        .def("getMedianFilter",         &StereoDepthConfig::getMedianFilter, DOC(dai, StereoDepthConfig, getMedianFilter))
        .def("getBilateralFilterSigma", &StereoDepthConfig::getBilateralFilterSigma, DOC(dai, StereoDepthConfig, getBilateralFilterSigma))
        .def("getLeftRightCheckThreshold",         &StereoDepthConfig::getLeftRightCheckThreshold, DOC(dai, StereoDepthConfig, getLeftRightCheckThreshold))
        .def("setLeftRightCheck",       &StereoDepthConfig::setLeftRightCheck, DOC(dai, StereoDepthConfig, setLeftRightCheck))
        .def("setSubpixel",             &StereoDepthConfig::setSubpixel, DOC(dai, StereoDepthConfig, setSubpixel))
        .def("getMaxDisparity",         &StereoDepthConfig::getMaxDisparity, DOC(dai, StereoDepthConfig, getMaxDisparity))
        .def("set",                     &StereoDepthConfig::set, py::arg("config"), DOC(dai, StereoDepthConfig, set))
        .def("get",                     &StereoDepthConfig::get, DOC(dai, StereoDepthConfig, get))
        ;

    edgeDetectorConfigData
        .def(py::init<>())
        .def_readwrite("sobelFilterHorizontalKernel", &EdgeDetectorConfigData::sobelFilterHorizontalKernel, DOC(dai, EdgeDetectorConfigData, sobelFilterHorizontalKernel))
        .def_readwrite("sobelFilterVerticalKernel", &EdgeDetectorConfigData::sobelFilterVerticalKernel, DOC(dai, EdgeDetectorConfigData, sobelFilterVerticalKernel))
        ;


    rawEdgeDetectorConfig
        .def(py::init<>())
        .def_readwrite("config", &RawEdgeDetectorConfig::config)
        ;


    edgeDetectorConfig
        .def(py::init<>())
        .def("setSobelFilterKernels",  &EdgeDetectorConfig::setSobelFilterKernels, py::arg("horizontalKernel"), py::arg("verticalKernel"), DOC(dai, EdgeDetectorConfig, setSobelFilterKernels))
        .def("getConfigData",         &EdgeDetectorConfig::getConfigData, DOC(dai, EdgeDetectorConfig, getConfigData))
        ;

    // Bind RawTrackedFeatures
    rawTrackedFeatures
        .def(py::init<>())
        .def_readwrite("trackedFeatures", &RawTrackedFeatures::trackedFeatures)
        ;

    trackedFeature
        .def(py::init<>())
        .def_readwrite("position", &TrackedFeature::position, DOC(dai, TrackedFeature, position))
        .def_readwrite("id", &TrackedFeature::id, DOC(dai, TrackedFeature, id))
#if 0
        .def_readwrite("age", &TrackedFeature::age, DOC(dai, TrackedFeature, age))
        .def_readwrite("harrisScore", &TrackedFeature::harrisScore, DOC(dai, TrackedFeature, harrisScore))
        .def_readwrite("trackingError", &TrackedFeature::trackingError, DOC(dai, TrackedFeature, trackingError))
#endif
        ;



    rawFeatureTrackerConfig
        .def(py::init<>())
        .def_readwrite("cornerDetector", &RawFeatureTrackerConfig::cornerDetector, DOC(dai, RawFeatureTrackerConfig, cornerDetector))
        .def_readwrite("motionEstimator", &RawFeatureTrackerConfig::motionEstimator, DOC(dai, RawFeatureTrackerConfig, motionEstimator))
        .def_readwrite("featureMaintainer", &RawFeatureTrackerConfig::featureMaintainer, DOC(dai, RawFeatureTrackerConfig, featureMaintainer))
        ;

    cornerDetectorType
        .value("HARRIS", RawFeatureTrackerConfig::CornerDetector::Type::HARRIS)
        .value("SHI_THOMASI", RawFeatureTrackerConfig::CornerDetector::Type::SHI_THOMASI)
    ;

    cornerDetectorThresholds
        .def(py::init<>())
        .def_readwrite("initialValue", &RawFeatureTrackerConfig::CornerDetector::Thresholds::initialValue, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, initialValue))
        .def_readwrite("min", &RawFeatureTrackerConfig::CornerDetector::Thresholds::min, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, min))
        .def_readwrite("max", &RawFeatureTrackerConfig::CornerDetector::Thresholds::max, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, max))
        .def_readwrite("decreaseFactor", &RawFeatureTrackerConfig::CornerDetector::Thresholds::decreaseFactor, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, decreaseFactor))
        .def_readwrite("increaseFactor", &RawFeatureTrackerConfig::CornerDetector::Thresholds::increaseFactor, DOC(dai, RawFeatureTrackerConfig, CornerDetector, Thresholds, increaseFactor))
        ;

    cornerDetector
        .def(py::init<>())
        .def_readwrite("type", &RawFeatureTrackerConfig::CornerDetector::type, DOC(dai, RawFeatureTrackerConfig, CornerDetector, type))
        .def_readwrite("cellGridDimension", &RawFeatureTrackerConfig::CornerDetector::cellGridDimension, DOC(dai, RawFeatureTrackerConfig, CornerDetector, cellGridDimension))
        .def_readwrite("numTargetFeatures", &RawFeatureTrackerConfig::CornerDetector::numTargetFeatures, DOC(dai, RawFeatureTrackerConfig, CornerDetector, numTargetFeatures))
        .def_readwrite("numMaxFeatures", &RawFeatureTrackerConfig::CornerDetector::numMaxFeatures, DOC(dai, RawFeatureTrackerConfig, CornerDetector, numMaxFeatures))
        .def_readwrite("enableSobel", &RawFeatureTrackerConfig::CornerDetector::enableSobel, DOC(dai, RawFeatureTrackerConfig, CornerDetector, enableSobel))
        .def_readwrite("enableSorting", &RawFeatureTrackerConfig::CornerDetector::enableSorting, DOC(dai, RawFeatureTrackerConfig, CornerDetector, enableSorting))
        .def_readwrite("thresholds", &RawFeatureTrackerConfig::CornerDetector::thresholds, DOC(dai, RawFeatureTrackerConfig, CornerDetector, thresholds))
        ;

    motionEstimatorType
        .value("LUCAS_KANADE_OPTICAL_FLOW", RawFeatureTrackerConfig::MotionEstimator::Type::LUCAS_KANADE_OPTICAL_FLOW)
        .value("HW_MOTION_ESTIMATION", RawFeatureTrackerConfig::MotionEstimator::Type::HW_MOTION_ESTIMATION)
    ;

    motionEstimatorOpticalFlow
        .def(py::init<>())
        .def_readwrite("pyramidLevels", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::pyramidLevels, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, pyramidLevels))
        .def_readwrite("searchWindowWidth", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowWidth, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowWidth))
        .def_readwrite("searchWindowHeight", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::searchWindowHeight, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, searchWindowHeight))
        .def_readwrite("epsilon", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::epsilon, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, epsilon))
        .def_readwrite("maxIterations", &RawFeatureTrackerConfig::MotionEstimator::OpticalFlow::maxIterations, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, OpticalFlow, maxIterations))
        ;

    motionEstimator
        .def(py::init<>())
        .def_readwrite("enable", &RawFeatureTrackerConfig::MotionEstimator::enable, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, enable))
        .def_readwrite("type", &RawFeatureTrackerConfig::MotionEstimator::type, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, type))
        .def_readwrite("opticalFlow", &RawFeatureTrackerConfig::MotionEstimator::opticalFlow, DOC(dai, RawFeatureTrackerConfig, MotionEstimator, opticalFlow))
        ;

    featureMaintainer
        .def(py::init<>())
        .def_readwrite("enable", &RawFeatureTrackerConfig::FeatureMaintainer::enable, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, enable))
        .def_readwrite("minimumDistanceBetweenFeatures", &RawFeatureTrackerConfig::FeatureMaintainer::minimumDistanceBetweenFeatures, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, minimumDistanceBetweenFeatures))
        .def_readwrite("lostFeatureErrorThreshold", &RawFeatureTrackerConfig::FeatureMaintainer::lostFeatureErrorThreshold, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, lostFeatureErrorThreshold))
        .def_readwrite("trackedFeatureThreshold", &RawFeatureTrackerConfig::FeatureMaintainer::trackedFeatureThreshold, DOC(dai, RawFeatureTrackerConfig, FeatureMaintainer, trackedFeatureThreshold))
        ;


    // Bind TrackedFeatures
    py::class_<TrackedFeatures, Buffer, std::shared_ptr<TrackedFeatures>>(m, "TrackedFeatures", DOC(dai, TrackedFeatures))
        .def(py::init<>())
        .def_property("trackedFeatures", [](TrackedFeatures& feat) { return &feat.trackedFeatures; }, [](TrackedFeatures& feat, std::vector<TrackedFeature> val) { feat.trackedFeatures = val; }, DOC(dai, TrackedFeatures, trackedFeatures))
        ;

    // FeatureTrackerConfig (after ConfigData)
    featureTrackerConfig
        .def(py::init<>())
        .def(py::init<std::shared_ptr<RawFeatureTrackerConfig>>())

        .def("setCornerDetector", static_cast<void(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::CornerDetector::Type)>(&FeatureTrackerConfig::setCornerDetector), py::arg("cornerDetector"), DOC(dai, FeatureTrackerConfig, setCornerDetector))
        .def("setCornerDetector", static_cast<void(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::CornerDetector)>(&FeatureTrackerConfig::setCornerDetector), py::arg("config"), DOC(dai, FeatureTrackerConfig, setCornerDetector, 2))
        .def("setMotionEstimator", static_cast<void(FeatureTrackerConfig::*)(bool)>(&FeatureTrackerConfig::setMotionEstimator), py::arg("enable"), DOC(dai, FeatureTrackerConfig, setMotionEstimator))
        .def("setMotionEstimator", static_cast<void(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::MotionEstimator)>(&FeatureTrackerConfig::setMotionEstimator), py::arg("config"), DOC(dai, FeatureTrackerConfig, setMotionEstimator, 2))
        .def("setOpticalFlow", static_cast<void(FeatureTrackerConfig::*)()>(&FeatureTrackerConfig::setOpticalFlow), DOC(dai, FeatureTrackerConfig, setOpticalFlow))
        .def("setOpticalFlow", static_cast<void(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::MotionEstimator::OpticalFlow)>(&FeatureTrackerConfig::setOpticalFlow), py::arg("config"), DOC(dai, FeatureTrackerConfig, setOpticalFlow, 2))
        .def("setHwMotionEstimation", &FeatureTrackerConfig::setHwMotionEstimation, DOC(dai, FeatureTrackerConfig, setHwMotionEstimation))
        .def("setFeatureMaintainer", static_cast<void(FeatureTrackerConfig::*)(bool)>(&FeatureTrackerConfig::setFeatureMaintainer), py::arg("enable"), DOC(dai, FeatureTrackerConfig, setFeatureMaintainer))
        .def("setFeatureMaintainer", static_cast<void(FeatureTrackerConfig::*)(dai::FeatureTrackerConfig::FeatureMaintainer)>(&FeatureTrackerConfig::setFeatureMaintainer), py::arg("config"), DOC(dai, FeatureTrackerConfig, setFeatureMaintainer, 2))
        .def("setNumTargetFeatures", &FeatureTrackerConfig::setNumTargetFeatures, py::arg("numTargetFeatures"), DOC(dai, FeatureTrackerConfig, setNumTargetFeatures))

        .def("set", &FeatureTrackerConfig::set, py::arg("config"), DOC(dai, FeatureTrackerConfig, set))
        .def("get", &FeatureTrackerConfig::get, DOC(dai, FeatureTrackerConfig, get))
        ;

    // add aliases
    m.attr("FeatureTrackerConfig").attr("CornerDetector") = m.attr("RawFeatureTrackerConfig").attr("CornerDetector");
    m.attr("FeatureTrackerConfig").attr("MotionEstimator") = m.attr("RawFeatureTrackerConfig").attr("MotionEstimator");
    m.attr("FeatureTrackerConfig").attr("FeatureMaintainer") = m.attr("RawFeatureTrackerConfig").attr("FeatureMaintainer");

}
