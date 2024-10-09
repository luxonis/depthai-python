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

    py::class_<RawStereoDepthConfig, RawBuffer, std::shared_ptr<RawStereoDepthConfig>> rawStereoDepthConfig(m, "RawStereoDepthConfig", DOC(dai, RawStereoDepthConfig));
    py::enum_<MedianFilter> medianFilter(m, "MedianFilter", DOC(dai, MedianFilter));
    py::class_<RawStereoDepthConfig::AlgorithmControl> algorithmControl(rawStereoDepthConfig, "AlgorithmControl", DOC(dai, RawStereoDepthConfig, AlgorithmControl));
    py::enum_<RawStereoDepthConfig::AlgorithmControl::DepthAlign> depthAlign(algorithmControl, "DepthAlign", DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthAlign));
    py::enum_<RawStereoDepthConfig::AlgorithmControl::DepthUnit> depthUnit(algorithmControl, "DepthUnit", DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit));
    py::class_<RawStereoDepthConfig::PostProcessing> postProcessing(rawStereoDepthConfig, "PostProcessing", DOC(dai, RawStereoDepthConfig, PostProcessing));
    py::enum_<RawStereoDepthConfig::PostProcessing::Filter> filter(postProcessing, "Filter", DOC(dai, RawStereoDepthConfig, PostProcessing, Filter));
    py::class_<RawStereoDepthConfig::PostProcessing::SpatialFilter> spatialFilter(postProcessing, "SpatialFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter));
    py::class_<RawStereoDepthConfig::PostProcessing::TemporalFilter> temporalFilter(postProcessing, "TemporalFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter));
    py::enum_<RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode> persistencyMode(temporalFilter, "PersistencyMode", DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode));
    py::class_<RawStereoDepthConfig::PostProcessing::ThresholdFilter> thresholdFilter(postProcessing, "ThresholdFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, ThresholdFilter));
    py::class_<RawStereoDepthConfig::PostProcessing::BrightnessFilter> brightnessFilter(postProcessing, "BrightnessFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, BrightnessFilter));
    py::class_<RawStereoDepthConfig::PostProcessing::SpeckleFilter> speckleFilter(postProcessing, "SpeckleFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, SpeckleFilter));
    py::class_<RawStereoDepthConfig::PostProcessing::DecimationFilter> decimationFilter(postProcessing, "DecimationFilter", DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter));
    py::enum_<RawStereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode> decimationMode(decimationFilter, "DecimationMode", DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode));
    py::class_<RawStereoDepthConfig::CostAggregation> costAggregation(rawStereoDepthConfig, "CostAggregation", DOC(dai, RawStereoDepthConfig, CostAggregation));
    py::class_<RawStereoDepthConfig::CostMatching> costMatching(rawStereoDepthConfig, "CostMatching", DOC(dai, RawStereoDepthConfig, CostMatching));
    py::class_<RawStereoDepthConfig::CostMatching::LinearEquationParameters> costMatchingLinearEquationParameters(costMatching, "LinearEquationParameters", DOC(dai, RawStereoDepthConfig, CostMatching, LinearEquationParameters));
    py::enum_<RawStereoDepthConfig::CostMatching::DisparityWidth> costMatchingDisparityWidth(costMatching, "DisparityWidth", DOC(dai, RawStereoDepthConfig, CostMatching, DisparityWidth));
    py::class_<RawStereoDepthConfig::CensusTransform> censusTransform(rawStereoDepthConfig, "CensusTransform", DOC(dai, RawStereoDepthConfig, CensusTransform));
    py::enum_<RawStereoDepthConfig::CensusTransform::KernelSize> censusTransformKernelSize(censusTransform, "KernelSize", DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize));
    py::class_<StereoDepthConfig, Buffer, std::shared_ptr<StereoDepthConfig>> stereoDepthConfig(m, "StereoDepthConfig", DOC(dai, StereoDepthConfig));

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

    filter
        .value("DECIMATION", RawStereoDepthConfig::PostProcessing::Filter::DECIMATION, DOC(dai, RawStereoDepthConfig, PostProcessing, Filter, DECIMATION))
        .value("SPECKLE", RawStereoDepthConfig::PostProcessing::Filter::SPECKLE, DOC(dai, RawStereoDepthConfig, PostProcessing, Filter, SPECKLE))
        .value("TEMPORAL", RawStereoDepthConfig::PostProcessing::Filter::TEMPORAL, DOC(dai, RawStereoDepthConfig, PostProcessing, Filter, TEMPORAL))
        .value("SPATIAL", RawStereoDepthConfig::PostProcessing::Filter::SPATIAL, DOC(dai, RawStereoDepthConfig, PostProcessing, Filter, SPATIAL))
        .value("FILTER_COUNT", RawStereoDepthConfig::PostProcessing::Filter::FILTER_COUNT, DOC(dai, RawStereoDepthConfig, PostProcessing, Filter, FILTER_COUNT))
        ;

    medianFilter
        .value("MEDIAN_OFF", MedianFilter::MEDIAN_OFF)
        .value("KERNEL_3x3", MedianFilter::KERNEL_3x3)
        .value("KERNEL_5x5", MedianFilter::KERNEL_5x5)
        .value("KERNEL_7x7", MedianFilter::KERNEL_7x7)
        ;
    m.attr("StereoDepthProperties").attr("MedianFilter") = medianFilter;


    depthAlign
        .value("RECTIFIED_RIGHT", RawStereoDepthConfig::AlgorithmControl::DepthAlign::RECTIFIED_RIGHT, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthAlign, RECTIFIED_RIGHT))
        .value("RECTIFIED_LEFT", RawStereoDepthConfig::AlgorithmControl::DepthAlign::RECTIFIED_LEFT, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthAlign, RECTIFIED_LEFT))
        .value("CENTER", RawStereoDepthConfig::AlgorithmControl::DepthAlign::CENTER, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthAlign, CENTER));

    m.attr("StereoDepthProperties").attr("DepthAlign") = depthAlign;

    depthUnit
        .value("METER", RawStereoDepthConfig::AlgorithmControl::DepthUnit::METER, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, METER))
        .value("CENTIMETER", RawStereoDepthConfig::AlgorithmControl::DepthUnit::CENTIMETER, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, CENTIMETER))
        .value("MILLIMETER", RawStereoDepthConfig::AlgorithmControl::DepthUnit::MILLIMETER, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, MILLIMETER))
        .value("INCH", RawStereoDepthConfig::AlgorithmControl::DepthUnit::INCH, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, INCH))
        .value("FOOT", RawStereoDepthConfig::AlgorithmControl::DepthUnit::FOOT, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, FOOT))
        .value("CUSTOM", RawStereoDepthConfig::AlgorithmControl::DepthUnit::CUSTOM, DOC(dai, RawStereoDepthConfig, AlgorithmControl, DepthUnit, CUSTOM))
        ;

    algorithmControl
        .def(py::init<>())
        .def_readwrite("depthAlign", &RawStereoDepthConfig::AlgorithmControl::depthAlign, DOC(dai, RawStereoDepthConfig, AlgorithmControl, depthAlign))
        .def_readwrite("depthUnit", &RawStereoDepthConfig::AlgorithmControl::depthUnit, DOC(dai, RawStereoDepthConfig, AlgorithmControl, depthUnit))
        .def_readwrite("customDepthUnitMultiplier", &RawStereoDepthConfig::AlgorithmControl::customDepthUnitMultiplier, DOC(dai, RawStereoDepthConfig, AlgorithmControl, customDepthUnitMultiplier))
        .def_readwrite("enableLeftRightCheck", &RawStereoDepthConfig::AlgorithmControl::enableLeftRightCheck, DOC(dai, RawStereoDepthConfig, AlgorithmControl, enableLeftRightCheck))
        .def_readwrite("enableExtended", &RawStereoDepthConfig::AlgorithmControl::enableExtended, DOC(dai, RawStereoDepthConfig, AlgorithmControl, enableExtended))
        .def_readwrite("enableSubpixel", &RawStereoDepthConfig::AlgorithmControl::enableSubpixel, DOC(dai, RawStereoDepthConfig, AlgorithmControl, enableSubpixel))
        .def_readwrite("leftRightCheckThreshold", &RawStereoDepthConfig::AlgorithmControl::leftRightCheckThreshold, DOC(dai, RawStereoDepthConfig, AlgorithmControl, leftRightCheckThreshold))
        .def_readwrite("subpixelFractionalBits", &RawStereoDepthConfig::AlgorithmControl::subpixelFractionalBits, DOC(dai, RawStereoDepthConfig, AlgorithmControl, subpixelFractionalBits))
        .def_readwrite("disparityShift", &RawStereoDepthConfig::AlgorithmControl::disparityShift, DOC(dai, RawStereoDepthConfig, AlgorithmControl, disparityShift))
        .def_readwrite("centerAlignmentShiftFactor", &RawStereoDepthConfig::AlgorithmControl::centerAlignmentShiftFactor, DOC(dai, RawStereoDepthConfig, AlgorithmControl, centerAlignmentShiftFactor))
        .def_readwrite("numInvalidateEdgePixels", &RawStereoDepthConfig::AlgorithmControl::numInvalidateEdgePixels, DOC(dai, RawStereoDepthConfig, AlgorithmControl, numInvalidateEdgePixels))
        ;

    spatialFilter
        .def(py::init<>())
        .def_readwrite("enable", &RawStereoDepthConfig::PostProcessing::SpatialFilter::enable, DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter, enable))
        .def_readwrite("holeFillingRadius", &RawStereoDepthConfig::PostProcessing::SpatialFilter::holeFillingRadius, DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter, holeFillingRadius))
        .def_readwrite("alpha", &RawStereoDepthConfig::PostProcessing::SpatialFilter::alpha, DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter, alpha))
        .def_readwrite("delta", &RawStereoDepthConfig::PostProcessing::SpatialFilter::delta, DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter, delta))
        .def_readwrite("numIterations", &RawStereoDepthConfig::PostProcessing::SpatialFilter::numIterations, DOC(dai, RawStereoDepthConfig, PostProcessing, SpatialFilter, numIterations))
        ;

    persistencyMode
        .value("PERSISTENCY_OFF", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::PERSISTENCY_OFF, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, PERSISTENCY_OFF))
        .value("VALID_8_OUT_OF_8", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_8_OUT_OF_8, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_8_OUT_OF_8))
        .value("VALID_2_IN_LAST_3", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_IN_LAST_3, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_IN_LAST_3))
        .value("VALID_2_IN_LAST_4", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_IN_LAST_4, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_IN_LAST_4))
        .value("VALID_2_OUT_OF_8", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_2_OUT_OF_8, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_2_OUT_OF_8))
        .value("VALID_1_IN_LAST_2", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_2, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_2))
        .value("VALID_1_IN_LAST_5", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_5, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_5))
        .value("VALID_1_IN_LAST_8", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::VALID_1_IN_LAST_8, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, VALID_1_IN_LAST_8))
        .value("PERSISTENCY_INDEFINITELY", RawStereoDepthConfig::PostProcessing::TemporalFilter::PersistencyMode::PERSISTENCY_INDEFINITELY, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, PersistencyMode, PERSISTENCY_INDEFINITELY))
        ;

    temporalFilter
        .def(py::init<>())
        .def_readwrite("enable", &RawStereoDepthConfig::PostProcessing::TemporalFilter::enable, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, enable))
        .def_readwrite("persistencyMode", &RawStereoDepthConfig::PostProcessing::TemporalFilter::persistencyMode, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, persistencyMode))
        .def_readwrite("alpha", &RawStereoDepthConfig::PostProcessing::TemporalFilter::alpha, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, alpha))
        .def_readwrite("delta", &RawStereoDepthConfig::PostProcessing::TemporalFilter::delta, DOC(dai, RawStereoDepthConfig, PostProcessing, TemporalFilter, delta))
        ;

    thresholdFilter
        .def(py::init<>())
        .def_readwrite("minRange", &RawStereoDepthConfig::PostProcessing::ThresholdFilter::minRange, DOC(dai, RawStereoDepthConfig, PostProcessing, ThresholdFilter, minRange))
        .def_readwrite("maxRange", &RawStereoDepthConfig::PostProcessing::ThresholdFilter::maxRange, DOC(dai, RawStereoDepthConfig, PostProcessing, ThresholdFilter, maxRange))
        ;

    brightnessFilter
        .def(py::init<>())
        .def_readwrite("minBrightness", &RawStereoDepthConfig::PostProcessing::BrightnessFilter::minBrightness, DOC(dai, RawStereoDepthConfig, PostProcessing, BrightnessFilter, minBrightness))
        .def_readwrite("maxBrightness", &RawStereoDepthConfig::PostProcessing::BrightnessFilter::maxBrightness, DOC(dai, RawStereoDepthConfig, PostProcessing, BrightnessFilter, maxBrightness))
        ;

    speckleFilter
        .def(py::init<>())
        .def_readwrite("enable", &RawStereoDepthConfig::PostProcessing::SpeckleFilter::enable, DOC(dai, RawStereoDepthConfig, PostProcessing, SpeckleFilter, enable))
        .def_readwrite("speckleRange", &RawStereoDepthConfig::PostProcessing::SpeckleFilter::speckleRange, DOC(dai, RawStereoDepthConfig, PostProcessing, SpeckleFilter, speckleRange))
        .def_readwrite("differenceThreshold", &RawStereoDepthConfig::PostProcessing::SpeckleFilter::differenceThreshold, DOC(dai, RawStereoDepthConfig, PostProcessing, SpeckleFilter, differenceThreshold))
        ;

    decimationMode
        .value("PIXEL_SKIPPING", RawStereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::PIXEL_SKIPPING, DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, PIXEL_SKIPPING))
        .value("NON_ZERO_MEDIAN", RawStereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::NON_ZERO_MEDIAN, DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, NON_ZERO_MEDIAN))
        .value("NON_ZERO_MEAN", RawStereoDepthConfig::PostProcessing::DecimationFilter::DecimationMode::NON_ZERO_MEAN, DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, DecimationMode, NON_ZERO_MEAN))
        ;

    decimationFilter
        .def(py::init<>())
        .def_readwrite("decimationFactor", &RawStereoDepthConfig::PostProcessing::DecimationFilter::decimationFactor, DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, decimationFactor))
        .def_readwrite("decimationMode", &RawStereoDepthConfig::PostProcessing::DecimationFilter::decimationMode, DOC(dai, RawStereoDepthConfig, PostProcessing, DecimationFilter, decimationMode))
        ;

    postProcessing
        .def(py::init<>())
        .def_readwrite("filteringOrder", &RawStereoDepthConfig::PostProcessing::filteringOrder, DOC(dai, RawStereoDepthConfig, PostProcessing, filteringOrder))
        .def_readwrite("median", &RawStereoDepthConfig::PostProcessing::median, DOC(dai, RawStereoDepthConfig, PostProcessing, median))
        .def_readwrite("bilateralSigmaValue", &RawStereoDepthConfig::PostProcessing::bilateralSigmaValue, DOC(dai, RawStereoDepthConfig, PostProcessing, bilateralSigmaValue))
        .def_readwrite("spatialFilter", &RawStereoDepthConfig::PostProcessing::spatialFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, spatialFilter))
        .def_readwrite("temporalFilter", &RawStereoDepthConfig::PostProcessing::temporalFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, temporalFilter))
        .def_readwrite("thresholdFilter", &RawStereoDepthConfig::PostProcessing::thresholdFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, thresholdFilter))
        .def_readwrite("brightnessFilter", &RawStereoDepthConfig::PostProcessing::brightnessFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, brightnessFilter))
        .def_readwrite("speckleFilter", &RawStereoDepthConfig::PostProcessing::speckleFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, speckleFilter))
        .def_readwrite("decimationFilter", &RawStereoDepthConfig::PostProcessing::decimationFilter, DOC(dai, RawStereoDepthConfig, PostProcessing, decimationFilter))
        ;

    // KernelSize
    censusTransformKernelSize
        .value("AUTO", RawStereoDepthConfig::CensusTransform::KernelSize::AUTO, DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize, AUTO))
        .value("KERNEL_5x5", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_5x5, DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize, KERNEL_5x5))
        .value("KERNEL_7x7", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x7, DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize, KERNEL_7x7))
        .value("KERNEL_7x9", RawStereoDepthConfig::CensusTransform::KernelSize::KERNEL_7x9, DOC(dai, RawStereoDepthConfig, CensusTransform, KernelSize, KERNEL_7x9))
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
        .value("DISPARITY_64", RawStereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_64, DOC(dai, RawStereoDepthConfig, CostMatching, DisparityWidth, DISPARITY_64))
        .value("DISPARITY_96", RawStereoDepthConfig::CostMatching::DisparityWidth::DISPARITY_96, DOC(dai, RawStereoDepthConfig, CostMatching, DisparityWidth, DISPARITY_96))
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
        .def_readwrite("horizontalPenaltyCostP1", &RawStereoDepthConfig::CostAggregation::horizontalPenaltyCostP1, DOC(dai, RawStereoDepthConfig, CostAggregation, horizontalPenaltyCostP1))
        .def_readwrite("horizontalPenaltyCostP2", &RawStereoDepthConfig::CostAggregation::horizontalPenaltyCostP2, DOC(dai, RawStereoDepthConfig, CostAggregation, horizontalPenaltyCostP2))
        .def_readwrite("verticalPenaltyCostP1", &RawStereoDepthConfig::CostAggregation::verticalPenaltyCostP1, DOC(dai, RawStereoDepthConfig, CostAggregation, verticalPenaltyCostP1))
        .def_readwrite("verticalPenaltyCostP2", &RawStereoDepthConfig::CostAggregation::verticalPenaltyCostP2, DOC(dai, RawStereoDepthConfig, CostAggregation, verticalPenaltyCostP2))
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
        .def("set",                     &StereoDepthConfig::set, py::arg("config"), DOC(dai, StereoDepthConfig, set))
        .def("get",                     &StereoDepthConfig::get, DOC(dai, StereoDepthConfig, get))
        ;
    m.attr("StereoDepthConfig").attr("MedianFilter") = medianFilter;
    m.attr("StereoDepthConfig").attr("AlgorithmControl") = algorithmControl;
    m.attr("StereoDepthConfig").attr("PostProcessing") = postProcessing;
    m.attr("StereoDepthConfig").attr("CensusTransform") = censusTransform;
    m.attr("StereoDepthConfig").attr("CostMatching") = costMatching;
    m.attr("StereoDepthConfig").attr("CostAggregation") = costAggregation;


}
