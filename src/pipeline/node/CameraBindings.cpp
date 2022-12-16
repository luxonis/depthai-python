#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Camera.hpp"

void bind_camera(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<CameraProperties> cameraProperties(m, "CameraProperties", DOC(dai, CameraProperties));
    // py::enum_<CameraProperties::SensorResolution> cameraPropertiesSensorResolution(cameraProperties, "SensorResolution", DOC(dai, CameraProperties, SensorResolution));
    py::enum_<CameraProperties::ColorOrder> cameraPropertiesColorOrder(cameraProperties, "ColorOrder", DOC(dai, CameraProperties, ColorOrder));
    auto camera = ADD_NODE(Camera);

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

    // // Camera Properties
    // cameraPropertiesSensorResolution
    //     .value("THE_1080_P", CameraProperties::SensorResolution::THE_1080_P)
    //     .value("THE_1200_P", CameraProperties::SensorResolution::THE_1200_P)
    //     .value("THE_4_K", CameraProperties::SensorResolution::THE_4_K)
    //     .value("THE_5_MP", CameraProperties::SensorResolution::THE_5_MP)
    //     .value("THE_12_MP", CameraProperties::SensorResolution::THE_12_MP)
    //     .value("THE_4000X3000", CameraProperties::SensorResolution::THE_4000X3000)
    //     .value("THE_13_MP", CameraProperties::SensorResolution::THE_13_MP)
    //     .value("THE_5312X6000", CameraProperties::SensorResolution::THE_5312X6000)
    //     .value("THE_48_MP", CameraProperties::SensorResolution::THE_48_MP)
    //     .value("THE_720_P", CameraProperties::SensorResolution::THE_720_P)
    //     .value("THE_800_P", CameraProperties::SensorResolution::THE_800_P)
    //     ;

    cameraPropertiesColorOrder
        .value("BGR", CameraProperties::ColorOrder::BGR)
        .value("RGB", CameraProperties::ColorOrder::RGB)
        ;

    cameraProperties
        .def_readwrite("initialControl", &CameraProperties::initialControl)
        .def_readwrite("boardSocket", &CameraProperties::boardSocket)
        .def_readwrite("imageOrientation", &CameraProperties::imageOrientation)
        .def_readwrite("colorOrder", &CameraProperties::colorOrder)
        .def_readwrite("interleaved", &CameraProperties::interleaved)
        .def_readwrite("fp16", &CameraProperties::fp16)
        .def_readwrite("previewHeight", &CameraProperties::previewHeight)
        .def_readwrite("previewWidth", &CameraProperties::previewWidth)
        .def_readwrite("videoHeight", &CameraProperties::videoHeight)
        .def_readwrite("videoWidth", &CameraProperties::videoWidth)
        .def_readwrite("stillHeight", &CameraProperties::stillHeight)
        .def_readwrite("stillWidth", &CameraProperties::stillWidth)
        // .def_readwrite("resolution", &CameraProperties::resolution)
        .def_readwrite("fps", &CameraProperties::fps)
        .def_readwrite("sensorCropX", &CameraProperties::sensorCropX)
        .def_readwrite("sensorCropY", &CameraProperties::sensorCropY)
        .def_readwrite("previewKeepAspectRatio", &CameraProperties::previewKeepAspectRatio)
        .def_readwrite("ispScale", &CameraProperties::ispScale)
        .def_readwrite("numFramesPoolRaw", &CameraProperties::numFramesPoolRaw)
        .def_readwrite("numFramesPoolIsp", &CameraProperties::numFramesPoolIsp)
        .def_readwrite("numFramesPoolVideo", &CameraProperties::numFramesPoolVideo)
        .def_readwrite("numFramesPoolPreview", &CameraProperties::numFramesPoolPreview)
        .def_readwrite("numFramesPoolStill", &CameraProperties::numFramesPoolStill)
    ;

    // Camera node
    camera
        .def_readonly("inputConfig", &Camera::inputConfig, DOC(dai, node, Camera, inputConfig))
        .def_readonly("inputControl", &Camera::inputControl, DOC(dai, node, Camera, inputControl))
        .def_readonly("initialControl", &Camera::initialControl, DOC(dai, node, Camera, initialControl))
        .def_readonly("video", &Camera::video, DOC(dai, node, Camera, video))
        .def_readonly("preview", &Camera::preview, DOC(dai, node, Camera, preview))
        .def_readonly("still", &Camera::still, DOC(dai, node, Camera, still))
        .def_readonly("isp", &Camera::isp, DOC(dai, node, Camera, isp))
        .def_readonly("raw", &Camera::raw, DOC(dai, node, Camera, raw))
        .def_readonly("frameEvent",  &Camera::frameEvent, DOC(dai, node, Camera, frameEvent))
        .def("setCamId", [](Camera& c, int64_t id) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCamId() is deprecated, use setBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            c.setCamId(id);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("getCamId", [](Camera& c) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "getCamId() is deprecated, use getBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return c.getCamId();
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setBoardSocket", &Camera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, Camera, setBoardSocket))
        .def("getBoardSocket", &Camera::getBoardSocket, DOC(dai, node, Camera, getBoardSocket))
        .def("setImageOrientation", &Camera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, Camera, setImageOrientation))
        .def("getImageOrientation", &Camera::getImageOrientation, DOC(dai, node, Camera, getImageOrientation))
        .def("setColorOrder", &Camera::setColorOrder, py::arg("colorOrder"), DOC(dai, node, Camera, setColorOrder))
        .def("getColorOrder", &Camera::getColorOrder, DOC(dai, node, Camera, getColorOrder))
        .def("setInterleaved", &Camera::setInterleaved, py::arg("interleaved"), DOC(dai, node, Camera, setInterleaved))
        .def("getInterleaved", &Camera::getInterleaved, DOC(dai, node, Camera, getInterleaved))
        .def("setFp16", &Camera::setFp16, py::arg("fp16"), DOC(dai, node, Camera, setFp16))
        .def("getFp16", &Camera::getFp16, DOC(dai, node, Camera, getFp16))
        .def("setPreviewSize", static_cast<void(Camera::*)(int,int)>(&Camera::setPreviewSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setPreviewSize))
        .def("setPreviewSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setPreviewSize), py::arg("size"), DOC(dai, node, Camera, setPreviewSize, 2))
        .def("setVideoSize", static_cast<void(Camera::*)(int,int)>(&Camera::setVideoSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setVideoSize))
        .def("setVideoSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setVideoSize), py::arg("size"), DOC(dai, node, Camera, setVideoSize, 2))
        .def("setStillSize", static_cast<void(Camera::*)(int,int)>(&Camera::setStillSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setStillSize))
        .def("setStillSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setStillSize), py::arg("size"), DOC(dai, node, Camera, setStillSize, 2))
        // .def("setResolution", &Camera::setResolution, py::arg("resolution"), DOC(dai, node, Camera, setResolution))
        // .def("getResolution", &Camera::getResolution, DOC(dai, node, Camera, getResolution))
        .def("setFps", &Camera::setFps, py::arg("fps"), DOC(dai, node, Camera, setFps))
        .def("getFps", &Camera::getFps, DOC(dai, node, Camera, getFps))
        .def("getPreviewSize", &Camera::getPreviewSize, DOC(dai, node, Camera, getPreviewSize))
        .def("getPreviewWidth", &Camera::getPreviewWidth, DOC(dai, node, Camera, getPreviewWidth))
        .def("getPreviewHeight", &Camera::getPreviewHeight, DOC(dai, node, Camera, getPreviewHeight))
        .def("getVideoSize", &Camera::getVideoSize, DOC(dai, node, Camera, getVideoSize))
        .def("getVideoWidth", &Camera::getVideoWidth, DOC(dai, node, Camera, getVideoWidth))
        .def("getVideoHeight", &Camera::getVideoHeight, DOC(dai, node, Camera, getVideoHeight))
        .def("getStillSize", &Camera::getStillSize, DOC(dai, node, Camera, getStillSize))
        .def("getStillWidth", &Camera::getStillWidth, DOC(dai, node, Camera, getStillWidth))
        .def("getStillHeight", &Camera::getStillHeight, DOC(dai, node, Camera, getStillHeight))
        .def("getResolutionSize", &Camera::getResolutionSize, DOC(dai, node, Camera, getResolutionSize))
        .def("getResolutionWidth", &Camera::getResolutionWidth, DOC(dai, node, Camera, getResolutionWidth))
        .def("getResolutionHeight", &Camera::getResolutionHeight, DOC(dai, node, Camera, getResolutionHeight))
        .def("sensorCenterCrop", &Camera::sensorCenterCrop, DOC(dai, node, Camera, sensorCenterCrop))
        .def("setSensorCrop", &Camera::setSensorCrop, py::arg("x"), py::arg("y"), DOC(dai, node, Camera, setSensorCrop))
        .def("getSensorCrop", &Camera::getSensorCrop, DOC(dai, node, Camera, getSensorCrop))
        .def("getSensorCropX", &Camera::getSensorCropX, DOC(dai, node, Camera, getSensorCropX))
        .def("getSensorCropY", &Camera::getSensorCropY, DOC(dai, node, Camera, getSensorCropY))

        .def("setWaitForConfigInput", [](Camera& cam, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            cam.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, Camera, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](Camera& cam){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return cam.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, Camera, getWaitForConfigInput))

        .def("setPreviewKeepAspectRatio", &Camera::setPreviewKeepAspectRatio, py::arg("keep"), DOC(dai, node, Camera, setPreviewKeepAspectRatio))
        .def("getPreviewKeepAspectRatio", &Camera::getPreviewKeepAspectRatio, DOC(dai, node, Camera, getPreviewKeepAspectRatio))
        .def("setIspScale", static_cast<void(Camera::*)(int,int)>(&Camera::setIspScale), py::arg("numerator"), py::arg("denominator"), DOC(dai, node, Camera, setIspScale))
        .def("setIspScale", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setIspScale), py::arg("scale"), DOC(dai, node, Camera, setIspScale, 2))
        .def("setIspScale", static_cast<void(Camera::*)(int,int,int,int)>(&Camera::setIspScale), py::arg("horizNum"), py::arg("horizDenom"), py::arg("vertNum"), py::arg("vertDenom"), DOC(dai, node, Camera, setIspScale, 3))
        .def("setIspScale", static_cast<void(Camera::*)(std::tuple<int,int>,std::tuple<int,int>)>(&Camera::setIspScale), py::arg("horizScale"), py::arg("vertScale"), DOC(dai, node, Camera, setIspScale, 4))
        .def("getIspSize", &Camera::getIspSize, DOC(dai, node, Camera, getIspSize))
        .def("getIspWidth", &Camera::getIspWidth, DOC(dai, node, Camera, getIspWidth))
        .def("getIspHeight", &Camera::getIspHeight, DOC(dai, node, Camera, getIspHeight))

        .def("setPreviewNumFramesPool", &Camera::setPreviewNumFramesPool, DOC(dai, node, Camera, setPreviewNumFramesPool))
        .def("setVideoNumFramesPool", &Camera::setVideoNumFramesPool, DOC(dai, node, Camera, setVideoNumFramesPool))
        .def("setStillNumFramesPool", &Camera::setStillNumFramesPool, DOC(dai, node, Camera, setStillNumFramesPool))
        .def("setRawNumFramesPool", &Camera::setRawNumFramesPool, DOC(dai, node, Camera, setRawNumFramesPool))
        .def("setIspNumFramesPool", &Camera::setIspNumFramesPool, DOC(dai, node, Camera, setIspNumFramesPool))
        .def("setNumFramesPool", &Camera::setNumFramesPool, py::arg("raw"), py::arg("isp"), py::arg("preview"), py::arg("video"), py::arg("still"), DOC(dai, node, Camera, setNumFramesPool))

        .def("getPreviewNumFramesPool", &Camera::getPreviewNumFramesPool, DOC(dai, node, Camera, getPreviewNumFramesPool))
        .def("getVideoNumFramesPool", &Camera::getVideoNumFramesPool, DOC(dai, node, Camera, getVideoNumFramesPool))
        .def("getStillNumFramesPool", &Camera::getStillNumFramesPool, DOC(dai, node, Camera, getStillNumFramesPool))
        .def("getRawNumFramesPool", &Camera::getRawNumFramesPool, DOC(dai, node, Camera, getRawNumFramesPool))
        .def("getIspNumFramesPool", &Camera::getIspNumFramesPool, DOC(dai, node, Camera, getIspNumFramesPool))
        .def("setCamera", &Camera::setCamera, py::arg("name"), DOC(dai, node, Camera, setCamera))
        .def("getCamera", &Camera::getCamera, DOC(dai, node, Camera, getCamera))

        .def("setSensorSize", static_cast<void(Camera::*)(int,int)>(&Camera::setSensorSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setSensorSize))
        .def("setSensorSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setSensorSize), py::arg("size"), DOC(dai, node, Camera, setSensorSize, 2))

        ;
    // ALIAS
    daiNodeModule.attr("Camera").attr("Properties") = cameraProperties;

}
