#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/StereoDepthConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_stereodepthconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawStereoDepthConfig, RawBuffer, std::shared_ptr<RawStereoDepthConfig>> rawStereoDepthConfig(m, "RawStereoDepthConfig", DOC(dai, RawStereoDepthConfig));
    py::class_<StereoDepthConfig, Buffer, std::shared_ptr<StereoDepthConfig>> stereoDepthConfig(m, "StereoDepthConfig", DOC(dai, StereoDepthConfig));
    py::enum_<StereoDepthConfig::MedianFilter> medianFilter(m, "MedianFilter", DOC(dai, MedianFilter));
    py::class_<StereoDepthConfig::AlgorithmControl> algorithmControl(stereoDepthConfig, "AlgorithmControl", DOC(dai, StereoDepthConfig, AlgorithmControl));
    py::enum_<StereoDepthConfig::AlgorithmControl::DepthAlign> depthAlign(algorithmControl, "DepthAlign", DOC(dai, StereoDepthConfig, AlgorithmControl, DepthAlign));
    py::enum_<StereoDepthConfig::AlgorithmControl::DepthUnit> depthUnit(algorithmControl, "DepthUnit", DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit));
    py::class_<StereoDepthConfig::PostProcessing> postProcessing(stereoDepthConfig, "PostProcessing", DOC(dai, StereoDepthConfig, PostProcessing));
    py::class_<StereoDepthConfig::PostProcessing::SpatialFilter> spatialFilter(postProcessing, "SpatialFilter", DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter));
    py::class_<StereoDepthConfig::PostProcessing::TemporalFilter> temporalFilter(postProcessing, "TemporalFilter", DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter));
    py::enum_<StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode> persistencyMode(temporalFilter, "PersistencyMode", DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode));
    py::class_<StereoDepthConfig::PostProcessing::ThresholdFilter> thresholdFilter(postProcessing, "ThresholdFilter", DOC(dai, StereoDepthConfig, PostProcessing, ThresholdFilter));
    py::class_<StereoDepthConfig::PostProcessing::BrightnessFilter> brightnessFilter(postProcessing, "BrightnessFilter", DOC(dai, StereoDepthConfig, PostProcessing, BrightnessFilter));
    py::class_<StereoDepthConfig::PostProcessing::SpeckleFilter> speckleFilter(postProcessing, "SpeckleFilter", DOC(dai, StereoDepthConfig, PostProcessing, SpeckleFilter));
    py::class_<StereoDepthConfig::PostProcessing::DecimationFilter> decimationFilter(postProcessing, "DecimationFilter", DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter));
    py::enum_<StereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode> decimationMode(decimationFilter, "DecimationMode", DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode));
    py::class_<StereoDepthConfig::CostAggregation> costAggregation(stereoDepthConfig, "CostAggregation", DOC(dai, StereoDepthConfig, CostAggregation));
    py::enum_<StereoDepthConfig::CostAggregation::LocalAggregationMode> localAggregationMode(costAggregation, "LocalAggregationMode", DOC(dai, StereoDepthConfig, CostAggregation, LocalAggregationMode));
    py::class_<StereoDepthConfig::CostMatching> costMatching(stereoDepthConfig, "CostMatching", DOC(dai, StereoDepthConfig, CostMatching));
    py::class_<StereoDepthConfig::CostMatching::LinearEquationParameters> costMatchingLinearEquationParameters(costMatching, "LinearEquationParameters", DOC(dai, StereoDepthConfig, CostMatching, LinearEquationParameters));
    py::enum_<StereoDepthConfig::CostMatching::DisparityWidth> costMatchingDisparityWidth(costMatching, "DisparityWidth", DOC(dai, StereoDepthConfig, CostMatching, DisparityWidth));
    py::class_<StereoDepthConfig::CensusTransform> censusTransform(stereoDepthConfig, "CensusTransform", DOC(dai, StereoDepthConfig, CensusTransform));
    py::enum_<StereoDepthConfig::CensusTransform::KernelSize> censusTransformKernelSize(censusTransform, "KernelSize", DOC(dai, StereoDepthConfig, CensusTransform, KernelSize));

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
    medianFilter
        .value("MEDIAN_OFF", StereoDepthConfig::MedianFilter::MEDIAN_OFF)
        .value("KERNEL_3x3", StereoDepthConfig::MedianFilter::KERNEL_3x3)
        .value("KERNEL_5x5", StereoDepthConfig::MedianFilter::KERNEL_5x5)
        .value("KERNEL_7x7", StereoDepthConfig::MedianFilter::KERNEL_7x7)
        ;
    m.attr("StereoDepthProperties").attr("MedianFilter") = medianFilter;


    depthAlign
        .value("RECTIFIED_RIGHT", StereoDepthConfig::AlgorithmControl::DepthAlign::RECTIFIED_RIGHT, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthAlign, RECTIFIED_RIGHT))
        .value("RECTIFIED_LEFT", StereoDepthConfig::AlgorithmControl::DepthAlign::RECTIFIED_LEFT, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthAlign, RECTIFIED_LEFT))
        .value("CENTER", StereoDepthConfig::AlgorithmControl::DepthAlign::CENTER, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthAlign, CENTER));

    m.attr("StereoDepthProperties").attr("DepthAlign") = depthAlign;

    depthUnit
        .value("METER", StereoDepthConfig::AlgorithmControl::DepthUnit::METER, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, METER))
        .value("CENTIMETER", StereoDepthConfig::AlgorithmControl::DepthUnit::CENTIMETER, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, CENTIMETER))
        .value("MILLIMETER", StereoDepthConfig::AlgorithmControl::DepthUnit::MILLIMETER, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, MILLIMETER))
        .value("INCH", StereoDepthConfig::AlgorithmControl::DepthUnit::INCH, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, INCH))
        .value("FOOT", StereoDepthConfig::AlgorithmControl::DepthUnit::FOOT, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, FOOT))
        .value("CUSTOM", StereoDepthConfig::AlgorithmControl::DepthUnit::CUSTOM, DOC(dai, StereoDepthConfig, AlgorithmControl, DepthUnit, CUSTOM))
        ;

    algorithmControl
        .def(py::init<>())
        .def_readwrite("depthAlign", &StereoDepthConfig::AlgorithmControl::depthAlign, DOC(dai, StereoDepthConfig, AlgorithmControl, depthAlign))
        .def_readwrite("depthUnit", &StereoDepthConfig::AlgorithmControl::depthUnit, DOC(dai, StereoDepthConfig, AlgorithmControl, depthUnit))
        .def_readwrite("customDepthUnitMultiplier", &StereoDepthConfig::AlgorithmControl::customDepthUnitMultiplier, DOC(dai, StereoDepthConfig, AlgorithmControl, customDepthUnitMultiplier))
        .def_readwrite("enableLeftRightCheck", &StereoDepthConfig::AlgorithmControl::enableLeftRightCheck, DOC(dai, StereoDepthConfig, AlgorithmControl, enableLeftRightCheck))
        .def_readwrite("enableExtended", &StereoDepthConfig::AlgorithmControl::enableExtended, DOC(dai, StereoDepthConfig, AlgorithmControl, enableExtended))
        .def_readwrite("enableSubpixel", &StereoDepthConfig::AlgorithmControl::enableSubpixel, DOC(dai, StereoDepthConfig, AlgorithmControl, enableSubpixel))
        .def_readwrite("leftRightCheckThreshold", &StereoDepthConfig::AlgorithmControl::leftRightCheckThreshold, DOC(dai, StereoDepthConfig, AlgorithmControl, leftRightCheckThreshold))
        .def_readwrite("subpixelFractionalBits", &StereoDepthConfig::AlgorithmControl::subpixelFractionalBits, DOC(dai, StereoDepthConfig, AlgorithmControl, subpixelFractionalBits))
        .def_readwrite("disparityShift", &StereoDepthConfig::AlgorithmControl::disparityShift, DOC(dai, StereoDepthConfig, AlgorithmControl, disparityShift))
        .def_readwrite("replaceInvalidDisparity", &StereoDepthConfig::AlgorithmControl::replaceInvalidDisparity, DOC(dai, StereoDepthConfig, AlgorithmControl, replaceInvalidDisparity))
        .def_readwrite("outlierRemoveThreshold", &StereoDepthConfig::AlgorithmControl::outlierRemoveThreshold, DOC(dai, StereoDepthConfig, AlgorithmControl, outlierRemoveThreshold))
        .def_readwrite("outlierCensusThreshold", &StereoDepthConfig::AlgorithmControl::outlierCensusThreshold, DOC(dai, StereoDepthConfig, AlgorithmControl, outlierCensusThreshold))
        .def_readwrite("outlierDiffThreshold", &StereoDepthConfig::AlgorithmControl::outlierDiffThreshold, DOC(dai, StereoDepthConfig, AlgorithmControl, outlierDiffThreshold))
        .def_readwrite("centerAlignmentShiftFactor", &StereoDepthConfig::AlgorithmControl::centerAlignmentShiftFactor, DOC(dai, StereoDepthConfig, AlgorithmControl, centerAlignmentShiftFactor))
        .def_readwrite("numInvalidateEdgePixels", &StereoDepthConfig::AlgorithmControl::numInvalidateEdgePixels, DOC(dai, StereoDepthConfig, AlgorithmControl, numInvalidateEdgePixels))
        ;

    spatialFilter
        .def(py::init<>())
        .def_readwrite("enable", &StereoDepthConfig::PostProcessing::SpatialFilter::enable, DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter, enable))
        .def_readwrite("holeFillingRadius", &StereoDepthConfig::PostProcessing::SpatialFilter::holeFillingRadius, DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter, holeFillingRadius))
        .def_readwrite("alpha", &StereoDepthConfig::PostProcessing::SpatialFilter::alpha, DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter, alpha))
        .def_readwrite("delta", &StereoDepthConfig::PostProcessing::SpatialFilter::delta, DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter, delta))
        .def_readwrite("numIterations", &StereoDepthConfig::PostProcessing::SpatialFilter::numIterations, DOC(dai, StereoDepthConfig, PostProcessing, SpatialFilter, numIterations))
        ;

    persistencyMode
        .value("PERSISTENCY_OFF", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::PERSISTENCY_OFF, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, PERSISTENCY_OFF))
        .value("VALID_8_OUT_OF_8", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_8_OUT_OF_8, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_8_OUT_OF_8))
        .value("VALID_2_IN_LAST_3", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_IN_LAST_3, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_IN_LAST_3))
        .value("VALID_2_IN_LAST_4", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_IN_LAST_4, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_IN_LAST_4))
        .value("VALID_2_OUT_OF_8", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_OUT_OF_8, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_OUT_OF_8))
        .value("VALID_1_IN_LAST_2", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_2, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_2))
        .value("VALID_1_IN_LAST_5", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_5, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_5))
        .value("VALID_1_IN_LAST_8", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_8, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_8))
        .value("PERSISTENCY_INDEFINITELY", StereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::PERSISTENCY_INDEFINITELY, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, PERSISTENCY_INDEFINITELY))
        ;

    temporalFilter
        .def(py::init<>())
        .def_readwrite("enable", &StereoDepthConfig::PostProcessing::TemporalFilter::enable, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, enable))
        .def_readwrite("persistencyMode", &StereoDepthConfig::PostProcessing::TemporalFilter::persistencyMode, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, persistencyMode))
        .def_readwrite("alpha", &StereoDepthConfig::PostProcessing::TemporalFilter::alpha, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, alpha))
        .def_readwrite("delta", &StereoDepthConfig::PostProcessing::TemporalFilter::delta, DOC(dai, StereoDepthConfig, PostProcessing, TemporalFilter, delta))
        ;

    thresholdFilter
        .def(py::init<>())
        .def_readwrite("minRange", &StereoDepthConfig::PostProcessing::ThresholdFilter::minRange, DOC(dai, StereoDepthConfig, PostProcessing, ThresholdFilter, minRange))
        .def_readwrite("maxRange", &StereoDepthConfig::PostProcessing::ThresholdFilter::maxRange, DOC(dai, StereoDepthConfig, PostProcessing, ThresholdFilter, maxRange))
        ;

    brightnessFilter
        .def(py::init<>())
        .def_readwrite("minBrightness", &StereoDepthConfig::PostProcessing::BrightnessFilter::minBrightness, DOC(dai, StereoDepthConfig, PostProcessing, BrightnessFilter, minBrightness))
        .def_readwrite("maxBrightness", &StereoDepthConfig::PostProcessing::BrightnessFilter::maxBrightness, DOC(dai, StereoDepthConfig, PostProcessing, BrightnessFilter, maxBrightness))
        ;

    speckleFilter
        .def(py::init<>())
        .def_readwrite("enable", &StereoDepthConfig::PostProcessing::SpeckleFilter::enable, DOC(dai, StereoDepthConfig, PostProcessing, SpeckleFilter, enable))
        .def_readwrite("speckleRange", &StereoDepthConfig::PostProcessing::SpeckleFilter::speckleRange, DOC(dai, StereoDepthConfig, PostProcessing, SpeckleFilter, speckleRange))
        ;

    decimationMode
        .value("PIXEL_SKIPPING", StereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::PIXEL_SKIPPING, DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, PIXEL_SKIPPING))
        .value("NON_ZERO_MEDIAN", StereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::NON_ZERO_MEDIAN, DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, NON_ZERO_MEDIAN))
        .value("NON_ZERO_MEAN", StereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::NON_ZERO_MEAN, DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, NON_ZERO_MEAN))
        ;

    decimationFilter
        .def(py::init<>())
        .def_readwrite("decimationFactor", &StereoDepthConfig::PostProcessing::DecimationFilter::decimationFactor, DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, decimationFactor))
        .def_readwrite("decimationMode", &StereoDepthConfig::PostProcessing::DecimationFilter::decimationMode, DOC(dai, StereoDepthConfig, PostProcessing, DecimationFilter, decimationMode))
        ;

    postProcessing
        .def(py::init<>())
        .def_readwrite("median", &StereoDepthConfig::PostProcessing::median, DOC(dai, StereoDepthConfig, PostProcessing, median))
        .def_readwrite("bilateralSigmaValue", &StereoDepthConfig::PostProcessing::bilateralSigmaValue, DOC(dai, StereoDepthConfig, PostProcessing, bilateralSigmaValue))
        .def_readwrite("spatialFilter", &StereoDepthConfig::PostProcessing::spatialFilter, DOC(dai, StereoDepthConfig, PostProcessing, spatialFilter))
        .def_readwrite("temporalFilter", &StereoDepthConfig::PostProcessing::temporalFilter, DOC(dai, StereoDepthConfig, PostProcessing, temporalFilter))
        .def_readwrite("thresholdFilter", &StereoDepthConfig::PostProcessing::thresholdFilter, DOC(dai, StereoDepthConfig, PostProcessing, thresholdFilter))
        .def_readwrite("brightnessFilter", &StereoDepthConfig::PostProcessing::brightnessFilter, DOC(dai, StereoDepthConfig, PostProcessing, brightnessFilter))
        .def_readwrite("speckleFilter", &StereoDepthConfig::PostProcessing::speckleFilter, DOC(dai, StereoDepthConfig, PostProcessing, speckleFilter))
        .def_readwrite("decimationFilter", &StereoDepthConfig::PostProcessing::decimationFilter, DOC(dai, StereoDepthConfig, PostProcessing, decimationFilter))
        ;

    // KernelSize
    censusTransformKernelSize
        .value("AUTO", StereoDepthConfig::CensusTransform::KernelSize::AUTO, DOC(dai, StereoDepthConfig, CensusTransform, KernelSize, AUTO))
        .value("KERNEL_5x5", StereoDepthConfig::CensusTransform::KernelSize::KERNEL_5x5, DOC(dai, StereoDepthConfig, CensusTransform, KernelSize, KERNEL_5x5))
        .value("KERNEL_7x7", StereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x7, DOC(dai, StereoDepthConfig, CensusTransform, KernelSize, KERNEL_7x7))
        .value("KERNEL_7x9", StereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x9, DOC(dai, StereoDepthConfig, CensusTransform, KernelSize, KERNEL_7x9))
        ;

    censusTransform
        .def(py::init<>())
        .def_readwrite("kernelSize", &StereoDepthConfig::CensusTransform::kernelSize, DOC(dai, StereoDepthConfig, CensusTransform, kernelSize))
        .def_readwrite("kernelMask", &StereoDepthConfig::CensusTransform::kernelMask, DOC(dai, StereoDepthConfig, CensusTransform, kernelMask))
        .def_readwrite("enableMeanMode", &StereoDepthConfig::CensusTransform::enableMeanMode, DOC(dai, StereoDepthConfig, CensusTransform, enableMeanMode))
        .def_readwrite("threshold", &StereoDepthConfig::CensusTransform::threshold, DOC(dai, StereoDepthConfig, CensusTransform, threshold))
        ;

    costMatchingLinearEquationParameters
        .def(py::init<>())
        .def_readwrite("alpha", &StereoDepthConfig::CostMatching::LinearEquationParameters::alpha, DOC(dai, StereoDepthConfig, CostMatching, LinearEquationParameters, alpha))
        .def_readwrite("beta", &StereoDepthConfig::CostMatching::LinearEquationParameters::beta, DOC(dai, StereoDepthConfig, CostMatching, LinearEquationParameters, beta))
        .def_readwrite("threshold", &StereoDepthConfig::CostMatching::LinearEquationParameters::threshold, DOC(dai, StereoDepthConfig, CostMatching, LinearEquationParameters, threshold))
        ;

    // Disparity width
    costMatchingDisparityWidth
        .value("DISPARITY_64", StereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_64, DOC(dai, StereoDepthConfig, CostMatching, DisparityWidth, DISPARITY_64))
        .value("DISPARITY_96", StereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_96, DOC(dai, StereoDepthConfig, CostMatching, DisparityWidth, DISPARITY_96))
        ;

    costMatching
        .def(py::init<>())
        .def_readwrite("disparityWidth", &StereoDepthConfig::CostMatching::disparityWidth, DOC(dai, StereoDepthConfig, CostMatching, disparityWidth))
        .def_readwrite("enableCompanding", &StereoDepthConfig::CostMatching::enableCompanding, DOC(dai, StereoDepthConfig, CostMatching, enableCompanding))
        .def_readwrite("invalidDisparityValue", &StereoDepthConfig::CostMatching::invalidDisparityValue, DOC(dai, StereoDepthConfig, CostMatching, invalidDisparityValue))
        .def_readwrite("confidenceThreshold", &StereoDepthConfig::CostMatching::confidenceThreshold, DOC(dai, StereoDepthConfig, CostMatching, confidenceThreshold))
        .def_readwrite("linearEquationParameters", &StereoDepthConfig::CostMatching::linearEquationParameters, DOC(dai, StereoDepthConfig, CostMatching, linearEquationParameters))
        ;

    // KernelSize
    localAggregationMode
        .value("AVG3x3", StereoDepthConfig::CostAggregation::LocalAggregationMode::AVG3x3, DOC(dai, StereoDepthConfig, CostAggregation, LocalAggregationMode, AVG3x3))
        .value("CLAMP3x3", StereoDepthConfig::CostAggregation::LocalAggregationMode::CLAMP3x3, DOC(dai, StereoDepthConfig, CostAggregation, LocalAggregationMode, CLAMP3x3))
        .value("PASS3x3", StereoDepthConfig::CostAggregation::LocalAggregationMode::PASS3x3, DOC(dai, StereoDepthConfig, CostAggregation, LocalAggregationMode, PASS3x3))
        ;

    costAggregation
        .def(py::init<>())
        .def_readwrite("divisionFactor", &StereoDepthConfig::CostAggregation::divisionFactor, DOC(dai, StereoDepthConfig, CostAggregation, divisionFactor))
        .def_readwrite("horizontalPenaltyCostP1", &StereoDepthConfig::CostAggregation::horizontalPenaltyCostP1, DOC(dai, StereoDepthConfig, CostAggregation, horizontalPenaltyCostP1))
        .def_readwrite("horizontalPenaltyCostP2", &StereoDepthConfig::CostAggregation::horizontalPenaltyCostP2, DOC(dai, StereoDepthConfig, CostAggregation, horizontalPenaltyCostP2))
        .def_readwrite("verticalPenaltyCostP1", &StereoDepthConfig::CostAggregation::verticalPenaltyCostP1, DOC(dai, StereoDepthConfig, CostAggregation, verticalPenaltyCostP1))
        .def_readwrite("verticalPenaltyCostP2", &StereoDepthConfig::CostAggregation::verticalPenaltyCostP2, DOC(dai, StereoDepthConfig, CostAggregation, verticalPenaltyCostP2))
        .def_readwrite("localAggregationMode", &StereoDepthConfig::CostAggregation::localAggregationMode, DOC(dai, StereoDepthConfig, CostAggregation, localAggregationMode))
        ;

    stereoDepthConfig
        .def(py::init<>())
        .def_readwrite("algorithmControl", &StereoDepthConfig::algorithmControl,  DOC(dai, StereoDepthConfig, algorithmControl))
        .def_readwrite("postProcessing", &StereoDepthConfig::postProcessing,  DOC(dai, StereoDepthConfig, postProcessing))
        .def_readwrite("censusTransform", &StereoDepthConfig::censusTransform,  DOC(dai, StereoDepthConfig, censusTransform))
        .def_readwrite("costMatching", &StereoDepthConfig::costMatching,  DOC(dai, StereoDepthConfig, costMatching))
        .def_readwrite("costAggregation", &StereoDepthConfig::costAggregation,  DOC(dai, StereoDepthConfig, costAggregation))
        ;
    // legacy
    m.attr("StereoDepthConfig").attr("MedianFilter") = medianFilter;


    // Message
    stereoDepthConfig
        .def(py::init<>())
        .def("setDepthAlign", &StereoDepthConfig::setDepthAlign, py::arg("align"), DOC(dai, StereoDepthConfig, setDepthAlign))
        .def("setConfidenceThreshold",  &StereoDepthConfig::setConfidenceThreshold, py::arg("confThr"), DOC(dai, StereoDepthConfig, setConfidenceThreshold))
        .def("setMedianFilter",         &StereoDepthConfig::setMedianFilter, py::arg("median"), DOC(dai, StereoDepthConfig, setMedianFilter))
        .def("setBilateralFilterSigma", &StereoDepthConfig::setBilateralFilterSigma, py::arg("sigma"), DOC(dai, StereoDepthConfig, setBilateralFilterSigma))
        .def("setLeftRightCheckThreshold", &StereoDepthConfig::setLeftRightCheckThreshold, py::arg("sigma"), DOC(dai, StereoDepthConfig, setLeftRightCheckThreshold))
        .def("getConfidenceThreshold",  &StereoDepthConfig::getConfidenceThreshold, DOC(dai, StereoDepthConfig, getConfidenceThreshold))
        .def("getMedianFilter",         &StereoDepthConfig::getMedianFilter, DOC(dai, StereoDepthConfig, getMedianFilter))
        .def("getBilateralFilterSigma", &StereoDepthConfig::getBilateralFilterSigma, DOC(dai, StereoDepthConfig, getBilateralFilterSigma))
        .def("getLeftRightCheckThreshold",         &StereoDepthConfig::getLeftRightCheckThreshold, DOC(dai, StereoDepthConfig, getLeftRightCheckThreshold))
        .def("setLeftRightCheck",       &StereoDepthConfig::setLeftRightCheck, py::arg("enable"), DOC(dai, StereoDepthConfig, setLeftRightCheck))
        .def("setExtendedDisparity",    &StereoDepthConfig::setExtendedDisparity, py::arg("enable"), DOC(dai, StereoDepthConfig, setExtendedDisparity))
        .def("setSubpixel",             &StereoDepthConfig::setSubpixel, py::arg("enable"), DOC(dai, StereoDepthConfig, setSubpixel))
        .def("setSubpixelFractionalBits", &StereoDepthConfig::setSubpixelFractionalBits, py::arg("subpixelFractionalBits"), DOC(dai, StereoDepthConfig, setSubpixelFractionalBits))
        .def("getMaxDisparity",         &StereoDepthConfig::getMaxDisparity, DOC(dai, StereoDepthConfig, getMaxDisparity))
        .def("setDepthUnit",            &StereoDepthConfig::setDepthUnit, DOC(dai, StereoDepthConfig, setDepthUnit))
        .def("getDepthUnit",            &StereoDepthConfig::getDepthUnit, DOC(dai, StereoDepthConfig, getDepthUnit))
        .def("setDisparityShift",       &StereoDepthConfig::setDisparityShift, DOC(dai, StereoDepthConfig, setDisparityShift))
        .def("setNumInvalidateEdgePixels",       &StereoDepthConfig::setNumInvalidateEdgePixels, DOC(dai, StereoDepthConfig, setNumInvalidateEdgePixels))
        // .def("set",                     &StereoDepthConfig::set, py::arg("config"), DOC(dai, StereoDepthConfig, set))
        // .def("get",                     &StereoDepthConfig::get, DOC(dai, StereoDepthConfig, get))
        ;
    m.attr("StereoDepthConfig").attr("MedianFilter") = medianFilter;
    m.attr("StereoDepthConfig").attr("AlgorithmControl") = algorithmControl;
    m.attr("StereoDepthConfig").attr("PostProcessing") = postProcessing;
    m.attr("StereoDepthConfig").attr("CensusTransform") = censusTransform;
    m.attr("StereoDepthConfig").attr("CostMatching") = costMatching;
    m.attr("StereoDepthConfig").attr("CostAggregation") = costAggregation;


}
