#include "DatatypeBindings.hpp"

#include "pipeline/CommonBindings.hpp"
#include "depthai-shared/datatype/DatatypeEnum.hpp"

void bind_adatatype(pybind11::module& m, void* pCallstack);
void bind_apriltagconfig(pybind11::module& m, void* pCallstack);
void bind_apriltags(pybind11::module& m, void* pCallstack);
void bind_buffer(pybind11::module& m, void* pCallstack);
void bind_cameracontrol(pybind11::module& m, void* pCallstack);
void bind_edgedetectorconfig(pybind11::module& m, void* pCallstack);
void bind_featuretrackerconfig(pybind11::module& m, void* pCallstack);
void bind_tofconfig(pybind11::module& m, void* pCallstack);
void bind_imagemanipconfig(pybind11::module& m, void* pCallstack);
void bind_imgdetections(pybind11::module& m, void* pCallstack);
void bind_imgframe(pybind11::module& m, void* pCallstack);
void bind_imudata(pybind11::module& m, void* pCallstack);
void bind_nndata(pybind11::module& m, void* pCallstack);
void bind_spatialimgdetections(pybind11::module& m, void* pCallstack);
void bind_spatiallocationcalculatorconfig(pybind11::module& m, void* pCallstack);
void bind_spatiallocationcalculatordata(pybind11::module& m, void* pCallstack);
void bind_stereodepthconfig(pybind11::module& m, void* pCallstack);
void bind_systeminformation(pybind11::module& m, void* pCallstack);
void bind_systeminformationS3(pybind11::module& m, void* pCallstack);
void bind_trackedfeatures(pybind11::module& m, void* pCallstack);
void bind_tracklets(pybind11::module& m, void* pCallstack);
void bind_benchmarkreport(pybind11::module& m, void* pCallstack);
void bind_pointcloudconfig(pybind11::module& m, void* pCallstack);
void bind_occupancypool(pybind11::module& m, void* pCallstack);
void bind_visionhealthmetrics(pybind11::module& m, void* pCallstack);
void bind_visionhealthconfig(pybind11::module& m, void* pCallstack);

void DatatypeBindings::addToCallstack(std::deque<StackFunction>& callstack) {
     // Bind common datatypebindings
    callstack.push_front(DatatypeBindings::bind);

    // Bind all datatypes (order matters)
    callstack.push_front(bind_adatatype);
    callstack.push_front(bind_buffer);
    callstack.push_front(bind_apriltagconfig);
    callstack.push_front(bind_apriltags);
    callstack.push_front(bind_cameracontrol);
    callstack.push_front(bind_edgedetectorconfig);
    callstack.push_front(bind_featuretrackerconfig);
    callstack.push_front(bind_tofconfig);
    callstack.push_front(bind_imagemanipconfig);
    callstack.push_front(bind_imgdetections);
    callstack.push_front(bind_imgframe);
    callstack.push_front(bind_imudata);
    callstack.push_front(bind_nndata);
    callstack.push_front(bind_spatialimgdetections);
    callstack.push_front(bind_spatiallocationcalculatorconfig);
    callstack.push_front(bind_spatiallocationcalculatordata);
    callstack.push_front(bind_stereodepthconfig);
    callstack.push_front(bind_systeminformation);
    callstack.push_front(bind_systeminformationS3);
    callstack.push_front(bind_trackedfeatures);
    callstack.push_front(bind_tracklets);
    callstack.push_front(bind_benchmarkreport);
    callstack.push_front(bind_pointcloudconfig);
    callstack.push_front(bind_occupancypool);
    callstack.push_front(bind_visionhealthmetrics);
    callstack.push_front(bind_visionhealthconfig);
}

void DatatypeBindings::bind(pybind11::module& m, void* pCallstack){
    using namespace dai;

    py::enum_<DatatypeEnum> datatypeEnum(m, "DatatypeEnum", DOC(dai, DatatypeEnum));

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

    m.def("isDatatypeSubclassOf", &isDatatypeSubclassOf);

    datatypeEnum
        .value("Buffer", DatatypeEnum::Buffer)
        .value("ImgFrame", DatatypeEnum::ImgFrame)
        .value("NNData", DatatypeEnum::NNData)
        .value("ImageManipConfig", DatatypeEnum::ImageManipConfig)
        .value("CameraControl", DatatypeEnum::CameraControl)
        .value("ImgDetections", DatatypeEnum::ImgDetections)
        .value("SpatialImgDetections", DatatypeEnum::SpatialImgDetections)
        .value("SystemInformation", DatatypeEnum::SystemInformation)
        .value("SpatialLocationCalculatorConfig", DatatypeEnum::SpatialLocationCalculatorConfig)
        .value("SpatialLocationCalculatorData", DatatypeEnum::SpatialLocationCalculatorData)
        .value("EdgeDetectorConfig", DatatypeEnum::EdgeDetectorConfig)
        .value("AprilTagConfig", DatatypeEnum::AprilTagConfig)
        .value("AprilTags", DatatypeEnum::AprilTags)
        .value("Tracklets", DatatypeEnum::Tracklets)
        .value("IMUData", DatatypeEnum::IMUData)
        .value("StereoDepthConfig", DatatypeEnum::StereoDepthConfig)
        .value("FeatureTrackerConfig", DatatypeEnum::FeatureTrackerConfig)
        .value("TrackedFeatures", DatatypeEnum::TrackedFeatures)
        .value("OccupancyPool", DatatypeEnum::OccupancyPool)
    ;

}
