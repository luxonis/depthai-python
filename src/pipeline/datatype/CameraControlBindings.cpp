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

    py::class_<RawCameraControl, RawBuffer, std::shared_ptr<RawCameraControl>> rawCameraControl(m, "RawCameraControl", DOC(dai, RawCameraControl));
    py::enum_<RawCameraControl::Command> rawCameraControlCommand(rawCameraControl, "Command", DOC(dai, RawCameraControl, Command));
    py::enum_<RawCameraControl::AutoFocusMode> rawCameraControlAutoFocusMode(rawCameraControl, "AutoFocusMode", DOC(dai, RawCameraControl, AutoFocusMode));
    py::enum_<RawCameraControl::AutoWhiteBalanceMode> rawCameraControlAutoWhiteBalanceMode(rawCameraControl, "AutoWhiteBalanceMode", DOC(dai, RawCameraControl, AutoWhiteBalanceMode));
    py::enum_<RawCameraControl::SceneMode> rawCameraControlSceneMode(rawCameraControl, "SceneMode", DOC(dai, RawCameraControl, SceneMode));
    py::enum_<RawCameraControl::ControlMode> rawCameraControlControlMode(rawCameraControl, "ControlMode", DOC(dai, RawCameraControl, ControlMode));
    py::enum_<RawCameraControl::CaptureIntent> rawCameraControlCaptureIntent(rawCameraControl, "CaptureIntent", DOC(dai, RawCameraControl, CaptureIntent));
    py::enum_<RawCameraControl::AntiBandingMode> rawCameraControlAntiBandingMode(rawCameraControl, "AntiBandingMode", DOC(dai, RawCameraControl, AntiBandingMode));
    py::enum_<RawCameraControl::EffectMode> rawCameraControlEffectMode(rawCameraControl, "EffectMode", DOC(dai, RawCameraControl, EffectMode));
    py::enum_<RawCameraControl::FrameSyncMode> rawCameraControlFrameSyncMode(rawCameraControl, "FrameSyncMode", DOC(dai, RawCameraControl, FrameSyncMode));
    py::class_<CameraControl, Buffer, std::shared_ptr<CameraControl>> cameraControl(m, "CameraControl", DOC(dai, CameraControl));

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
    rawCameraControlCommand
        .value("START_STREAM", RawCameraControl::Command::START_STREAM)
        .value("STOP_STREAM", RawCameraControl::Command::STOP_STREAM)
        .value("STILL_CAPTURE", RawCameraControl::Command::STILL_CAPTURE)
        .value("MOVE_LENS", RawCameraControl::Command::MOVE_LENS)
        .value("AF_TRIGGER", RawCameraControl::Command::AF_TRIGGER)
        .value("AE_MANUAL", RawCameraControl::Command::AE_MANUAL)
        .value("AE_AUTO", RawCameraControl::Command::AE_AUTO)
        .value("AWB_MODE", RawCameraControl::Command::AWB_MODE)
        .value("SCENE_MODE", RawCameraControl::Command::SCENE_MODE)
        .value("ANTIBANDING_MODE", RawCameraControl::Command::ANTIBANDING_MODE)
        .value("EXPOSURE_COMPENSATION", RawCameraControl::Command::EXPOSURE_COMPENSATION)
        .value("AE_LOCK", RawCameraControl::Command::AE_LOCK)
        .value("AE_TARGET_FPS_RANGE", RawCameraControl::Command::AE_TARGET_FPS_RANGE)
        .value("AWB_LOCK", RawCameraControl::Command::AWB_LOCK)
        .value("CAPTURE_INTENT", RawCameraControl::Command::CAPTURE_INTENT)
        .value("CONTROL_MODE", RawCameraControl::Command::CONTROL_MODE)
        .value("FRAME_DURATION", RawCameraControl::Command::FRAME_DURATION)
        .value("SENSITIVITY", RawCameraControl::Command::SENSITIVITY)
        .value("EFFECT_MODE", RawCameraControl::Command::EFFECT_MODE)
        .value("AF_MODE", RawCameraControl::Command::AF_MODE)
        .value("NOISE_REDUCTION_STRENGTH", RawCameraControl::Command::NOISE_REDUCTION_STRENGTH)
        .value("SATURATION", RawCameraControl::Command::SATURATION)
        .value("BRIGHTNESS", RawCameraControl::Command::BRIGHTNESS)
        .value("STREAM_FORMAT", RawCameraControl::Command::STREAM_FORMAT)
        .value("RESOLUTION", RawCameraControl::Command::RESOLUTION)
        .value("SHARPNESS", RawCameraControl::Command::SHARPNESS)
        .value("CUSTOM_USECASE", RawCameraControl::Command::CUSTOM_USECASE)
        .value("CUSTOM_CAPT_MODE", RawCameraControl::Command::CUSTOM_CAPT_MODE)
        .value("CUSTOM_EXP_BRACKETS", RawCameraControl::Command::CUSTOM_EXP_BRACKETS)
        .value("CUSTOM_CAPTURE", RawCameraControl::Command::CUSTOM_CAPTURE)
        .value("CONTRAST", RawCameraControl::Command::CONTRAST)
        .value("AE_REGION", RawCameraControl::Command::AE_REGION)
        .value("AF_REGION", RawCameraControl::Command::AF_REGION)
        .value("LUMA_DENOISE", RawCameraControl::Command::LUMA_DENOISE)
        .value("CHROMA_DENOISE", RawCameraControl::Command::CHROMA_DENOISE)
        .value("WB_COLOR_TEMP", RawCameraControl::Command::WB_COLOR_TEMP)
        ;

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

    camCtrlAttr.push_back("ControlMode");
    rawCameraControlControlMode
        .value("OFF", RawCameraControl::ControlMode::OFF)
        .value("AUTO", RawCameraControl::ControlMode::AUTO)
        .value("USE_SCENE_MODE", RawCameraControl::ControlMode::USE_SCENE_MODE)
    ;

    camCtrlAttr.push_back("CaptureIntent");
    rawCameraControlCaptureIntent
        .value("CUSTOM", RawCameraControl::CaptureIntent::CUSTOM)
        .value("PREVIEW", RawCameraControl::CaptureIntent::PREVIEW)
        .value("STILL_CAPTURE", RawCameraControl::CaptureIntent::STILL_CAPTURE)
        .value("VIDEO_RECORD", RawCameraControl::CaptureIntent::VIDEO_RECORD)
        .value("VIDEO_SNAPSHOT", RawCameraControl::CaptureIntent::VIDEO_SNAPSHOT)
        .value("ZERO_SHUTTER_LAG", RawCameraControl::CaptureIntent::ZERO_SHUTTER_LAG)
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

    camCtrlAttr.push_back("FrameSyncMode");
    rawCameraControlFrameSyncMode
        .value("OFF", RawCameraControl::FrameSyncMode::OFF)
        .value("OUTPUT", RawCameraControl::FrameSyncMode::OUTPUT)
        .value("INPUT", RawCameraControl::FrameSyncMode::INPUT)
    ;

    rawCameraControl
        .def(py::init<>())
        .def_readwrite("cmdMask", &RawCameraControl::cmdMask)
        .def_readwrite("autoFocusMode", &RawCameraControl::autoFocusMode)
        .def_readwrite("lensPosition", &RawCameraControl::lensPosition)
        .def_readwrite("lensPositionRaw", &RawCameraControl::lensPositionRaw)
        .def_readwrite("expManual", &RawCameraControl::expManual)
        .def_readwrite("afRegion", &RawCameraControl::afRegion)
        .def_readwrite("awbMode", &RawCameraControl::awbMode)
        .def_readwrite("sceneMode", &RawCameraControl::sceneMode)
        .def_readwrite("antiBandingMode", &RawCameraControl::antiBandingMode)
        .def_readwrite("captureIntent", &RawCameraControl::captureIntent)
        .def_readwrite("controlMode", &RawCameraControl::controlMode)
        .def_readwrite("effectMode", &RawCameraControl::effectMode)
        .def_readwrite("aeMaxExposureTimeUs", &RawCameraControl::aeMaxExposureTimeUs)
        .def_readwrite("aeLockMode", &RawCameraControl::aeLockMode)
        .def_readwrite("awbLockMode", &RawCameraControl::awbLockMode)
        .def_readwrite("expCompensation", &RawCameraControl::expCompensation)
        .def_readwrite("brightness", &RawCameraControl::brightness)
        .def_readwrite("contrast", &RawCameraControl::contrast)
        .def_readwrite("saturation", &RawCameraControl::saturation)
        .def_readwrite("sharpness", &RawCameraControl::sharpness)
        .def_readwrite("lumaDenoise", &RawCameraControl::lumaDenoise)
        .def_readwrite("chromaDenoise", &RawCameraControl::chromaDenoise)
        .def_readwrite("wbColorTemp", &RawCameraControl::wbColorTemp)
        .def("setCommand", &RawCameraControl::setCommand)
        .def("clearCommand", &RawCameraControl::clearCommand)
        .def("getCommand", &RawCameraControl::getCommand)
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
        .def("setManualFocusRaw", &CameraControl::setManualFocusRaw, py::arg("lensPositionRaw"), DOC(dai, CameraControl, setManualFocusRaw))
        .def("setAutoExposureEnable", &CameraControl::setAutoExposureEnable, DOC(dai, CameraControl, setAutoExposureEnable))
        .def("setAutoExposureLock", &CameraControl::setAutoExposureLock, py::arg("lock"), DOC(dai, CameraControl, setAutoExposureLock))
        .def("setAutoExposureRegion", &CameraControl::setAutoExposureRegion, py::arg("startX"), py::arg("startY"), py::arg("width"), py::arg("height"), DOC(dai, CameraControl, setAutoExposureRegion))
        .def("setAutoExposureCompensation", &CameraControl::setAutoExposureCompensation, py::arg("compensation"), DOC(dai, CameraControl, setAutoExposureCompensation))
        .def("setAutoExposureLimit", py::overload_cast<uint32_t>(&CameraControl::setAutoExposureLimit), py::arg("maxExposureTimeUs"), DOC(dai, CameraControl, setAutoExposureLimit))
        .def("setAutoExposureLimit", py::overload_cast<std::chrono::microseconds>(&CameraControl::setAutoExposureLimit), py::arg("maxExposureTime"), DOC(dai, CameraControl, setAutoExposureLimit, 2))
        .def("setFps", &CameraControl::setFps, py::arg("fps"), DOC(dai, CameraControl, setFps))
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
        .def("setControlMode", &CameraControl::setControlMode, py::arg("mode"), DOC(dai, CameraControl, setControlMode))
        .def("setCaptureIntent", &CameraControl::setCaptureIntent, py::arg("mode"), DOC(dai, CameraControl, setCaptureIntent))
        .def("set",         &CameraControl::set, py::arg("config"), DOC(dai, CameraControl, set))
        // getters
        .def("getCaptureStill", &CameraControl::getCaptureStill, DOC(dai, CameraControl, getCaptureStill))
        .def("getExposureTime", &CameraControl::getExposureTime, DOC(dai, CameraControl, getExposureTime))
        .def("getSensitivity", &CameraControl::getSensitivity, DOC(dai, CameraControl, getSensitivity))
        .def("getLensPosition", &CameraControl::getLensPosition, DOC(dai, CameraControl, getLensPosition))
        .def("getLensPositionRaw", &CameraControl::getLensPositionRaw, DOC(dai, CameraControl, getLensPositionRaw))
        .def("get",         &CameraControl::get, DOC(dai, CameraControl, get))
        ;
    // Add also enum attributes from RawCameraControl
    for (const auto& a : camCtrlAttr) {
        m.attr("CameraControl").attr(a) = m.attr("RawCameraControl").attr(a);
    }


}
