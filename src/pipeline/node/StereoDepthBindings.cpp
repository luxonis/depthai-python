#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/StereoDepth.hpp"

void bind_stereodepth(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    auto stereoDepth = ADD_NODE(StereoDepth);
    py::class_<StereoDepthProperties> stereoDepthProperties(m, "StereoDepthProperties", DOC(dai, StereoDepthProperties));
    py::class_<StereoDepthProperties::RectificationMesh> rectificationMesh(stereoDepthProperties, "RectificationMesh", DOC(dai, StereoDepthProperties, RectificationMesh));
    py::enum_<StereoDepth::PresetMode> stereoDepthPresetMode(stereoDepth, "PresetMode", DOC(dai, node, StereoDepth, PresetMode));

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

    // Properties
    rectificationMesh
        .def_readwrite("meshLeftUri", &StereoDepthProperties::RectificationMesh::meshLeftUri, DOC(dai, StereoDepthProperties, RectificationMesh, meshLeftUri))
        .def_readwrite("meshRightUri", &StereoDepthProperties::RectificationMesh::meshRightUri, DOC(dai, StereoDepthProperties, RectificationMesh, meshRightUri))
        .def_readwrite("meshSize", &StereoDepthProperties::RectificationMesh::meshSize, DOC(dai, StereoDepthProperties, RectificationMesh, meshSize))
        .def_readwrite("stepWidth", &StereoDepthProperties::RectificationMesh::stepWidth, DOC(dai, StereoDepthProperties, RectificationMesh, stepWidth))
        .def_readwrite("stepHeight", &StereoDepthProperties::RectificationMesh::stepHeight, DOC(dai, StereoDepthProperties, RectificationMesh, stepHeight))
        ;

    stereoDepthProperties
        .def_readwrite("initialConfig", &StereoDepthProperties::initialConfig, DOC(dai, StereoDepthProperties, initialConfig))
        .def_readwrite("depthAlignCamera", &StereoDepthProperties::depthAlignCamera, DOC(dai, StereoDepthProperties, depthAlignCamera))
        .def_readwrite("enableRectification", &StereoDepthProperties::enableRectification, DOC(dai, StereoDepthProperties, enableRectification))
        .def_readwrite("rectifyEdgeFillColor", &StereoDepthProperties::rectifyEdgeFillColor, DOC(dai, StereoDepthProperties, rectifyEdgeFillColor))
        .def_readwrite("width", &StereoDepthProperties::width, DOC(dai, StereoDepthProperties, width))
        .def_readwrite("height", &StereoDepthProperties::height, DOC(dai, StereoDepthProperties, height))
        .def_readwrite("outWidth", &StereoDepthProperties::outWidth, DOC(dai, StereoDepthProperties, outWidth))
        .def_readwrite("outHeight", &StereoDepthProperties::outHeight, DOC(dai, StereoDepthProperties, outHeight))
        .def_readwrite("outKeepAspectRatio", &StereoDepthProperties::outKeepAspectRatio, DOC(dai, StereoDepthProperties, outKeepAspectRatio))
        .def_readwrite("mesh", &StereoDepthProperties::mesh, DOC(dai, StereoDepthProperties, mesh))
        .def_readwrite("enableRuntimeStereoModeSwitch", &StereoDepthProperties::enableRuntimeStereoModeSwitch, DOC(dai, StereoDepthProperties, enableRuntimeStereoModeSwitch))
        .def_readwrite("numFramesPool", &StereoDepthProperties::numFramesPool, DOC(dai, StereoDepthProperties, numFramesPool))
        .def_readwrite("numPostProcessingShaves", &StereoDepthProperties::numPostProcessingShaves, DOC(dai, StereoDepthProperties, numPostProcessingShaves))
        .def_readwrite("numPostProcessingMemorySlices", &StereoDepthProperties::numPostProcessingMemorySlices, DOC(dai, StereoDepthProperties, numPostProcessingMemorySlices))
        .def_readwrite("focalLengthFromCalibration", &StereoDepthProperties::focalLengthFromCalibration, DOC(dai, StereoDepthProperties, focalLengthFromCalibration))
        .def_readwrite("useHomographyRectification", &StereoDepthProperties::useHomographyRectification, DOC(dai, StereoDepthProperties, useHomographyRectification))
        .def_readwrite("baseline", &StereoDepthProperties::baseline, DOC(dai, StereoDepthProperties, baseline))
        .def_readwrite("focalLength", &StereoDepthProperties::focalLength, DOC(dai, StereoDepthProperties, focalLength))
        .def_readwrite("disparityToDepthUseSpecTranslation", &StereoDepthProperties::disparityToDepthUseSpecTranslation, DOC(dai, StereoDepthProperties, disparityToDepthUseSpecTranslation))
        .def_readwrite("rectificationUseSpecTranslation", &StereoDepthProperties::rectificationUseSpecTranslation, DOC(dai, StereoDepthProperties, rectificationUseSpecTranslation))
        .def_readwrite("depthAlignmentUseSpecTranslation", &StereoDepthProperties::depthAlignmentUseSpecTranslation, DOC(dai, StereoDepthProperties, depthAlignmentUseSpecTranslation))
        .def_readwrite("alphaScaling", &StereoDepthProperties::alphaScaling, DOC(dai, StereoDepthProperties, alphaScaling))
        ;

    stereoDepthPresetMode
        .value("HIGH_ACCURACY", StereoDepth::PresetMode::HIGH_ACCURACY, DOC(dai, node, StereoDepth, PresetMode, HIGH_ACCURACY))
        .value("HIGH_DENSITY", StereoDepth::PresetMode::HIGH_DENSITY, DOC(dai, node, StereoDepth, PresetMode, HIGH_DENSITY))
        ;

    // Node
    stereoDepth
        .def_readonly("initialConfig",  &StereoDepth::initialConfig, DOC(dai, node, StereoDepth, initialConfig))
        .def_readonly("inputConfig",    &StereoDepth::inputConfig, DOC(dai, node, StereoDepth, inputConfig))
        .def_readonly("left",           &StereoDepth::left, DOC(dai, node, StereoDepth, left))
        .def_readonly("right",          &StereoDepth::right, DOC(dai, node, StereoDepth, right))
        .def_readonly("depth",          &StereoDepth::depth, DOC(dai, node, StereoDepth, depth))
        .def_readonly("disparity",      &StereoDepth::disparity, DOC(dai, node, StereoDepth, disparity))
        .def_readonly("syncedLeft",     &StereoDepth::syncedLeft, DOC(dai, node, StereoDepth, syncedLeft))
        .def_readonly("syncedRight",    &StereoDepth::syncedRight, DOC(dai, node, StereoDepth, syncedRight))
        .def_readonly("rectifiedLeft",  &StereoDepth::rectifiedLeft, DOC(dai, node, StereoDepth, rectifiedLeft))
        .def_readonly("rectifiedRight", &StereoDepth::rectifiedRight, DOC(dai, node, StereoDepth, rectifiedRight))
        .def_readonly("outConfig",      &StereoDepth::outConfig, DOC(dai, node, StereoDepth, outConfig))
        .def_readonly("debugDispLrCheckIt1", &StereoDepth::debugDispLrCheckIt1, DOC(dai, node, StereoDepth, debugDispLrCheckIt1))
        .def_readonly("debugDispLrCheckIt2", &StereoDepth::debugDispLrCheckIt2, DOC(dai, node, StereoDepth, debugDispLrCheckIt2))
        .def_readonly("debugExtDispLrCheckIt1", &StereoDepth::debugExtDispLrCheckIt1, DOC(dai, node, StereoDepth, debugExtDispLrCheckIt1))
        .def_readonly("debugExtDispLrCheckIt2", &StereoDepth::debugExtDispLrCheckIt2, DOC(dai, node, StereoDepth, debugExtDispLrCheckIt2))
        .def_readonly("debugDispCostDump",   &StereoDepth::debugDispCostDump, DOC(dai, node, StereoDepth, debugDispCostDump))
        .def_readonly("confidenceMap",          &StereoDepth::confidenceMap, DOC(dai, node, StereoDepth, confidenceMap))
        .def("loadMeshFiles",           &StereoDepth::loadMeshFiles, py::arg("pathLeft"), py::arg("pathRight"), DOC(dai, node, StereoDepth, loadMeshFiles))
        .def("loadMeshData",            &StereoDepth::loadMeshData, py::arg("dataLeft"), py::arg("dataRight"), DOC(dai, node, StereoDepth, loadMeshData))
        .def("setMeshStep",             &StereoDepth::setMeshStep, py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setMeshStep))
        .def("setInputResolution",      static_cast<void(StereoDepth::*)(int,int)>(&StereoDepth::setInputResolution), py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setInputResolution))
        .def("setInputResolution",      static_cast<void(StereoDepth::*)(std::tuple<int,int>)>(&StereoDepth::setInputResolution), py::arg("resolution"), DOC(dai, node, StereoDepth, setInputResolution, 2))
        .def("setOutputSize",           &StereoDepth::setOutputSize, py::arg("width"), py::arg("height"), DOC(dai, node, StereoDepth, setOutputSize))
        .def("setOutputKeepAspectRatio",&StereoDepth::setOutputKeepAspectRatio, py::arg("keep"), DOC(dai, node, StereoDepth, setOutputKeepAspectRatio))
        .def("setDepthAlign",           static_cast<void(StereoDepth::*)(StereoDepthProperties::DepthAlign)>(&StereoDepth::setDepthAlign), py::arg("align"), DOC(dai, node, StereoDepth, setDepthAlign))
        .def("setDepthAlign",           static_cast<void(StereoDepth::*)(CameraBoardSocket)>(&StereoDepth::setDepthAlign), py::arg("camera"), DOC(dai, node, StereoDepth, setDepthAlign, 2))
        .def("setRectification",        &StereoDepth::setRectification, py::arg("enable"), DOC(dai, node, StereoDepth, setRectification))
        .def("setLeftRightCheck",       &StereoDepth::setLeftRightCheck, py::arg("enable"), DOC(dai, node, StereoDepth, setLeftRightCheck))
        .def("setSubpixel",             &StereoDepth::setSubpixel, py::arg("enable"), DOC(dai, node, StereoDepth, setSubpixel))
        .def("setSubpixelFractionalBits", &StereoDepth::setSubpixelFractionalBits, py::arg("subpixelFractionalBits"), DOC(dai, node, StereoDepth, setSubpixelFractionalBits))
        .def("setExtendedDisparity",    &StereoDepth::setExtendedDisparity, py::arg("enable"), DOC(dai, node, StereoDepth, setExtendedDisparity))
        .def("setRectifyEdgeFillColor", &StereoDepth::setRectifyEdgeFillColor, py::arg("color"), DOC(dai, node, StereoDepth, setRectifyEdgeFillColor))
        .def("setRectifyMirrorFrame", [](StereoDepth& s, bool enable) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setRectifyMirrorFrame() is deprecated.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setRectifyMirrorFrame(enable);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setRectifyMirrorFrame))

        .def("setConfidenceThreshold", [](StereoDepth& s, int confThr) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setConfidenceThreshold() is deprecated, Use 'initialConfig.setConfidenceThreshold()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setConfidenceThreshold(confThr);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setConfidenceThreshold))
        .def("setMedianFilter", [](StereoDepth& s, dai::MedianFilter median) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setMedianFilter() is deprecated, Use 'initialConfig.setMedianFilter()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setMedianFilter(median);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setMedianFilter))
        .def("setOutputRectified", [](StereoDepth& s, bool enable) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setOutputRectified() is deprecated, the output is auto-enabled if used.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setOutputRectified(enable);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setOutputDepth", [](StereoDepth& s, bool enable) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setOutputDepth() is deprecated, the output is auto-enabled if used.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setOutputDepth(enable);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setEmptyCalibration", [](StereoDepth& s){
            PyErr_WarnEx(PyExc_DeprecationWarning, "setEmptyCalibration() is deprecated, Use 'setRectification(False)' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            s.setEmptyCalibration();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setEmptyCalibration))
        .def("setRuntimeModeSwitch", &StereoDepth::setRuntimeModeSwitch, DOC(dai, node, StereoDepth, setRuntimeModeSwitch))
        .def("setNumFramesPool", &StereoDepth::setNumFramesPool, DOC(dai, node, StereoDepth, setNumFramesPool))
        .def("getMaxDisparity", [](StereoDepth& s){
            PyErr_WarnEx(PyExc_DeprecationWarning, "getMaxDisparity() is deprecated, Use 'initialConfig.getMaxDisparity()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return s.getMaxDisparity();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, getMaxDisparity))
        .def("setPostProcessingHardwareResources", &StereoDepth::setPostProcessingHardwareResources, DOC(dai, node, StereoDepth, setPostProcessingHardwareResources))
        .def("setDefaultProfilePreset", &StereoDepth::setDefaultProfilePreset, DOC(dai, node, StereoDepth, setDefaultProfilePreset))
        .def("setFocalLengthFromCalibration", [](StereoDepth& s, bool focalLengthFromCalibration){
            PyErr_WarnEx(PyExc_DeprecationWarning, "setFocalLengthFromCalibration is deprecated. Default value is true.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return s.setFocalLengthFromCalibration(focalLengthFromCalibration);
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, StereoDepth, setFocalLengthFromCalibration))
        .def("useHomographyRectification", &StereoDepth::useHomographyRectification, DOC(dai, node, StereoDepth, useHomographyRectification))
        .def("enableDistortionCorrection", &StereoDepth::enableDistortionCorrection, DOC(dai, node, StereoDepth, enableDistortionCorrection))
        .def("setBaseline", &StereoDepth::setBaseline, DOC(dai, node, StereoDepth, setBaseline))
        .def("setFocalLength", &StereoDepth::setFocalLength, DOC(dai, node, StereoDepth, setFocalLength))
        .def("setDisparityToDepthUseSpecTranslation", &StereoDepth::setDisparityToDepthUseSpecTranslation, DOC(dai, node, StereoDepth, setDisparityToDepthUseSpecTranslation))
        .def("setRectificationUseSpecTranslation", &StereoDepth::setRectificationUseSpecTranslation, DOC(dai, node, StereoDepth, setRectificationUseSpecTranslation))
        .def("setDepthAlignmentUseSpecTranslation", &StereoDepth::setDepthAlignmentUseSpecTranslation, DOC(dai, node, StereoDepth, setDepthAlignmentUseSpecTranslation))
        .def("setAlphaScaling", &StereoDepth::setAlphaScaling, DOC(dai, node, StereoDepth, setAlphaScaling))
        ;
    // ALIAS
    daiNodeModule.attr("StereoDepth").attr("Properties") = stereoDepthProperties;



}
