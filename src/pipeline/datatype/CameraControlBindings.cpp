#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/CameraControl.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_cameracontrol(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // py::class_<RawCameraControl, RawBuffer, std::shared_ptr<RawCameraControl>> rawCameraControl(m, "RawCameraControl", DOC(dai, RawCameraControl));
    py::class_<CameraControl, Buffer, std::shared_ptr<CameraControl>> cameraControl(m, "CameraControl", DOC(dai, CameraControl));
    py::enum_<CameraControl::Command> cameraControlCommand(cameraControl, "Command", DOC(dai, CameraControl, Command));
    py::enum_<CameraControl::AutoFocusMode> cameraControlAutoFocusMode(cameraControl, "AutoFocusMode", DOC(dai, CameraControl, AutoFocusMode));
    py::enum_<CameraControl::AutoWhiteBalanceMode> cameraControlAutoWhiteBalanceMode(cameraControl, "AutoWhiteBalanceMode", DOC(dai, CameraControl, AutoWhiteBalanceMode));
    py::enum_<CameraControl::SceneMode> cameraControlSceneMode(cameraControl, "SceneMode", DOC(dai, CameraControl, SceneMode));
    py::enum_<CameraControl::ControlMode> cameraControlControlMode(cameraControl, "ControlMode", DOC(dai, CameraControl, ControlMode));
    py::enum_<CameraControl::CaptureIntent> cameraControlCaptureIntent(cameraControl, "CaptureIntent", DOC(dai, CameraControl, CaptureIntent));
    py::enum_<CameraControl::AntiBandingMode> cameraControlAntiBandingMode(cameraControl, "AntiBandingMode", DOC(dai, CameraControl, AntiBandingMode));
    py::enum_<CameraControl::EffectMode> cameraControlEffectMode(cameraControl, "EffectMode", DOC(dai, CameraControl, EffectMode));
    py::enum_<CameraControl::FrameSyncMode> cameraControlFrameSyncMode(cameraControl, "FrameSyncMode", DOC(dai, CameraControl, FrameSyncMode));

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
std::vector<const char *> camCtrlAttr;
    camCtrlAttr.push_back("Command");
    cameraControlCommand
        .value("START_STREAM", CameraControl::Command::START_STREAM)
        .value("STOP_STREAM", CameraControl::Command::STOP_STREAM)
        .value("STILL_CAPTURE", CameraControl::Command::STILL_CAPTURE)
        .value("MOVE_LENS", CameraControl::Command::MOVE_LENS)
        .value("AF_TRIGGER", CameraControl::Command::AF_TRIGGER)
        .value("AE_MANUAL", CameraControl::Command::AE_MANUAL)
        .value("AE_AUTO", CameraControl::Command::AE_AUTO)
        .value("AWB_MODE", CameraControl::Command::AWB_MODE)
        .value("SCENE_MODE", CameraControl::Command::SCENE_MODE)
        .value("ANTIBANDING_MODE", CameraControl::Command::ANTIBANDING_MODE)
        .value("EXPOSURE_COMPENSATION", CameraControl::Command::EXPOSURE_COMPENSATION)
        .value("AE_LOCK", CameraControl::Command::AE_LOCK)
        .value("AE_TARGET_FPS_RANGE", CameraControl::Command::AE_TARGET_FPS_RANGE)
        .value("AWB_LOCK", CameraControl::Command::AWB_LOCK)
        .value("CAPTURE_INTENT", CameraControl::Command::CAPTURE_INTENT)
        .value("CONTROL_MODE", CameraControl::Command::CONTROL_MODE)
        .value("FRAME_DURATION", CameraControl::Command::FRAME_DURATION)
        .value("SENSITIVITY", CameraControl::Command::SENSITIVITY)
        .value("EFFECT_MODE", CameraControl::Command::EFFECT_MODE)
        .value("AF_MODE", CameraControl::Command::AF_MODE)
        .value("NOISE_REDUCTION_STRENGTH", CameraControl::Command::NOISE_REDUCTION_STRENGTH)
        .value("SATURATION", CameraControl::Command::SATURATION)
        .value("BRIGHTNESS", CameraControl::Command::BRIGHTNESS)
        .value("STREAM_FORMAT", CameraControl::Command::STREAM_FORMAT)
        .value("RESOLUTION", CameraControl::Command::RESOLUTION)
        .value("SHARPNESS", CameraControl::Command::SHARPNESS)
        .value("CUSTOM_USECASE", CameraControl::Command::CUSTOM_USECASE)
        .value("CUSTOM_CAPT_MODE", CameraControl::Command::CUSTOM_CAPT_MODE)
        .value("CUSTOM_EXP_BRACKETS", CameraControl::Command::CUSTOM_EXP_BRACKETS)
        .value("CUSTOM_CAPTURE", CameraControl::Command::CUSTOM_CAPTURE)
        .value("CONTRAST", CameraControl::Command::CONTRAST)
        .value("AE_REGION", CameraControl::Command::AE_REGION)
        .value("AF_REGION", CameraControl::Command::AF_REGION)
        .value("LUMA_DENOISE", CameraControl::Command::LUMA_DENOISE)
        .value("CHROMA_DENOISE", CameraControl::Command::CHROMA_DENOISE)
        .value("WB_COLOR_TEMP", CameraControl::Command::WB_COLOR_TEMP)
        ;

    camCtrlAttr.push_back("AutoFocusMode");
    cameraControlAutoFocusMode
        .value("OFF", CameraControl::AutoFocusMode::OFF)
        .value("AUTO", CameraControl::AutoFocusMode::AUTO)
        .value("MACRO", CameraControl::AutoFocusMode::MACRO)
        .value("CONTINUOUS_VIDEO", CameraControl::AutoFocusMode::CONTINUOUS_VIDEO)
        .value("CONTINUOUS_PICTURE", CameraControl::AutoFocusMode::CONTINUOUS_PICTURE)
        .value("EDOF", CameraControl::AutoFocusMode::EDOF)
    ;

    camCtrlAttr.push_back("AutoWhiteBalanceMode");
    cameraControlAutoWhiteBalanceMode
        .value("OFF", CameraControl::AutoWhiteBalanceMode::OFF)
        .value("AUTO", CameraControl::AutoWhiteBalanceMode::AUTO)
        .value("INCANDESCENT", CameraControl::AutoWhiteBalanceMode::INCANDESCENT)
        .value("FLUORESCENT", CameraControl::AutoWhiteBalanceMode::FLUORESCENT)
        .value("WARM_FLUORESCENT", CameraControl::AutoWhiteBalanceMode::WARM_FLUORESCENT)
        .value("DAYLIGHT", CameraControl::AutoWhiteBalanceMode::DAYLIGHT)
        .value("CLOUDY_DAYLIGHT", CameraControl::AutoWhiteBalanceMode::CLOUDY_DAYLIGHT)
        .value("TWILIGHT", CameraControl::AutoWhiteBalanceMode::TWILIGHT)
        .value("SHADE", CameraControl::AutoWhiteBalanceMode::SHADE)
    ;

    camCtrlAttr.push_back("SceneMode");
    cameraControlSceneMode
        .value("UNSUPPORTED", CameraControl::SceneMode::UNSUPPORTED)
        .value("FACE_PRIORITY", CameraControl::SceneMode::FACE_PRIORITY)
        .value("ACTION", CameraControl::SceneMode::ACTION)
        .value("PORTRAIT", CameraControl::SceneMode::PORTRAIT)
        .value("LANDSCAPE", CameraControl::SceneMode::LANDSCAPE)
        .value("NIGHT", CameraControl::SceneMode::NIGHT)
        .value("NIGHT_PORTRAIT", CameraControl::SceneMode::NIGHT_PORTRAIT)
        .value("THEATRE", CameraControl::SceneMode::THEATRE)
        .value("BEACH", CameraControl::SceneMode::BEACH)
        .value("SNOW", CameraControl::SceneMode::SNOW)
        .value("SUNSET", CameraControl::SceneMode::SUNSET)
        .value("STEADYPHOTO", CameraControl::SceneMode::STEADYPHOTO)
        .value("FIREWORKS", CameraControl::SceneMode::FIREWORKS)
        .value("SPORTS", CameraControl::SceneMode::SPORTS)
        .value("PARTY", CameraControl::SceneMode::PARTY)
        .value("CANDLELIGHT", CameraControl::SceneMode::CANDLELIGHT)
        .value("BARCODE", CameraControl::SceneMode::BARCODE)
    ;

    camCtrlAttr.push_back("ControlMode");
    cameraControlControlMode
        .value("OFF", CameraControl::ControlMode::OFF)
        .value("AUTO", CameraControl::ControlMode::AUTO)
        .value("USE_SCENE_MODE", CameraControl::ControlMode::USE_SCENE_MODE)
    ;

    camCtrlAttr.push_back("CaptureIntent");
    cameraControlCaptureIntent
        .value("CUSTOM", CameraControl::CaptureIntent::CUSTOM)
        .value("PREVIEW", CameraControl::CaptureIntent::PREVIEW)
        .value("STILL_CAPTURE", CameraControl::CaptureIntent::STILL_CAPTURE)
        .value("VIDEO_RECORD", CameraControl::CaptureIntent::VIDEO_RECORD)
        .value("VIDEO_SNAPSHOT", CameraControl::CaptureIntent::VIDEO_SNAPSHOT)
        .value("ZERO_SHUTTER_LAG", CameraControl::CaptureIntent::ZERO_SHUTTER_LAG)
    ;

    camCtrlAttr.push_back("AntiBandingMode");
    cameraControlAntiBandingMode
        .value("OFF", CameraControl::AntiBandingMode::OFF)
        .value("MAINS_50_HZ", CameraControl::AntiBandingMode::MAINS_50_HZ)
        .value("MAINS_60_HZ", CameraControl::AntiBandingMode::MAINS_60_HZ)
        .value("AUTO", CameraControl::AntiBandingMode::AUTO)
    ;

    camCtrlAttr.push_back("EffectMode");
    cameraControlEffectMode
        .value("OFF", CameraControl::EffectMode::OFF)
        .value("MONO", CameraControl::EffectMode::MONO)
        .value("NEGATIVE", CameraControl::EffectMode::NEGATIVE)
        .value("SOLARIZE", CameraControl::EffectMode::SOLARIZE)
        .value("SEPIA", CameraControl::EffectMode::SEPIA)
        .value("POSTERIZE", CameraControl::EffectMode::POSTERIZE)
        .value("WHITEBOARD", CameraControl::EffectMode::WHITEBOARD)
        .value("BLACKBOARD", CameraControl::EffectMode::BLACKBOARD)
        .value("AQUA", CameraControl::EffectMode::AQUA)
    ;

    camCtrlAttr.push_back("FrameSyncMode");
    cameraControlFrameSyncMode
        .value("OFF", CameraControl::FrameSyncMode::OFF)
        .value("OUTPUT", CameraControl::FrameSyncMode::OUTPUT)
        .value("INPUT", CameraControl::FrameSyncMode::INPUT)
    ;

    cameraControl
        .def(py::init<>())
        .def_readwrite("cmdMask", &CameraControl::cmdMask)
        .def_readwrite("autoFocusMode", &CameraControl::autoFocusMode)
        .def_readwrite("lensPosition", &CameraControl::lensPosition)
        .def_readwrite("expManual", &CameraControl::expManual)
        .def_readwrite("afRegion", &CameraControl::afRegion)
        .def_readwrite("awbMode", &CameraControl::awbMode)
        .def_readwrite("sceneMode", &CameraControl::sceneMode)
        .def_readwrite("antiBandingMode", &CameraControl::antiBandingMode)
        .def_readwrite("captureIntent", &CameraControl::captureIntent)
        .def_readwrite("controlMode", &CameraControl::controlMode)
        .def_readwrite("effectMode", &CameraControl::effectMode)
        .def_readwrite("aeMaxExposureTimeUs", &CameraControl::aeMaxExposureTimeUs)
        .def_readwrite("aeLockMode", &CameraControl::aeLockMode)
        .def_readwrite("awbLockMode", &CameraControl::awbLockMode)
        .def_readwrite("expCompensation", &CameraControl::expCompensation)
        .def_readwrite("brightness", &CameraControl::brightness)
        .def_readwrite("contrast", &CameraControl::contrast)
        .def_readwrite("saturation", &CameraControl::saturation)
        .def_readwrite("sharpness", &CameraControl::sharpness)
        .def_readwrite("lumaDenoise", &CameraControl::lumaDenoise)
        .def_readwrite("chromaDenoise", &CameraControl::chromaDenoise)
        .def_readwrite("wbColorTemp", &CameraControl::wbColorTemp)
        .def("setCommand", &CameraControl::setCommand)
        .def("clearCommand", &CameraControl::clearCommand)
        .def("getCommand", &CameraControl::getCommand)
        ;

    // Message
    cameraControl
        .def(py::init<>(), DOC(dai, CameraControl, CameraControl))
        // setters
        .def("setCaptureStill", &CameraControl::setCaptureStill, py::arg("capture"), DOC(dai, CameraControl, setCaptureStill))
        .def("setStartStreaming", &CameraControl::setStartStreaming, DOC(dai, CameraControl, setStartStreaming))
        .def("setStopStreaming", &CameraControl::setStopStreaming, DOC(dai, CameraControl, setStopStreaming))
        .def("setExternalTrigger", &CameraControl::setExternalTrigger, py::arg("numFramesBurst"), py::arg("numFramesDiscard"), DOC(dai, CameraControl, setExternalTrigger))
        .def("setFrameSyncMode", &CameraControl::setFrameSyncMode, py::arg("mode"), DOC(dai, CameraControl, setFrameSyncMode))
        .def("setStrobeSensor", &CameraControl::setStrobeSensor, py::arg("activeLevel"), DOC(dai, CameraControl, setStrobeSensor))
        .def("setStrobeExternal", &CameraControl::setStrobeExternal, py::arg("gpioNumber"), py::arg("activeLevel"), DOC(dai, CameraControl, setStrobeExternal))
        .def("setStrobeDisable", &CameraControl::setStrobeDisable, DOC(dai, CameraControl, setStrobeDisable))
        .def("setAutoFocusMode", &CameraControl::setAutoFocusMode, py::arg("mode"), DOC(dai, CameraControl, setAutoFocusMode))
        .def("setAutoFocusTrigger", &CameraControl::setAutoFocusTrigger, DOC(dai, CameraControl, setAutoFocusTrigger))
        .def("setAutoFocusLensRange", &CameraControl::setAutoFocusLensRange, py::arg("infinityPosition"), py::arg("macroPosition"), DOC(dai, CameraControl, setAutoFocusLensRange))
        .def("setAutoFocusRegion", &CameraControl::setAutoFocusRegion, py::arg("startX"), py::arg("startY"), py::arg("width"), py::arg("height"), DOC(dai, CameraControl, setAutoFocusRegion))
        .def("setManualFocus", &CameraControl::setManualFocus, py::arg("lensPosition"), DOC(dai, CameraControl, setManualFocus))
        .def("setAutoExposureEnable", &CameraControl::setAutoExposureEnable, DOC(dai, CameraControl, setAutoExposureEnable))
        .def("setAutoExposureLock", &CameraControl::setAutoExposureLock, py::arg("lock"), DOC(dai, CameraControl, setAutoExposureLock))
        .def("setAutoExposureRegion", &CameraControl::setAutoExposureRegion, py::arg("startX"), py::arg("startY"), py::arg("width"), py::arg("height"), DOC(dai, CameraControl, setAutoExposureRegion))
        .def("setAutoExposureCompensation", &CameraControl::setAutoExposureCompensation, py::arg("compensation"), DOC(dai, CameraControl, setAutoExposureCompensation))
        .def("setAutoExposureLimit", py::overload_cast<uint32_t>(&CameraControl::setAutoExposureLimit), py::arg("maxExposureTimeUs"), DOC(dai, CameraControl, setAutoExposureLimit))
        .def("setAutoExposureLimit", py::overload_cast<std::chrono::microseconds>(&CameraControl::setAutoExposureLimit), py::arg("maxExposureTime"), DOC(dai, CameraControl, setAutoExposureLimit, 2))
        .def("setAntiBandingMode", &CameraControl::setAntiBandingMode, py::arg("mode"), DOC(dai, CameraControl, setAntiBandingMode))
        .def("setManualExposure", py::overload_cast<uint32_t, uint32_t>(&CameraControl::setManualExposure), py::arg("exposureTimeUs"), py::arg("sensitivityIso"), DOC(dai, CameraControl, setManualExposure))
        .def("setManualExposure", py::overload_cast<std::chrono::microseconds, uint32_t>(&CameraControl::setManualExposure), py::arg("exposureTime"), py::arg("sensitivityIso"), DOC(dai, CameraControl, setManualExposure, 2))
        .def("setAutoWhiteBalanceMode", &CameraControl::setAutoWhiteBalanceMode, py::arg("mode"), DOC(dai, CameraControl, setAutoWhiteBalanceMode))
        .def("setAutoWhiteBalanceLock", &CameraControl::setAutoWhiteBalanceLock, py::arg("lock"), DOC(dai, CameraControl, setAutoWhiteBalanceLock))
        .def("setManualWhiteBalance", &CameraControl::setManualWhiteBalance, py::arg("colorTemperatureK"), DOC(dai, CameraControl, setManualWhiteBalance))
        .def("setBrightness", &CameraControl::setBrightness, py::arg("value"), DOC(dai, CameraControl, setBrightness))
        .def("setContrast", &CameraControl::setContrast, py::arg("value"), DOC(dai, CameraControl, setContrast))
        .def("setSaturation", &CameraControl::setSaturation, py::arg("value"), DOC(dai, CameraControl, setSaturation))
        .def("setSharpness", &CameraControl::setSharpness, py::arg("value"), DOC(dai, CameraControl, setSharpness))
        .def("setLumaDenoise", &CameraControl::setLumaDenoise, py::arg("value"), DOC(dai, CameraControl, setLumaDenoise))
        .def("setChromaDenoise", &CameraControl::setChromaDenoise, py::arg("value"), DOC(dai, CameraControl, setChromaDenoise))
        .def("setSceneMode", &CameraControl::setSceneMode, py::arg("mode"), DOC(dai, CameraControl, setSceneMode))
        .def("setEffectMode", &CameraControl::setEffectMode, py::arg("mode"), DOC(dai, CameraControl, setEffectMode))
        .def("setMisc", py::overload_cast<std::string, std::string>(&CameraControl::setMisc), py::arg("control"), py::arg("value"), DOC(dai, CameraControl, setMisc))
        .def("setMisc", py::overload_cast<std::string, int>(&CameraControl::setMisc), py::arg("control"), py::arg("value"), DOC(dai, CameraControl, setMisc, 2))
        .def("setMisc", py::overload_cast<std::string, float>(&CameraControl::setMisc), py::arg("control"), py::arg("value"), DOC(dai, CameraControl, setMisc, 3))
        .def("clearMiscControls", &CameraControl::clearMiscControls, DOC(dai, CameraControl, clearMiscControls))
        .def("setControlMode", &CameraControl::setControlMode, py::arg("mode"), DOC(dai, CameraControl, setControlMode))
        .def("setCaptureIntent", &CameraControl::setCaptureIntent, py::arg("mode"), DOC(dai, CameraControl, setCaptureIntent))
        // .def("set",         &CameraControl::set, py::arg("config"), DOC(dai, CameraControl, set))
        // getters
        .def("getMiscControls", &CameraControl::getMiscControls, DOC(dai, CameraControl, getMiscControls))
        .def("getCaptureStill", &CameraControl::getCaptureStill, DOC(dai, CameraControl, getCaptureStill))
        .def("getExposureTime", &CameraControl::getExposureTime, DOC(dai, CameraControl, getExposureTime))
        .def("getSensitivity", &CameraControl::getSensitivity, DOC(dai, CameraControl, getSensitivity))
        .def("getLensPosition", &CameraControl::getLensPosition, DOC(dai, CameraControl, getLensPosition))
        // .def("get",         &CameraControl::get, DOC(dai, CameraControl, get))
        ;
    // Add also enum attributes from CameraControl
    for (const auto& a : camCtrlAttr) {
        m.attr("CameraControl").attr(a) = m.attr("CameraControl").attr(a);
    }


}
