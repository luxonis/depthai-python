#pragma once

#include "stream/stream_info.hpp"
#include "general/data_subject.hpp"
#include "stream/stream_data.hpp"

class HostCaptureCommand
    : public DataSubject<StreamInfo, StreamData>
{
public:
    HostCaptureCommand(const StreamInfo& streamToSendCommand);
    void capture();

private:
    StreamInfo stream;

    uint32_t command;


};
