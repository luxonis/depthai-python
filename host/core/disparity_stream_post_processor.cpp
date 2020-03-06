#include <math.h>

#include <iostream>

#include "disparity_stream_post_processor.hpp"
#include "../../shared/disparity_luts.hpp"


DisparityStreamPostProcessor::DisparityStreamPostProcessor(
    bool produce_d_color,
    bool produce_d_mm
)
    : _produce_depth_color(produce_d_color)
    , _produce_depth_mm(produce_d_mm)
{
    // TODO: remove hardcode
    _lut_disp_to_depth_mm = generateLUTDisparityToDepthMM(1280, 69.0f, 0.035f);
}

std::vector<uint16_t> DisparityStreamPostProcessor::getLutDisparityToMillimeters() const
{
    return _lut_disp_to_depth_mm;
}


void  DisparityStreamPostProcessor::onNewData(
    const StreamInfo &data_info,
    const StreamData &data
)
{
    assert(data_info.name == c_stream_in);

    if (_produce_depth_mm)
    {
        prepareDepthMMAndNotifyObservers(data_info, data);
    }

    if (_produce_depth_color)
    {
        prepareDepthColorAndNotifyObservers(data_info, data);
    }
}

void DisparityStreamPostProcessor::prepareDepthColorAndNotifyObservers(
    const StreamInfo &data_info,
    const StreamData &data
)
{
    // TODO: remove hardcode
    StreamInfo depth_si(c_stream_out_color.c_str(), 720*1280*3, { 720, 1280, 3});

    std::vector<unsigned char> depth_raw(depth_si.size);
    const unsigned char* disp_uc = (const unsigned char*) data.data;

    for (int i = 0, j = 0; i < data.size; ++i, j+=3)
    {
        const unsigned char &disp = *(disp_uc + i);
        depth_raw[j  ] = c_disp_to_color[disp][0];
        depth_raw[j+1] = c_disp_to_color[disp][1];
        depth_raw[j+2] = c_disp_to_color[disp][2];
    }

    StreamData depth_d;
    depth_d.packet_number = data.packet_number;
    depth_d.data = depth_raw.data();
    depth_d.size = depth_raw.size();

    notifyObservers(depth_si, depth_d);
}


void DisparityStreamPostProcessor::prepareDepthMMAndNotifyObservers(
    const StreamInfo &data_info,
    const StreamData &data
)
{
    // TODO: remove hardcode
    StreamInfo depth_si(c_stream_out_mm.c_str(), 720*1280*2, { 720, 1280}, 2);

    std::vector<uint16_t> depth_raw(depth_si.size);
    const unsigned char* disp_uc = (const unsigned char*) data.data;

    for (int i = 0; i < data.size; ++i)
    {
        const unsigned char &disp = *(disp_uc + i);
        depth_raw[i] = _lut_disp_to_depth_mm[disp];
    }

    StreamData depth_d;
    depth_d.packet_number = data.packet_number;
    depth_d.data = depth_raw.data();
    depth_d.size = depth_raw.size();

    notifyObservers(depth_si, depth_d);
}
