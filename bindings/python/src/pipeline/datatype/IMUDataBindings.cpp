#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/IMUData.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_imudata(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<IMUReport, std::shared_ptr<IMUReport>> imuReport(m, "IMUReport", DOC(dai, IMUReport));
    py::enum_<IMUReport::Accuracy> imuReportAccuracy(imuReport, "Accuracy");
    py::class_<IMUReportAccelerometer, IMUReport, std::shared_ptr<IMUReportAccelerometer>> imuReportAccelerometer(m, "IMUReportAccelerometer", DOC(dai, IMUReportAccelerometer));
    py::class_<IMUReportGyroscope, IMUReport, std::shared_ptr<IMUReportGyroscope>> imuReportGyroscope(m, "IMUReportGyroscope", DOC(dai, IMUReportGyroscope));
    py::class_<IMUReportMagneticField, IMUReport, std::shared_ptr<IMUReportMagneticField>> imuReportMagneticField(m, "IMUReportMagneticField", DOC(dai, IMUReportMagneticField));
    py::class_<IMUReportRotationVectorWAcc, IMUReport, std::shared_ptr<IMUReportRotationVectorWAcc>> imuReportRotationVectorWAcc(m, "IMUReportRotationVectorWAcc", DOC(dai, IMUReportRotationVectorWAcc));
    py::class_<IMUPacket> imuPacket(m, "IMUPacket", DOC(dai, IMUPacket));
    // py::class_<RawIMUData, RawBuffer, std::shared_ptr<RawIMUData>> rawIMUPackets(m, "RawIMUData", DOC(dai, RawIMUData));
    py::class_<IMUData, Buffer, std::shared_ptr<IMUData>> imuData(m, "IMUData", DOC(dai, IMUData));

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
    imuReport
        .def(py::init<>())
        .def_readwrite("sequence", &IMUReport::sequence)
        .def_readwrite("accuracy", &IMUReport::accuracy)
        .def_readwrite("timestamp", &IMUReport::timestamp)
        .def_readwrite("tsDevice", &IMUReport::tsDevice)
        .def("getTimestamp", &IMUReport::getTimestamp, DOC(dai, IMUReport, getTimestamp))
        .def("getTimestampDevice", &IMUReport::getTimestampDevice, DOC(dai, IMUReport, getTimestampDevice))
        .def("getSequenceNum", &IMUReport::getSequenceNum, DOC(dai, IMUReport, getSequenceNum))
        ;

    imuReportAccuracy
        .value("UNRELIABLE", IMUReport::Accuracy::UNRELIABLE)
        .value("LOW", IMUReport::Accuracy::LOW)
        .value("MEDIUM", IMUReport::Accuracy::MEDIUM)
        .value("HIGH", IMUReport::Accuracy::HIGH)
        ;

    imuReportAccelerometer
        .def(py::init<>())
        .def_readwrite("x", &IMUReportAccelerometer::x)
        .def_readwrite("y", &IMUReportAccelerometer::y)
        .def_readwrite("z", &IMUReportAccelerometer::z)
        ;

    imuReportGyroscope
        .def(py::init<>())
        .def_readwrite("x", &IMUReportGyroscope::x)
        .def_readwrite("y", &IMUReportGyroscope::y)
        .def_readwrite("z", &IMUReportGyroscope::z)
        ;

    imuReportMagneticField
        .def(py::init<>())
        .def_readwrite("x", &IMUReportMagneticField::x)
        .def_readwrite("y", &IMUReportMagneticField::y)
        .def_readwrite("z", &IMUReportMagneticField::z)
        ;

    imuReportRotationVectorWAcc
        .def(py::init<>())
        .def_readwrite("i", &IMUReportRotationVectorWAcc::i)
        .def_readwrite("j", &IMUReportRotationVectorWAcc::j)
        .def_readwrite("k", &IMUReportRotationVectorWAcc::k)
        .def_readwrite("real", &IMUReportRotationVectorWAcc::real)
        .def_readwrite("rotationVectorAccuracy", &IMUReportRotationVectorWAcc::rotationVectorAccuracy)
        ;

