#include "DatatypeBindings.hpp"

#include "pipeline/CommonBindings.hpp"

void bind_adatatype(pybind11::module& m, void* pCallstack);
void bind_apriltagconfig(pybind11::module& m, void* pCallstack);
void bind_apriltags(pybind11::module& m, void* pCallstack);
void bind_buffer(pybind11::module& m, void* pCallstack);
void bind_cameracontrol(pybind11::module& m, void* pCallstack);
void bind_edgedetectorconfig(pybind11::module& m, void* pCallstack);
void bind_featuretrackerconfig(pybind11::module& m, void* pCallstack);
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
void bind_trackedfeatures(pybind11::module& m, void* pCallstack);
void bind_tracklets(pybind11::module& m, void* pCallstack);

void DatatypeBindings::addToCallstack(std::deque<StackFunction>& callstack) {
    // Bind all datatypes (order matters)
    callstack.push_front(bind_adatatype);
    callstack.push_front(bind_buffer);
    callstack.push_front(bind_apriltagconfig);
    callstack.push_front(bind_apriltags);
    callstack.push_front(bind_cameracontrol);
    callstack.push_front(bind_edgedetectorconfig);
    callstack.push_front(bind_featuretrackerconfig);
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
    callstack.push_front(bind_trackedfeatures);
    callstack.push_front(bind_tracklets);
}
