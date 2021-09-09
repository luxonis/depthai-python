#include "CommonBindings.hpp"

// depthai-shared
#include "depthai-shared/common/CameraBoardSocket.hpp"
#include "depthai-shared/common/EepromData.hpp"
#include "depthai-shared/common/CameraImageOrientation.hpp"
#include "depthai-shared/common/MemoryInfo.hpp"
#include "depthai-shared/common/ChipTemperature.hpp"
#include "depthai-shared/common/CpuUsage.hpp"
#include "depthai-shared/common/ProcessorType.hpp"
#include "depthai-shared/common/Timestamp.hpp"
#include "depthai-shared/common/Point2f.hpp"
#include "depthai-shared/common/Point3f.hpp"
#include "depthai-shared/common/Size2f.hpp"
#include "depthai-shared/common/UsbSpeed.hpp"

void CommonBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<Timestamp> timestamp(m, "Timestamp", DOC(dai, Timestamp));
    py::class_<Point2f> point2f(m, "Point2f", DOC(dai, Point2f));
    py::class_<Point3f> point3f(m, "Point3f", DOC(dai, Point3f));
    py::class_<Size2f> size2f(m, "Size2f", DOC(dai, Size2f));
    py::enum_<CameraBoardSocket> cameraBoardSocket(m, "CameraBoardSocket", DOC(dai, CameraBoardSocket));
    py::enum_<CameraImageOrientation> cameraImageOrientation(m, "CameraImageOrientation", DOC(dai, CameraImageOrientation));
    py::class_<MemoryInfo> memoryInfo(m, "MemoryInfo", DOC(dai, MemoryInfo));
    py::class_<ChipTemperature> chipTemperature(m, "ChipTemperature", DOC(dai, ChipTemperature));
    py::class_<CpuUsage> cpuUsage(m, "CpuUsage", DOC(dai, CpuUsage));
    py::enum_<CameraModel> cameraModel(m, "CameraModel", DOC(dai, CameraModel));
    py::class_<StereoRectification> stereoRectification(m, "StereoRectification", DOC(dai, StereoRectification));
    py::class_<Extrinsics> extrinsics(m, "Extrinsics", DOC(dai, Extrinsics));
    py::class_<CameraInfo> cameraInfo(m, "CameraInfo", DOC(dai, CameraInfo));
    py::class_<EepromData> eepromData(m, "EepromData", DOC(dai, EepromData));
    py::enum_<UsbSpeed> usbSpeed(m, "UsbSpeed", DOC(dai, UsbSpeed));
    py::enum_<ProcessorType> processorType(m, "ProcessorType");


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


    timestamp
        .def(py::init<>())
        .def_readwrite("sec", &Timestamp::sec)
        .def_readwrite("nsec", &Timestamp::nsec)
        .def("get", &Timestamp::get)
        ;

    point2f
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &Point2f::x)
        .def_readwrite("y", &Point2f::y)
        ;

    point3f
        .def(py::init<>())
        .def(py::init<float, float, float>())
        .def_readwrite("x", &Point3f::x)
        .def_readwrite("y", &Point3f::y)
        .def_readwrite("z", &Point3f::z)
        ;

    size2f
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("width", &Size2f::width)
        .def_readwrite("height", &Size2f::height)
        ;

    // CameraBoardSocket enum bindings
    cameraBoardSocket
        .value("AUTO", CameraBoardSocket::AUTO)
        .value("RGB", CameraBoardSocket::RGB)
        .value("LEFT", CameraBoardSocket::LEFT)
        .value("RIGHT", CameraBoardSocket::RIGHT)
    ;

    // CameraImageOrientation enum bindings
    cameraImageOrientation
        .value("AUTO", CameraImageOrientation::AUTO)
        .value("NORMAL", CameraImageOrientation::NORMAL)
        .value("HORIZONTAL_MIRROR", CameraImageOrientation::HORIZONTAL_MIRROR)
        .value("VERTICAL_FLIP", CameraImageOrientation::VERTICAL_FLIP)
        .value("ROTATE_180_DEG", CameraImageOrientation::ROTATE_180_DEG)
    ;

    // MemoryInfo
    memoryInfo
        .def(py::init<>())
        .def_readwrite("remaining", &MemoryInfo::remaining)
        .def_readwrite("used", &MemoryInfo::used)
        .def_readwrite("total", &MemoryInfo::total)
    ;

    // ChipTemperature
    chipTemperature
        .def(py::init<>())
        .def_readwrite("css", &ChipTemperature::css)
        .def_readwrite("mss", &ChipTemperature::mss)
        .def_readwrite("upa", &ChipTemperature::upa)
        .def_readwrite("dss", &ChipTemperature::dss)
        .def_readwrite("average", &ChipTemperature::average)
    ;

    // CpuUsage
    cpuUsage
        .def(py::init<>())
        .def_readwrite("average", &CpuUsage::average)
        .def_readwrite("msTime", &CpuUsage::msTime)
    ;
    // CameraModel enum bindings
    cameraModel
        .value("Perspective", CameraModel::Perspective)
        .value("Fisheye", CameraModel::Fisheye)
        .value("Equirectangular", CameraModel::Equirectangular)
        .value("RadialDivision", CameraModel::RadialDivision)
    ;

    // StereoRectification
    stereoRectification
        .def(py::init<>())
        .def_readwrite("rectifiedRotationLeft", &StereoRectification::rectifiedRotationLeft)
        .def_readwrite("rectifiedRotationRight", &StereoRectification::rectifiedRotationRight)
        .def_readwrite("leftCameraSocket", &StereoRectification::leftCameraSocket)
        .def_readwrite("rightCameraSocket", &StereoRectification::rightCameraSocket)
        ;

    // Extrinsics
    extrinsics
        .def(py::init<>())
        .def_readwrite("rotationMatrix", &Extrinsics::rotationMatrix)
        .def_readwrite("translation", &Extrinsics::translation)
        .def_readwrite("specTranslation", &Extrinsics::specTranslation)
        .def_readwrite("toCameraSocket", &Extrinsics::toCameraSocket)
        ;

    // CameraInfo
    cameraInfo
        .def(py::init<>())
        .def_readwrite("width", &CameraInfo::width)
        .def_readwrite("height", &CameraInfo::height)
        .def_readwrite("intrinsicMatrix", &CameraInfo::intrinsicMatrix)
        .def_readwrite("distortionCoeff", &CameraInfo::distortionCoeff)
        .def_readwrite("extrinsics", &CameraInfo::extrinsics)
        .def_readwrite("cameraType", &CameraInfo::cameraType)
        .def_readwrite("specHfovDeg", &CameraInfo::specHfovDeg)
        ;

    // EepromData
    eepromData
        .def(py::init<>())
        .def_readwrite("version", &EepromData::version)
        .def_readwrite("boardName", &EepromData::boardName)
        .def_readwrite("boardRev", &EepromData::boardRev)
        .def_readwrite("cameraData", &EepromData::cameraData)
        .def_readwrite("stereoRectificationData", &EepromData::stereoRectificationData)
        .def_readwrite("imuExtrinsics", &EepromData::imuExtrinsics)
        ;
    // UsbSpeed
    usbSpeed
        .value("UNKNOWN", UsbSpeed::UNKNOWN)
        .value("LOW", UsbSpeed::LOW)
        .value("FULL", UsbSpeed::FULL)
        .value("HIGH", UsbSpeed::HIGH)
        .value("SUPER", UsbSpeed::SUPER)
        .value("SUPER_PLUS", UsbSpeed::SUPER_PLUS)
    ;

    // ProcessorType
    processorType
        .value("LEON_CSS", ProcessorType::LEON_CSS)
        .value("LEON_MSS", ProcessorType::LEON_MSS)
    ;

}
