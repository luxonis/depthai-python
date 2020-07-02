#include "host_capture_command.hpp"

HostCaptureCommand::HostCaptureCommand(const StreamInfo& streamToSendCommand) : stream(streamToSendCommand){ /* empty body */ }

void HostCaptureCommand::sendCaptureMetadata(CaptureMetadata meta){
    StreamData captureMetadata;
    captureMetadata.packet_number = 0;
    captureMetadata.data = &meta;
    captureMetadata.size = sizeof(CaptureMetadata);
    notifyObservers(stream, captureMetadata);
}

void HostCaptureCommand::capture(){
    sendCaptureMetadata(CaptureMetadata::createStillCapture());
}


void HostCaptureCommand::afMode(CaptureMetadata::AutofocusMode mode){
    sendCaptureMetadata(CaptureMetadata::createAfMode(mode));
}

void HostCaptureCommand::afTrigger(){
    sendCaptureMetadata(CaptureMetadata::createAfTrigger());
}
