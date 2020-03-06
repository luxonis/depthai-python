#pragma once

#include "stream/stream_info.hpp"
#include "general/data_observer.hpp"
#include "stream/stream_data.hpp"


class DeviceSupportListener
    : public DataObserver<StreamInfo, StreamData>
{
public:

protected:
    // class DataObserver
    virtual void onNewData(const StreamInfo &data_info, const StreamData &data) final;

};
