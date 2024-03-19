#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/IMU.hpp"

void bind_imu(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::enum_<IMUSensor> imuSensor(m, "IMUSensor", DOC(dai, IMUSensor));
    py::class_<IMUSensorConfig, std::shared_ptr<IMUSensorConfig>> imuSensorConfig(m, "IMUSensorConfig", DOC(dai, IMUSensorConfig));
    py::class_<IMUProperties> imuProperties(m, "IMUProperties", DOC(dai, IMUProperties));
    auto imu = ADD_NODE(IMU);

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
    imuSensor
        .value("ACCELEROMETER_RAW", IMUSensor::ACCELEROMETER_RAW, DOC(dai, IMUSensor, ACCELEROMETER_RAW))
        .value("ACCELEROMETER", IMUSensor::ACCELEROMETER, DOC(dai, IMUSensor, ACCELEROMETER))
        .value("LINEAR_ACCELERATION", IMUSensor::LINEAR_ACCELERATION, DOC(dai, IMUSensor, LINEAR_ACCELERATION))
        .value("GRAVITY", IMUSensor::GRAVITY, DOC(dai, IMUSensor, GRAVITY))
        .value("GYROSCOPE_RAW", IMUSensor::GYROSCOPE_RAW, DOC(dai, IMUSensor, GYROSCOPE_RAW))
        .value("GYROSCOPE_CALIBRATED", IMUSensor::GYROSCOPE_CALIBRATED, DOC(dai, IMUSensor, GYROSCOPE_CALIBRATED))
        .value("GYROSCOPE_UNCALIBRATED", IMUSensor::GYROSCOPE_UNCALIBRATED, DOC(dai, IMUSensor, GYROSCOPE_UNCALIBRATED))
        .value("MAGNETOMETER_RAW", IMUSensor::MAGNETOMETER_RAW, DOC(dai, IMUSensor, MAGNETOMETER_RAW))
        .value("MAGNETOMETER_CALIBRATED", IMUSensor::MAGNETOMETER_CALIBRATED, DOC(dai, IMUSensor, MAGNETOMETER_CALIBRATED))
        .value("MAGNETOMETER_UNCALIBRATED", IMUSensor::MAGNETOMETER_UNCALIBRATED, DOC(dai, IMUSensor, MAGNETOMETER_UNCALIBRATED))
        .value("ROTATION_VECTOR", IMUSensor::ROTATION_VECTOR, DOC(dai, IMUSensor, ROTATION_VECTOR))
        .value("GAME_ROTATION_VECTOR", IMUSensor::GAME_ROTATION_VECTOR, DOC(dai, IMUSensor, GAME_ROTATION_VECTOR))
        .value("GEOMAGNETIC_ROTATION_VECTOR", IMUSensor::GEOMAGNETIC_ROTATION_VECTOR, DOC(dai, IMUSensor, GEOMAGNETIC_ROTATION_VECTOR))
        .value("ARVR_STABILIZED_ROTATION_VECTOR", IMUSensor::ARVR_STABILIZED_ROTATION_VECTOR, DOC(dai, IMUSensor, ARVR_STABILIZED_ROTATION_VECTOR))
        .value("ARVR_STABILIZED_GAME_ROTATION_VECTOR", IMUSensor::ARVR_STABILIZED_GAME_ROTATION_VECTOR, DOC(dai, IMUSensor, ARVR_STABILIZED_GAME_ROTATION_VECTOR))
        // .value("GYRO_INTEGRATED_ROTATION_VECTOR", IMUSensor::GYRO_INTEGRATED_ROTATION_VECTOR)
    ;

    imuSensorConfig
        .def(py::init<>())
        .def_readwrite("sensitivityEnabled", &IMUSensorConfig::sensitivityEnabled)
        .def_readwrite("sensitivityRelative", &IMUSensorConfig::sensitivityRelative)
        .def_readwrite("changeSensitivity", &IMUSensorConfig::changeSensitivity)
        .def_readwrite("reportRate", &IMUSensorConfig::reportRate)
        .def_readwrite("sensorId", &IMUSensorConfig::sensorId)
        ;

    imuProperties
        .def_readwrite("imuSensors", &IMUProperties::imuSensors, DOC(dai, IMUProperties, imuSensors))
        .def_readwrite("batchReportThreshold", &IMUProperties::batchReportThreshold, DOC(dai, IMUProperties, batchReportThreshold))
        .def_readwrite("maxBatchReports", &IMUProperties::maxBatchReports, DOC(dai, IMUProperties, maxBatchReports))
        .def_readwrite("enableFirmwareUpdate", &IMUProperties::enableFirmwareUpdate, DOC(dai, IMUProperties, enableFirmwareUpdate))
    ;

    // Node
    imu
        .def_readonly("out", &IMU::out, DOC(dai, node, IMU, out))
        .def("enableIMUSensor", static_cast<void(IMU::*)(IMUSensorConfig imuSensor)>(&IMU::enableIMUSensor), py::arg("sensorConfig"), DOC(dai, node, IMU, enableIMUSensor))
        .def("enableIMUSensor", static_cast<void(IMU::*)(const std::vector<IMUSensorConfig>& imuSensors)>(&IMU::enableIMUSensor), py::arg("sensorConfigs"), DOC(dai, node, IMU, enableIMUSensor, 2))
        .def("enableIMUSensor", static_cast<void(IMU::*)(IMUSensor sensor, uint32_t reportRate)>(&IMU::enableIMUSensor), py::arg("sensor"), py::arg("reportRate"), DOC(dai, node, IMU, enableIMUSensor, 3))
        .def("enableIMUSensor", static_cast<void(IMU::*)(const std::vector<IMUSensor>& sensors, uint32_t reportRate)>(&IMU::enableIMUSensor), py::arg("sensors"), py::arg("reportRate"), DOC(dai, node, IMU, enableIMUSensor, 4))
        .def("setBatchReportThreshold", &IMU::setBatchReportThreshold, py::arg("batchReportThreshold"), DOC(dai, node, IMU, setBatchReportThreshold))
        .def("getBatchReportThreshold", &IMU::getBatchReportThreshold, DOC(dai, node, IMU, getBatchReportThreshold))
        .def("setMaxBatchReports", &IMU::setMaxBatchReports, py::arg("maxBatchReports"), DOC(dai, node, IMU, setMaxBatchReports))
        .def("getMaxBatchReports", &IMU::getMaxBatchReports, DOC(dai, node, IMU, getMaxBatchReports))
        .def("enableFirmwareUpdate", &IMU::enableFirmwareUpdate, DOC(dai, node, IMU, enableFirmwareUpdate))
        ;
    daiNodeModule.attr("IMU").attr("Properties") = imuProperties;

}
