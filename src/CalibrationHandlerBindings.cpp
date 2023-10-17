#include "CalibrationHandlerBindings.hpp"
#include "depthai/device/CalibrationHandler.hpp"
#include "depthai-shared/common/Point2f.hpp"
#include <vector>

void CalibrationHandlerBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Type definitions
    py::class_<CalibrationHandler> calibrationHandler(m, "CalibrationHandler", DOC(dai, CalibrationHandler));

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Bindings
    calibrationHandler
        .def(py::init<>(), DOC(dai, CalibrationHandler, CalibrationHandler))
        .def(py::init<dai::Path>(), DOC(dai, CalibrationHandler, CalibrationHandler, 2))
        .def(py::init<dai::Path, dai::Path>(), DOC(dai, CalibrationHandler, CalibrationHandler, 3))
        .def(py::init<EepromData>(), DOC(dai, CalibrationHandler, CalibrationHandler, 4))

        .def_static("fromJson", &CalibrationHandler::fromJson, DOC(dai, CalibrationHandler, fromJson))

        .def("getEepromData", &CalibrationHandler::getEepromData, DOC(dai, CalibrationHandler, getEepromData))

        .def("getCameraIntrinsics", py::overload_cast<CameraBoardSocket, int, int, Point2f, Point2f, bool>(&CalibrationHandler::getCameraIntrinsics, py::const_), py::arg("cameraId"), py::arg("resizeWidth") = -1, py::arg("resizeHeight") = -1, py::arg("topLeftPixelId") = Point2f(), py::arg("bottomRightPixelId") = Point2f(), py::arg("keepAspectRatio") = true, DOC(dai, CalibrationHandler, getCameraIntrinsics))
        .def("getCameraIntrinsics", py::overload_cast<CameraBoardSocket, Size2f, Point2f, Point2f, bool>(&CalibrationHandler::getCameraIntrinsics, py::const_), py::arg("cameraId"), py::arg("destShape"), py::arg("topLeftPixelId") = Point2f(), py::arg("bottomRightPixelId") = Point2f(), py::arg("keepAspectRatio") = true, DOC(dai, CalibrationHandler, getCameraIntrinsics, 2))
        .def("getCameraIntrinsics", py::overload_cast<CameraBoardSocket, std::tuple<int, int>, Point2f, Point2f, bool>(&CalibrationHandler::getCameraIntrinsics, py::const_), py::arg("cameraId"), py::arg("destShape"), py::arg("topLeftPixelId") = Point2f(), py::arg("bottomRightPixelId") = Point2f(), py::arg("keepAspectRatio") = true, DOC(dai, CalibrationHandler, getCameraIntrinsics, 3))

        .def("getDefaultIntrinsics", &CalibrationHandler::getDefaultIntrinsics, py::arg("cameraId"), DOC(dai, CalibrationHandler, getDefaultIntrinsics))
        .def("getDistortionCoefficients", &CalibrationHandler::getDistortionCoefficients, py::arg("cameraId"), DOC(dai, CalibrationHandler, getDistortionCoefficients))

        .def("getFov", &CalibrationHandler::getFov, py::arg("cameraId"), py::arg("useSpec") = true, DOC(dai, CalibrationHandler, getFov))
        .def("getLensPosition", &CalibrationHandler::getLensPosition, py::arg("cameraId"), DOC(dai, CalibrationHandler, getLensPosition))
        .def("getDistortionModel", &CalibrationHandler::getDistortionModel, py::arg("cameraId"), DOC(dai, CalibrationHandler, getDistortionModel))

        .def("getCameraExtrinsics", &CalibrationHandler::getCameraExtrinsics, py::arg("srcCamera"), py::arg("dstCamera"), py::arg("useSpecTranslation") = false, DOC(dai, CalibrationHandler, getCameraExtrinsics))

        .def("getCameraTranslationVector", &CalibrationHandler::getCameraTranslationVector, py::arg("srcCamera"), py::arg("dstCamera"), py::arg("useSpecTranslation") = true, DOC(dai, CalibrationHandler, getCameraTranslationVector))
        .def("getCameraRotationMatrix", &CalibrationHandler::getCameraRotationMatrix, py::arg("srcCamera"), py::arg("dstCamera"), DOC(dai, CalibrationHandler, getCameraRotationMatrix))
        .def("getBaselineDistance", &CalibrationHandler::getBaselineDistance, py::arg("cam1") = dai::CameraBoardSocket::CAM_C, py::arg("cam2") = dai::CameraBoardSocket::CAM_B, py::arg("useSpecTranslation") = true, DOC(dai, CalibrationHandler, getBaselineDistance))

        .def("getCameraToImuExtrinsics", &CalibrationHandler::getCameraToImuExtrinsics, py::arg("cameraId"), py::arg("useSpecTranslation") = false, DOC(dai, CalibrationHandler, getCameraToImuExtrinsics))
        .def("getImuToCameraExtrinsics", &CalibrationHandler::getImuToCameraExtrinsics, py::arg("cameraId"), py::arg("useSpecTranslation") = false, DOC(dai, CalibrationHandler, getImuToCameraExtrinsics))

        .def("getStereoRightRectificationRotation", &CalibrationHandler::getStereoRightRectificationRotation, DOC(dai, CalibrationHandler, getStereoRightRectificationRotation))
        .def("getStereoLeftRectificationRotation", &CalibrationHandler::getStereoLeftRectificationRotation, DOC(dai, CalibrationHandler, getStereoLeftRectificationRotation))
        .def("getStereoLeftCameraId", &CalibrationHandler::getStereoLeftCameraId, DOC(dai, CalibrationHandler, getStereoLeftCameraId))
        .def("getStereoRightCameraId", &CalibrationHandler::getStereoRightCameraId, DOC(dai, CalibrationHandler, getStereoRightCameraId))

        .def("eepromToJsonFile", &CalibrationHandler::eepromToJsonFile, py::arg("destPath"), DOC(dai, CalibrationHandler, eepromToJsonFile))
        .def("eepromToJson", &CalibrationHandler::eepromToJson, DOC(dai, CalibrationHandler, eepromToJson))

        .def("setBoardInfo", py::overload_cast<std::string, std::string>(&CalibrationHandler::setBoardInfo), py::arg("boardName"), py::arg("boardRev"), DOC(dai, CalibrationHandler, setBoardInfo))
        .def("setBoardInfo", py::overload_cast<std::string, std::string, std::string, std::string, std::string, std::string, uint64_t, uint32_t, std::string>(&CalibrationHandler::setBoardInfo), py::arg("productName"), py::arg("boardName"), py::arg("boardRev"), py::arg("boardConf"), py::arg("hardwareConf"), py::arg("batchName"), py::arg("batchTime"), py::arg("boardOptions"), py::arg("boardCustom") = "", DOC(dai, CalibrationHandler, setBoardInfo, 2))
        .def("setProductName", &CalibrationHandler::setProductName, py::arg("productName"), DOC(dai, CalibrationHandler, setProductName))

        .def("setCameraIntrinsics", py::overload_cast<CameraBoardSocket, std::vector<std::vector<float>>, Size2f>(&CalibrationHandler::setCameraIntrinsics), py::arg("cameraId"), py::arg("intrinsics"), py::arg("frameSize"), DOC(dai, CalibrationHandler, setCameraIntrinsics))
        .def("setCameraIntrinsics", py::overload_cast<CameraBoardSocket, std::vector<std::vector<float>>, int, int>(&CalibrationHandler::setCameraIntrinsics), py::arg("cameraId"), py::arg("intrinsics"), py::arg("width"), py::arg("height"), DOC(dai, CalibrationHandler, setCameraIntrinsics, 2))
        .def("setCameraIntrinsics", py::overload_cast<CameraBoardSocket, std::vector<std::vector<float>>, std::tuple<int, int>>(&CalibrationHandler::setCameraIntrinsics), py::arg("cameraId"), py::arg("intrinsics"), py::arg("frameSize"), DOC(dai, CalibrationHandler, setCameraIntrinsics, 3))

        .def("setDistortionCoefficients", &CalibrationHandler::setDistortionCoefficients, py::arg("cameraId"), py::arg("distortionCoefficients"), DOC(dai, CalibrationHandler, setDistortionCoefficients))
        .def("setFov", &CalibrationHandler::setFov, py::arg("cameraId"), py::arg("hfov"), DOC(dai, CalibrationHandler, setFov))

        .def("setLensPosition", &CalibrationHandler::setLensPosition, py::arg("cameraId"), py::arg("lensPosition"), DOC(dai, CalibrationHandler, setLensPosition))
        .def("setCameraType", &CalibrationHandler::setCameraType, py::arg("cameraId"), py::arg("cameraModel"), DOC(dai, CalibrationHandler, setCameraType))

        .def("setCameraExtrinsics", &CalibrationHandler::setCameraExtrinsics, py::arg("srcCameraId"), py::arg("destCameraId"), py::arg("rotationMatrix"), py::arg("translation"), py::arg("specTranslation") = std::vector<float>(3,0), DOC(dai, CalibrationHandler, setCameraExtrinsics))
        .def("setImuExtrinsics", &CalibrationHandler::setImuExtrinsics, py::arg("destCameraId"), py::arg("rotationMatrix"), py::arg("translation"), py::arg("specTranslation") = std::vector<float>(3,0), DOC(dai, CalibrationHandler, setImuExtrinsics))

        .def("setStereoLeft", &CalibrationHandler::setStereoLeft, py::arg("cameraId"), py::arg("rectifiedRotation"), DOC(dai, CalibrationHandler, setStereoLeft))
        .def("setStereoRight", &CalibrationHandler::setStereoRight, py::arg("cameraId"), py::arg("rectifiedRotation"), DOC(dai, CalibrationHandler, setStereoRight));


}