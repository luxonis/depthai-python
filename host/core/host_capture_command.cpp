#include "host_capture_command.hpp"

HostCaptureCommand::HostCaptureCommand(const StreamInfo& streamToSendCommand) : stream(streamToSendCommand){

}

void HostCaptureCommand::capture()
{
    command = 1;

    StreamData captureCommand;
    captureCommand.packet_number = 0;
    captureCommand.data = &command;
    captureCommand.size = 4;

    notifyObservers(stream, captureCommand);
}

