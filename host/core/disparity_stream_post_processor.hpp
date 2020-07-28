#pragma once


#include <vector>

#include "stream/stream_info.hpp"
#include "general/data_observer.hpp"
#include "general/data_subject.hpp"
#include "stream/stream_data.hpp"


class DisparityStreamPostProcessor
    : public DataSubject<StreamInfo, StreamData>
    , public DataObserver<StreamInfo, StreamData>
{
public:
    DisparityStreamPostProcessor(bool produce_d_color);

protected:
    // class DataObserver
    virtual void onNewData(const StreamInfo &data_info, const StreamData &data);


private:
    const std::string c_stream_in        = "disparity";
    const std::string c_stream_out_color = "disparity_color";

    const bool _produce_depth_color = false;

    void prepareDepthColorAndNotifyObservers(const StreamInfo &data_info, const StreamData &data);
};
