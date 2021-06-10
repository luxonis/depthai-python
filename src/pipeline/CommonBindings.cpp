#include "CommonBindings.hpp"

// depthai-shared
#include "depthai-shared/common/CameraBoardSocket.hpp"
#include "depthai-shared/common/EepromData.hpp"
#include "depthai-shared/common/CameraImageOrientation.hpp"
#include "depthai-shared/common/MemoryInfo.hpp"
#include "depthai-shared/common/ChipTemperature.hpp"
#include "depthai-shared/common/CpuUsage.hpp"
#include "depthai-shared/common/Timestamp.hpp"
#include "depthai-shared/common/Point2f.hpp"
#include "depthai-shared/common/Point3f.hpp"
#include "depthai-shared/common/Size2f.hpp"
#include "depthai-shared/common/UsbSpeed.hpp"

void CommonBindings::bind(pybind11::module& m){

    using namespace dai;

    py::class_<Timestamp>(m, "Timestamp", DOC(dai, Timestamp))
        .def(py::init<>())
        .def_readwrite("sec", &Timestamp::sec)
        .def_readwrite("nsec", &Timestamp::nsec)
        .def("get", &Timestamp::get)
        ;

    py::class_<Point2f>(m, "Point2f", DOC(dai, Point2f))
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &Point2f::x)
        .def_readwrite("y", &Point2f::y)
        ;

    py::class_<Point3f>(m, "Point3f", DOC(dai, Point3f))
        .def(py::init<>())
        .def(py::init<float, float, float>())
        .def_readwrite("x", &Point3f::x)
        .def_readwrite("y", &Point3f::y)
        .def_readwrite("z", &Point3f::z)
        ;

    py::class_<Size2f>(m, "Size2f", DOC(dai, Size2f))
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("width", &Size2f::width)
        .def_readwrite("height", &Size2f::height)
        ;

    // CameraBoardSocket enum bindings
    py::enum_<CameraBoardSocket>(m, "CameraBoardSocket", DOC(dai, CameraBoardSocket))
        .value("AUTO", CameraBoardSocket::AUTO)
        .value("RGB", CameraBoardSocket::RGB)
        .value("LEFT", CameraBoardSocket::LEFT)
        .value("RIGHT", CameraBoardSocket::RIGHT)
    ;

    // CameraImageOrientation enum bindings
    py::enum_<CameraImageOrientation>(m, "CameraImageOrientation", DOC(dai, CameraImageOrientation))
        .value("AUTO", CameraImageOrientation::AUTO)
        .value("NORMAL", CameraImageOrientation::NORMAL)
        .value("HORIZONTAL_MIRROR", CameraImageOrientation::HORIZONTAL_MIRROR)
        .value("VERTICAL_FLIP", CameraImageOrientation::VERTICAL_FLIP)
        .value("ROTATE_180_DEG", CameraImageOrientation::ROTATE_180_DEG)
    ;

    // MemoryInfo
    py::class_<MemoryInfo>(m, "MemoryInfo", DOC(dai, MemoryInfo))
        .def(py::init<>())
        .def_readwrite("remaining", &MemoryInfo::remaining)
        .def_readwrite("used", &MemoryInfo::used)
        .def_readwrite("total", &MemoryInfo::total)
    ;

    // ChipTemperature
    py::class_<ChipTemperature>(m, "ChipTemperature", DOC(dai, ChipTemperature))
        .def(py::init<>())
        .def_readwrite("css", &ChipTemperature::css)
        .def_readwrite("mss", &ChipTemperature::mss)
        .def_readwrite("upa", &ChipTemperature::upa)
        .def_readwrite("dss", &ChipTemperature::dss)
        .def_readwrite("average", &ChipTemperature::average)
    ;

    // CpuUsage
    py::class_<CpuUsage>(m, "CpuUsage", DOC(dai, CpuUsage))
        .def(py::init<>())
        .def_readwrite("average", &CpuUsage::average)
        .def_readwrite("msTime", &CpuUsage::msTime)
    ;
    // CameraModel enum bindings
    py::enum_<CameraModel>(m, "CameraModel", DOC(dai, CameraModel))
        .value("Perspective", CameraModel::Perspective)
        .value("Fisheye", CameraModel::Fisheye)
        .value("Equirectangular", CameraModel::Equirectangular)
        .value("RadialDivision", CameraModel::RadialDivision)
    ;

    // StereoRectification
    py::class_<StereoRectification> (m, "StereoRectification", DOC(dai, StereoRectification))
        .def(py::init<>())
        .def_readwrite("rectifiedRotationLeft", &StereoRectification::rectifiedRotationLeft)
        .def_readwrite("rectifiedRotationRight", &StereoRectification::rectifiedRotationRight)
        .def_readwrite("leftCameraSocket", &StereoRectification::leftCameraSocket)
        .def_readwrite("rightCameraSocket", &StereoRectification::rightCameraSocket)
        ;

    // Extrinsics
    py::class_<Extrinsics> (m, "Extrinsics", DOC(dai, Extrinsics))
        .def(py::init<>())
        .def_readwrite("rotationMatrix", &Extrinsics::rotationMatrix)
        .def_readwrite("translation", &Extrinsics::translation)
        .def_readwrite("specTranslation", &Extrinsics::specTranslation)
        .def_readwrite("toCameraSocket", &Extrinsics::toCameraSocket)
        ;

    // CameraInfo
    py::class_<CameraInfo> (m, "CameraInfo", DOC(dai, CameraInfo))
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
    py::class_<EepromData> (m, "EepromData", DOC(dai, EepromData))
        .def(py::init<>())
        .def_readwrite("version", &EepromData::version)
        .def_readwrite("boardName", &EepromData::boardName)
        .def_readwrite("boardRev", &EepromData::boardRev)
        .def_readwrite("cameraData", &EepromData::cameraData)
        .def_readwrite("stereoRectificationData", &EepromData::stereoRectificationData)
        .def_readwrite("imuExtrinsics", &EepromData::imuExtrinsics)
        ;
    // UsbSpeed
    py::enum_<UsbSpeed>(m, "UsbSpeed", DOC(dai, UsbSpeed))
        .value("UNKNOWN", UsbSpeed::UNKNOWN)
        .value("LOW", UsbSpeed::LOW)
        .value("FULL", UsbSpeed::FULL)
        .value("HIGH", UsbSpeed::HIGH)
        .value("SUPER", UsbSpeed::SUPER)
        .value("SUPER_PLUS", UsbSpeed::SUPER_PLUS)
    ;

}
