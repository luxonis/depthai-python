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
    py::enum_<CameraProperties::WarpMeshSource> cameraPropertiesWarpMeshSource(cameraProperties, "WarpMeshSource", DOC(dai, CameraProperties, WarpMeshSource));
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

    // Camera Properties - WarpMeshSource
    cameraPropertiesWarpMeshSource
        .value("AUTO", CameraProperties::WarpMeshSource::AUTO)
        .value("NONE", CameraProperties::WarpMeshSource::NONE)
        .value("CALIBRATION", CameraProperties::WarpMeshSource::CALIBRATION)
        .value("URI", CameraProperties::WarpMeshSource::URI)
        ;

    cameraPropertiesColorOrder
        .value("BGR", CameraProperties::ColorOrder::BGR)
        .value("RGB", CameraProperties::ColorOrder::RGB)
        ;

    cameraProperties
        .def_readwrite("initialControl", &CameraProperties::initialControl)
        .def_readwrite("boardSocket", &CameraProperties::boardSocket)
        .def_readwrite("cameraName", &CameraProperties::cameraName)
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
        .def_readwrite("resolutionHeight", &CameraProperties::resolutionHeight)
        .def_readwrite("resolutionWidth", &CameraProperties::resolutionWidth)

        .def_readwrite("fps", &CameraProperties::fps)
        .def_readwrite("isp3aFps", &CameraProperties::isp3aFps)
        .def_readwrite("sensorCropX", &CameraProperties::sensorCropX)
        .def_readwrite("sensorCropY", &CameraProperties::sensorCropY)
        .def_readwrite("previewKeepAspectRatio", &CameraProperties::previewKeepAspectRatio)
        .def_readwrite("ispScale", &CameraProperties::ispScale)
        .def_readwrite("sensorType", &CameraProperties::sensorType)

        .def_readwrite("numFramesPoolRaw", &CameraProperties::numFramesPoolRaw)
        .def_readwrite("numFramesPoolIsp", &CameraProperties::numFramesPoolIsp)
        .def_readwrite("numFramesPoolVideo", &CameraProperties::numFramesPoolVideo)
        .def_readwrite("numFramesPoolPreview", &CameraProperties::numFramesPoolPreview)
        .def_readwrite("numFramesPoolStill", &CameraProperties::numFramesPoolStill)

        .def_readwrite("warpMeshSource", &CameraProperties::warpMeshSource)
        .def_readwrite("warpMeshUri", &CameraProperties::warpMeshUri)
        .def_readwrite("warpMeshWidth", &CameraProperties::warpMeshWidth)
        .def_readwrite("warpMeshHeight", &CameraProperties::warpMeshHeight)
        .def_readwrite("calibAlpha", &CameraProperties::calibAlpha)
        .def_readwrite("warpMeshStepWidth", &CameraProperties::warpMeshStepWidth)
        .def_readwrite("warpMeshStepHeight", &CameraProperties::warpMeshStepHeight)
        .def_readwrite("rawPacked", &CameraProperties::rawPacked)
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
        // .def_readonly("mockIsp",  &Camera::mockIsp, DOC(dai, node, Camera, mockIsp))
        .def("setBoardSocket", &Camera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, Camera, setBoardSocket))
        .def("getBoardSocket", &Camera::getBoardSocket, DOC(dai, node, Camera, getBoardSocket))
        .def("setImageOrientation", &Camera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, Camera, setImageOrientation))
        .def("getImageOrientation", &Camera::getImageOrientation, DOC(dai, node, Camera, getImageOrientation))
        .def("setPreviewSize", static_cast<void(Camera::*)(int,int)>(&Camera::setPreviewSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setPreviewSize))
        .def("setPreviewSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setPreviewSize), py::arg("size"), DOC(dai, node, Camera, setPreviewSize, 2))
        .def("setVideoSize", static_cast<void(Camera::*)(int,int)>(&Camera::setVideoSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setVideoSize))
        .def("setVideoSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setVideoSize), py::arg("size"), DOC(dai, node, Camera, setVideoSize, 2))
        .def("setStillSize", static_cast<void(Camera::*)(int,int)>(&Camera::setStillSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setStillSize))
        .def("setStillSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setStillSize), py::arg("size"), DOC(dai, node, Camera, setStillSize, 2))
        // .def("setResolution", &Camera::setResolution, py::arg("resolution"), DOC(dai, node, Camera, setResolution))
        // .def("getResolution", &Camera::getResolution, DOC(dai, node, Camera, getResolution))
        .def("setFps", &Camera::setFps, py::arg("fps"), DOC(dai, node, Camera, setFps))
        .def("setIsp3aFps", &Camera::setIsp3aFps, DOC(dai, node, Camera, setIsp3aFps))
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
        .def("getSize", &Camera::getSize, DOC(dai, node, Camera, getSize))
        .def("getWidth", &Camera::getWidth, DOC(dai, node, Camera, getWidth))
        .def("getHeight", &Camera::getHeight, DOC(dai, node, Camera, getHeight))
        // .def("sensorCenterCrop", &Camera::sensorCenterCrop, DOC(dai, node, Camera, sensorCenterCrop))
        // .def("setSensorCrop", &Camera::setSensorCrop, py::arg("x"), py::arg("y"), DOC(dai, node, Camera, setSensorCrop))
        // .def("getSensorCrop", &Camera::getSensorCrop, DOC(dai, node, Camera, getSensorCrop))
        // .def("getSensorCropX", &Camera::getSensorCropX, DOC(dai, node, Camera, getSensorCropX))
        // .def("getSensorCropY", &Camera::getSensorCropY, DOC(dai, node, Camera, getSensorCropY))

        // .def("setIspScale", static_cast<void(Camera::*)(int,int)>(&Camera::setIspScale), py::arg("numerator"), py::arg("denominator"), DOC(dai, node, Camera, setIspScale))
        // .def("setIspScale", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setIspScale), py::arg("scale"), DOC(dai, node, Camera, setIspScale, 2))
        // .def("setIspScale", static_cast<void(Camera::*)(int,int,int,int)>(&Camera::setIspScale), py::arg("horizNum"), py::arg("horizDenom"), py::arg("vertNum"), py::arg("vertDenom"), DOC(dai, node, Camera, setIspScale, 3))
        // .def("setIspScale", static_cast<void(Camera::*)(std::tuple<int,int>,std::tuple<int,int>)>(&Camera::setIspScale), py::arg("horizScale"), py::arg("vertScale"), DOC(dai, node, Camera, setIspScale, 4))

        .def("setCamera", &Camera::setCamera, py::arg("name"), DOC(dai, node, Camera, setCamera))
        .def("getCamera", &Camera::getCamera, DOC(dai, node, Camera, getCamera))

        .def("setSize", static_cast<void(Camera::*)(int,int)>(&Camera::setSize), py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setSize))
        .def("setSize", static_cast<void(Camera::*)(std::tuple<int,int>)>(&Camera::setSize), py::arg("size"), DOC(dai, node, Camera, setSize, 2))


        .def("setMeshSource", &Camera::setMeshSource, py::arg("source"), DOC(dai, node, Camera, setMeshSource))
        .def("getMeshSource", &Camera::getMeshSource, DOC(dai, node, Camera, getMeshSource))
        .def("loadMeshFile", &Camera::loadMeshFile, py::arg("warpMesh"), DOC(dai, node, Camera, loadMeshFile))
        .def("loadMeshData", &Camera::loadMeshData, py::arg("warpMesh"), DOC(dai, node, Camera, loadMeshData))
        .def("setMeshStep", &Camera::setMeshStep, py::arg("width"), py::arg("height"), DOC(dai, node, Camera, setMeshStep))
        .def("getMeshStep", &Camera::getMeshStep, DOC(dai, node, Camera, getMeshStep))
        .def("setCalibrationAlpha", &Camera::setCalibrationAlpha, py::arg("alpha"), DOC(dai, node, Camera, setCalibrationAlpha))
        .def("getCalibrationAlpha", &Camera::getCalibrationAlpha, DOC(dai, node, Camera, getCalibrationAlpha))

        .def("setRawOutputPacked", &Camera::setRawOutputPacked, py::arg("packed"), DOC(dai, node, Camera, setRawOutputPacked))
        ;
    // ALIAS
    daiNodeModule.attr("Camera").attr("Properties") = cameraProperties;

}