#if 0
    py::class_<IMUReportRotationVector, IMUReport, std::shared_ptr<IMUReportRotationVector>>(m, "IMUReportRotationVector", DOC(dai, IMUReportRotationVector))
        .def(py::init<>())
        .def_readwrite("i", &IMUReportRotationVector::i)
        .def_readwrite("j", &IMUReportRotationVector::j)
        .def_readwrite("k", &IMUReportRotationVector::k)
        .def_readwrite("real", &IMUReportRotationVector::real)
        ;

    py::class_<IMUReportGyroscopeUncalibrated, IMUReport, std::shared_ptr<IMUReportGyroscopeUncalibrated>>(m, "IMUReportGyroscopeUncalibrated", DOC(dai, IMUReportGyroscopeUncalibrated))
        .def(py::init<>())
        .def_readwrite("x", &IMUReportGyroscopeUncalibrated::x)
        .def_readwrite("y", &IMUReportGyroscopeUncalibrated::y)
        .def_readwrite("z", &IMUReportGyroscopeUncalibrated::z)
        .def_readwrite("biasX", &IMUReportGyroscopeUncalibrated::biasX)
        .def_readwrite("biasY", &IMUReportGyroscopeUncalibrated::biasY)
        .def_readwrite("biasZ", &IMUReportGyroscopeUncalibrated::biasZ)
        ;

    py::class_<IMUReportMagneticFieldUncalibrated, IMUReport, std::shared_ptr<IMUReportMagneticFieldUncalibrated>>(m, "IMUReportMagneticFieldUncalibrated", DOC(dai, IMUReportMagneticFieldUncalibrated))
        .def(py::init<>())
        .def_readwrite("x", &IMUReportMagneticFieldUncalibrated::x)
        .def_readwrite("y", &IMUReportMagneticFieldUncalibrated::y)
        .def_readwrite("z", &IMUReportMagneticFieldUncalibrated::z)
        .def_readwrite("biasX", &IMUReportMagneticFieldUncalibrated::biasX)
        .def_readwrite("biasY", &IMUReportMagneticFieldUncalibrated::biasY)
        .def_readwrite("biasZ", &IMUReportMagneticFieldUncalibrated::biasZ)
        ;

    py::class_<IMUReportGyroIntegratedRV, IMUReport, std::shared_ptr<IMUReportGyroIntegratedRV>>(m, "IMUReportGyroIntegratedRV", DOC(dai, IMUReportGyroIntegratedRV))
        .def(py::init<>())
        .def_readwrite("i", &IMUReportGyroIntegratedRV::i)
        .def_readwrite("j", &IMUReportGyroIntegratedRV::j)
        .def_readwrite("k", &IMUReportGyroIntegratedRV::k)
        .def_readwrite("real", &IMUReportGyroIntegratedRV::real)
        .def_readwrite("angVelX", &IMUReportGyroIntegratedRV::angVelX)
        .def_readwrite("angVelY", &IMUReportGyroIntegratedRV::angVelY)
        .def_readwrite("angVelZ", &IMUReportGyroIntegratedRV::angVelZ)
        ;

#endif

    imuPacket
        .def(py::init<>())
        .def_readwrite("acceleroMeter", &IMUPacket::acceleroMeter)
        .def_readwrite("gyroscope", &IMUPacket::gyroscope)
        .def_readwrite("magneticField", &IMUPacket::magneticField)
        .def_readwrite("rotationVector", &IMUPacket::rotationVector)
#if 0
        .def_readwrite("rawAcceleroMeter", &IMUPacket::rawAcceleroMeter)
        .def_readwrite("linearAcceleroMeter", &IMUPacket::linearAcceleroMeter)
        .def_readwrite("gravity", &IMUPacket::gravity)
        .def_readwrite("rawGyroscope", &IMUPacket::rawGyroscope)
        .def_readwrite("gyroscopeUncalibrated", &IMUPacket::gyroscopeUncalibrated)
        .def_readwrite("rawMagneticField", &IMUPacket::rawMagneticField)
        .def_readwrite("magneticFieldUncalibrated", &IMUPacket::magneticFieldUncalibrated)
        .def_readwrite("gameRotationVector", &IMUPacket::gameRotationVector)
        .def_readwrite("geoMagRotationVector", &IMUPacket::geoMagRotationVector)
        .def_readwrite("arvrStabilizedRotationVector", &IMUPacket::arvrStabilizedRotationVector)
        .def_readwrite("arvrStabilizedGameRotationVector", &IMUPacket::arvrStabilizedGameRotationVector)
        .def_readwrite("gyroIntegratedRotationVector", &IMUPacket::gyroIntegratedRotationVector)
#endif
        ;

    // rawIMUPackets
    //     .def(py::init<>())
    //     .def_readwrite("packets", &RawIMUData::packets)
    //     ;

    // Message
    imuData
        .def(py::init<>())
        .def_property("packets", [](IMUData& imuDta) { return &imuDta.packets; }, [](IMUData& imuDta, std::vector<IMUPacket> val) { imuDta.packets = val; }, DOC(dai, IMUData, packets))
        ;

}
