#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/ColorCamera.hpp"
#include "depthai/pipeline/datatype/ImgFrame.hpp"

void bind_colorcamera(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<ColorCameraProperties> colorCameraProperties(m, "ColorCameraProperties", DOC(dai, ColorCameraProperties));
    py::enum_<ColorCameraProperties::SensorResolution> colorCameraPropertiesSensorResolution(colorCameraProperties, "SensorResolution", DOC(dai, ColorCameraProperties, SensorResolution));
    py::enum_<ColorCameraProperties::ColorOrder> colorCameraPropertiesColorOrder(colorCameraProperties, "ColorOrder", DOC(dai, ColorCameraProperties, ColorOrder));
    py::enum_<ColorCameraProperties::WarpMeshSource> colorCameraPropertiesWarpMeshSource(colorCameraProperties, "WarpMeshSource", DOC(dai, ColorCameraProperties, WarpMeshSource));
    auto colorCamera = ADD_NODE(ColorCamera);

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

    // ColorCamera Properties
    colorCameraPropertiesSensorResolution
        .value("THE_1080_P", ColorCameraProperties::SensorResolution::THE_1080_P)
        .value("THE_1200_P", ColorCameraProperties::SensorResolution::THE_1200_P)
        .value("THE_4_K", ColorCameraProperties::SensorResolution::THE_4_K)
        .value("THE_5_MP", ColorCameraProperties::SensorResolution::THE_5_MP)
        .value("THE_12_MP", ColorCameraProperties::SensorResolution::THE_12_MP)
        .value("THE_4000X3000", ColorCameraProperties::SensorResolution::THE_4000X3000)
        .value("THE_13_MP", ColorCameraProperties::SensorResolution::THE_13_MP)
        .value("THE_5312X6000", ColorCameraProperties::SensorResolution::THE_5312X6000)
        .value("THE_48_MP", ColorCameraProperties::SensorResolution::THE_48_MP)
        .value("THE_720_P", ColorCameraProperties::SensorResolution::THE_720_P)
        .value("THE_800_P", ColorCameraProperties::SensorResolution::THE_800_P)
        .value("THE_240X180", ColorCameraProperties::SensorResolution::THE_240X180)
        .value("THE_1280X962", ColorCameraProperties::SensorResolution::THE_1280X962)
        .value("THE_2000X1500", ColorCameraProperties::SensorResolution::THE_2000X1500)
        .value("THE_2028X1520", ColorCameraProperties::SensorResolution::THE_2028X1520)
        .value("THE_2104X1560", ColorCameraProperties::SensorResolution::THE_2104X1560)
        .value("THE_1440X1080", ColorCameraProperties::SensorResolution::THE_1440X1080)
        .value("THE_1352X1012", ColorCameraProperties::SensorResolution::THE_1352X1012)
        .value("THE_2024X1520", ColorCameraProperties::SensorResolution::THE_2024X1520)
        ;

    colorCameraPropertiesColorOrder
        .value("BGR", ColorCameraProperties::ColorOrder::BGR)
        .value("RGB", ColorCameraProperties::ColorOrder::RGB)
        ;

    colorCameraPropertiesWarpMeshSource
        .value("AUTO", ColorCameraProperties::WarpMeshSource::AUTO)
        .value("NONE", ColorCameraProperties::WarpMeshSource::NONE)
        .value("CALIBRATION", ColorCameraProperties::WarpMeshSource::CALIBRATION)
        .value("URI", ColorCameraProperties::WarpMeshSource::URI)
        ;

    colorCameraProperties
        .def_readwrite("initialControl", &ColorCameraProperties::initialControl)
        .def_readwrite("boardSocket", &ColorCameraProperties::boardSocket)
        .def_readwrite("imageOrientation", &ColorCameraProperties::imageOrientation)
        .def_readwrite("previewHeight", &ColorCameraProperties::previewHeight)
        .def_readwrite("previewWidth", &ColorCameraProperties::previewWidth)
        .def_readwrite("videoHeight", &ColorCameraProperties::videoHeight)
        .def_readwrite("videoWidth", &ColorCameraProperties::videoWidth)
        .def_readwrite("stillHeight", &ColorCameraProperties::stillHeight)
        .def_readwrite("stillWidth", &ColorCameraProperties::stillWidth)
        .def_readwrite("resolution", &ColorCameraProperties::resolution)
        .def_readwrite("fps", &ColorCameraProperties::fps)
        .def_readwrite("isp3aFps", &ColorCameraProperties::isp3aFps)
        .def_readwrite("sensorCropX", &ColorCameraProperties::sensorCropX)
        .def_readwrite("sensorCropY", &ColorCameraProperties::sensorCropY)
        .def_readwrite("previewKeepAspectRatio", &ColorCameraProperties::previewKeepAspectRatio)
        .def_readwrite("ispScale", &ColorCameraProperties::ispScale)
        .def_readwrite("numFramesPoolRaw", &ColorCameraProperties::numFramesPoolRaw)
        .def_readwrite("numFramesPoolIsp", &ColorCameraProperties::numFramesPoolIsp)
        .def_readwrite("numFramesPoolVideo", &ColorCameraProperties::numFramesPoolVideo)
        .def_readwrite("numFramesPoolPreview", &ColorCameraProperties::numFramesPoolPreview)
        .def_readwrite("numFramesPoolStill", &ColorCameraProperties::numFramesPoolStill)
        .def_readwrite("warpMeshSource", &ColorCameraProperties::warpMeshSource)
        .def_readwrite("warpMeshUri", &ColorCameraProperties::warpMeshUri)
        .def_readwrite("warpMeshWidth", &ColorCameraProperties::warpMeshWidth)
        .def_readwrite("warpMeshHeight", &ColorCameraProperties::warpMeshHeight)
        .def_readwrite("calibAlpha", &ColorCameraProperties::calibAlpha)
        .def_readwrite("warpMeshStepWidth", &ColorCameraProperties::warpMeshStepWidth)
        .def_readwrite("warpMeshStepHeight", &ColorCameraProperties::warpMeshStepHeight)
        .def_readwrite("eventFilter", &ColorCameraProperties::eventFilter)
    ;

    // ColorCamera node
    colorCamera
        .def_readonly("inputConfig", &ColorCamera::inputConfig, DOC(dai, node, ColorCamera, inputConfig))
        .def_readonly("inputControl", &ColorCamera::inputControl, DOC(dai, node, ColorCamera, inputControl))
        .def_readonly("inputMesh", &ColorCamera::inputMesh, DOC(dai, node, ColorCamera, inputMesh))
        .def_readonly("initialControl", &ColorCamera::initialControl, DOC(dai, node, ColorCamera, initialControl))
        .def_readonly("video", &ColorCamera::video, DOC(dai, node, ColorCamera, video))
        .def_readonly("preview", &ColorCamera::preview, DOC(dai, node, ColorCamera, preview))
        .def_readonly("still", &ColorCamera::still, DOC(dai, node, ColorCamera, still))
        .def_readonly("isp", &ColorCamera::isp, DOC(dai, node, ColorCamera, isp))
        .def_readonly("raw", &ColorCamera::raw, DOC(dai, node, ColorCamera, raw))
        .def_readonly("frameEvent",  &ColorCamera::frameEvent, DOC(dai, node, ColorCamera, frameEvent))
        .def("setCamId", [](ColorCamera& c, int64_t id) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCamId() is deprecated, use setBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            c.setCamId(id);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("getCamId", [](ColorCamera& c) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "getCamId() is deprecated, use getBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return c.getCamId();
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setBoardSocket", &ColorCamera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, ColorCamera, setBoardSocket))
        .def("getBoardSocket", &ColorCamera::getBoardSocket, DOC(dai, node, ColorCamera, getBoardSocket))
        .def("setImageOrientation", &ColorCamera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, ColorCamera, setImageOrientation))
        .def("getImageOrientation", &ColorCamera::getImageOrientation, DOC(dai, node, ColorCamera, getImageOrientation))
        .def("setColorOrder", &ColorCamera::setColorOrder, py::arg("colorOrder"), DOC(dai, node, ColorCamera, setColorOrder))
        .def("getColorOrder", &ColorCamera::getColorOrder, DOC(dai, node, ColorCamera, getColorOrder))
        .def("setInterleaved", &ColorCamera::setInterleaved, py::arg("interleaved"), DOC(dai, node, ColorCamera, setInterleaved))
        .def("getInterleaved", &ColorCamera::getInterleaved, DOC(dai, node, ColorCamera, getInterleaved))
        .def("setPreviewType", &ColorCamera::setPreviewType, py::arg("previewType"), DOC(dai, node, ColorCamera, setPreviewType))
        .def("getPreviewType", &ColorCamera::getPreviewType, DOC(dai, node, ColorCamera, getPreviewType))
        .def("setFp16", &ColorCamera::setFp16, py::arg("fp16"), DOC(dai, node, ColorCamera, setFp16))
        .def("getFp16", &ColorCamera::getFp16, DOC(dai, node, ColorCamera, getFp16))
        .def("setPreviewType", &ColorCamera::setPreviewType, py::arg("type"), DOC(dai, node, ColorCamera, setPreviewType))
        .def("getPreviewType", &ColorCamera::getPreviewType, DOC(dai, node, ColorCamera, getPreviewType))
        .def("setPreviewSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setPreviewSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setPreviewSize))
        .def("setPreviewSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setPreviewSize), py::arg("size"), DOC(dai, node, ColorCamera, setPreviewSize, 2))
        .def("setVideoSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setVideoSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setVideoSize))
        .def("setVideoSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setVideoSize), py::arg("size"), DOC(dai, node, ColorCamera, setVideoSize, 2))
        .def("setStillSize", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setStillSize), py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setStillSize))
        .def("setStillSize", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setStillSize), py::arg("size"), DOC(dai, node, ColorCamera, setStillSize, 2))
        .def("setResolution", &ColorCamera::setResolution, py::arg("resolution"), DOC(dai, node, ColorCamera, setResolution))
        .def("getResolution", &ColorCamera::getResolution, DOC(dai, node, ColorCamera, getResolution))
        .def("setFps", &ColorCamera::setFps, py::arg("fps"), DOC(dai, node, ColorCamera, setFps))
        .def("setIsp3aFps", &ColorCamera::setIsp3aFps, DOC(dai, node, ColorCamera, setIsp3aFps))
        .def("getFps", &ColorCamera::getFps, DOC(dai, node, ColorCamera, getFps))
        .def("setFrameEventFilter", &ColorCamera::setFrameEventFilter, py::arg("events"), DOC(dai, node, ColorCamera, setFrameEventFilter))
        .def("getFrameEventFilter", &ColorCamera::getFrameEventFilter, DOC(dai, node, ColorCamera, getFrameEventFilter))
        .def("getPreviewSize", &ColorCamera::getPreviewSize, DOC(dai, node, ColorCamera, getPreviewSize))
        .def("getPreviewWidth", &ColorCamera::getPreviewWidth, DOC(dai, node, ColorCamera, getPreviewWidth))
        .def("getPreviewHeight", &ColorCamera::getPreviewHeight, DOC(dai, node, ColorCamera, getPreviewHeight))
        .def("getVideoSize", &ColorCamera::getVideoSize, DOC(dai, node, ColorCamera, getVideoSize))
        .def("getVideoWidth", &ColorCamera::getVideoWidth, DOC(dai, node, ColorCamera, getVideoWidth))
        .def("getVideoHeight", &ColorCamera::getVideoHeight, DOC(dai, node, ColorCamera, getVideoHeight))
        .def("getStillSize", &ColorCamera::getStillSize, DOC(dai, node, ColorCamera, getStillSize))
        .def("getStillWidth", &ColorCamera::getStillWidth, DOC(dai, node, ColorCamera, getStillWidth))
        .def("getStillHeight", &ColorCamera::getStillHeight, DOC(dai, node, ColorCamera, getStillHeight))
        .def("getResolutionSize", &ColorCamera::getResolutionSize, DOC(dai, node, ColorCamera, getResolutionSize))
        .def("getResolutionWidth", &ColorCamera::getResolutionWidth, DOC(dai, node, ColorCamera, getResolutionWidth))
        .def("getResolutionHeight", &ColorCamera::getResolutionHeight, DOC(dai, node, ColorCamera, getResolutionHeight))
        .def("sensorCenterCrop", &ColorCamera::sensorCenterCrop, DOC(dai, node, ColorCamera, sensorCenterCrop))
        .def("setSensorCrop", &ColorCamera::setSensorCrop, py::arg("x"), py::arg("y"), DOC(dai, node, ColorCamera, setSensorCrop))
        .def("getSensorCrop", &ColorCamera::getSensorCrop, DOC(dai, node, ColorCamera, getSensorCrop))
        .def("getSensorCropX", &ColorCamera::getSensorCropX, DOC(dai, node, ColorCamera, getSensorCropX))
        .def("getSensorCropY", &ColorCamera::getSensorCropY, DOC(dai, node, ColorCamera, getSensorCropY))

        .def("setWaitForConfigInput", [](ColorCamera& cam, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            cam.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, ColorCamera, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](ColorCamera& cam){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return cam.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, ColorCamera, getWaitForConfigInput))

        .def("setPreviewKeepAspectRatio", &ColorCamera::setPreviewKeepAspectRatio, py::arg("keep"), DOC(dai, node, ColorCamera, setPreviewKeepAspectRatio))
        .def("getPreviewKeepAspectRatio", &ColorCamera::getPreviewKeepAspectRatio, DOC(dai, node, ColorCamera, getPreviewKeepAspectRatio))
        .def("setIspScale", static_cast<void(ColorCamera::*)(int,int)>(&ColorCamera::setIspScale), py::arg("numerator"), py::arg("denominator"), DOC(dai, node, ColorCamera, setIspScale))
        .def("setIspScale", static_cast<void(ColorCamera::*)(std::tuple<int,int>)>(&ColorCamera::setIspScale), py::arg("scale"), DOC(dai, node, ColorCamera, setIspScale, 2))
        .def("setIspScale", static_cast<void(ColorCamera::*)(int,int,int,int)>(&ColorCamera::setIspScale), py::arg("horizNum"), py::arg("horizDenom"), py::arg("vertNum"), py::arg("vertDenom"), DOC(dai, node, ColorCamera, setIspScale, 3))
        .def("setIspScale", static_cast<void(ColorCamera::*)(std::tuple<int,int>,std::tuple<int,int>)>(&ColorCamera::setIspScale), py::arg("horizScale"), py::arg("vertScale"), DOC(dai, node, ColorCamera, setIspScale, 4))
        .def("getIspSize", &ColorCamera::getIspSize, DOC(dai, node, ColorCamera, getIspSize))
        .def("getIspWidth", &ColorCamera::getIspWidth, DOC(dai, node, ColorCamera, getIspWidth))
        .def("getIspHeight", &ColorCamera::getIspHeight, DOC(dai, node, ColorCamera, getIspHeight))

        .def("setPreviewNumFramesPool", &ColorCamera::setPreviewNumFramesPool, DOC(dai, node, ColorCamera, setPreviewNumFramesPool))
        .def("setVideoNumFramesPool", &ColorCamera::setVideoNumFramesPool, DOC(dai, node, ColorCamera, setVideoNumFramesPool))
        .def("setStillNumFramesPool", &ColorCamera::setStillNumFramesPool, DOC(dai, node, ColorCamera, setStillNumFramesPool))
        .def("setRawNumFramesPool", &ColorCamera::setRawNumFramesPool, DOC(dai, node, ColorCamera, setRawNumFramesPool))
        .def("setIspNumFramesPool", &ColorCamera::setIspNumFramesPool, DOC(dai, node, ColorCamera, setIspNumFramesPool))
        .def("setNumFramesPool", &ColorCamera::setNumFramesPool, py::arg("raw"), py::arg("isp"), py::arg("preview"), py::arg("video"), py::arg("still"), DOC(dai, node, ColorCamera, setNumFramesPool))

        .def("getPreviewNumFramesPool", &ColorCamera::getPreviewNumFramesPool, DOC(dai, node, ColorCamera, getPreviewNumFramesPool))
        .def("getVideoNumFramesPool", &ColorCamera::getVideoNumFramesPool, DOC(dai, node, ColorCamera, getVideoNumFramesPool))
        .def("getStillNumFramesPool", &ColorCamera::getStillNumFramesPool, DOC(dai, node, ColorCamera, getStillNumFramesPool))
        .def("getRawNumFramesPool", &ColorCamera::getRawNumFramesPool, DOC(dai, node, ColorCamera, getRawNumFramesPool))
        .def("getIspNumFramesPool", &ColorCamera::getIspNumFramesPool, DOC(dai, node, ColorCamera, getIspNumFramesPool))
        .def("setCamera", &ColorCamera::setCamera, py::arg("name"), DOC(dai, node, ColorCamera, setCamera))
        .def("getCamera", &ColorCamera::getCamera, DOC(dai, node, ColorCamera, getCamera))
        
        .def("setMeshSource", &ColorCamera::setMeshSource, py::arg("source"), DOC(dai, node, ColorCamera, setMeshSource))
        .def("getMeshSource", &ColorCamera::getMeshSource, DOC(dai, node, ColorCamera, getMeshSource))
        // .def("loadMeshFile", &ColorCamera::loadMeshFile, py::arg("warpMesh"), DOC(dai, node, ColorCamera, loadMeshFile))
        .def("loadMeshData", &ColorCamera::loadMeshData, py::arg("warpMesh"), DOC(dai, node, ColorCamera, loadMeshData))
        .def("setMeshStep", &ColorCamera::setMeshStep, py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setMeshStep))
        .def("getMeshStep", &ColorCamera::getMeshStep, DOC(dai, node, ColorCamera, getMeshStep))
        .def("setMeshSize", &ColorCamera::setMeshSize, py::arg("width"), py::arg("height"), DOC(dai, node, ColorCamera, setMeshSize))
        .def("getMeshSize", &ColorCamera::getMeshSize, DOC(dai, node, ColorCamera, getMeshSize))
        .def("setCalibrationAlpha", &ColorCamera::setCalibrationAlpha, py::arg("alpha"), DOC(dai, node, ColorCamera, setCalibrationAlpha))
        .def("getCalibrationAlpha", &ColorCamera::getCalibrationAlpha, DOC(dai, node, ColorCamera, getCalibrationAlpha))
        

        .def("setRawOutputPacked", &ColorCamera::setRawOutputPacked, py::arg("packed"), DOC(dai, node, ColorCamera, setRawOutputPacked))
        ;
    // ALIAS
    daiNodeModule.attr("ColorCamera").attr("Properties") = colorCameraProperties;

}
