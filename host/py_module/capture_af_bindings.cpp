#include "capture_af_bindings.hpp"

//project
#include "metadata/capture_metadata.hpp"
#include "host_capture_command.hpp"

// Binding for CaptureMetadata POD
namespace py = pybind11;

extern std::unique_ptr<HostCaptureCommand> g_host_caputure_command;


static void request_jpeg(){
    if(g_host_caputure_command != nullptr){
        g_host_caputure_command->capture();
    }
}

static void request_af_trigger(){
    if(g_host_caputure_command != nullptr){
        g_host_caputure_command->afTrigger();
    }
}

static void request_af_mode(CaptureMetadata::AutofocusMode mode){
    if(g_host_caputure_command != nullptr){
        g_host_caputure_command->afMode(mode);
    }
}


void init_binding_capture_af(pybind11::module& m){

    //py::class_<CaptureMetadata> captureMetadata(m, "CaptureMetadata");

    py::enum_<CaptureMetadata::AutofocusMode>(m, "AutofocusMode")
        .value("AF_MODE_AUTO", CaptureMetadata::AutofocusMode::AF_MODE_AUTO)
        .value("AF_MODE_MACRO", CaptureMetadata::AutofocusMode::AF_MODE_MACRO)
        .value("AF_MODE_CONTINUOUS_VIDEO", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_VIDEO)
        .value("AF_MODE_CONTINUOUS_PICTURE", CaptureMetadata::AutofocusMode::AF_MODE_CONTINUOUS_PICTURE)
        .value("AF_MODE_EDOF", CaptureMetadata::AutofocusMode::AF_MODE_EDOF)
    ;



    // depthai.request_jpeg()
    m.def(
        "request_jpeg",
        &request_jpeg,
        "Function to request a still JPEG encoded image ('jpeg' stream must be enabled)"
    );

    // depthai.request_af_trigger()
    m.def(
        "request_af_trigger",
        &request_af_trigger,
        "Function to request autofocus trigger"
    );
    
    // depthai.request_af_mode(One of: depthai.AutofocusMode.__members__)
    m.def(
        "request_af_mode",
        &request_af_mode,
        "Function to request a certain autofocus mode (Check AutofocusMode.__members__)"
    );



}