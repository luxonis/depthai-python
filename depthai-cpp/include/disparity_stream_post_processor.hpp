#pragma once
// This file is created as temporary solution for calculation
// of distance data from disparity

// Std
#include <vector>

// Shared
#include "depthai-shared/general/data_observer.hpp"
#include "depthai-shared/general/data_subject.hpp"
#include "depthai-shared/stream/stream_info.hpp"
#include "depthai-shared/stream/stream_data.hpp"


class DisparityStreamPostProcessor
    : public DataSubject<StreamInfo, StreamData>
    , public DataObserver<StreamInfo, StreamData>
{
public:
    DisparityStreamPostProcessor(bool produce_d_color, bool produce_d_mm);

    std::vector<uint16_t> getLutDisparityToMillimeters() const;


protected:
    // class DataObserver
    virtual void onNewData(const StreamInfo &data_info, const StreamData &data);


private:
    const std::string c_stream_in        = "disparity";
    const std::string c_stream_out_color = "depth_color_h";
    const std::string c_stream_out_mm    = "depth_mm_h";

    const bool _produce_depth_color = false;
    const bool _produce_depth_mm = false;


    std::vector<uint16_t> _lut_disp_to_depth_mm;

    std::vector<uint16_t> generateDispToDepthMM(
        unsigned width, float fov, float base_line_dist) const;


    void prepareDepthColorAndNotifyObservers(const StreamInfo &data_info, const StreamData &data);
    void prepareDepthMMAndNotifyObservers(const StreamInfo &data_info, const StreamData &data);
};
