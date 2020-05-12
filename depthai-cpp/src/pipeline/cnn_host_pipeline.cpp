#include <iostream>

#include "pipeline/cnn_host_pipeline.hpp"
#include "depth/depth_packet.hpp"
#include "depthai-shared/timer.hpp"


void CNNHostPipeline::searchAndCacheStreamsForDepth()
{
    if (_depth_type == DepthType::DepthAverage)
    {
        searchAndCacheStreamsForDepthAverage();
    }
    else if (_depth_type == DepthType::DepthMedian)
    {
        searchAndCacheStreamsForDepthMedian();
    }
}


void CNNHostPipeline::searchAndCacheStreamsForDepthAverage()
{
    for (auto &packet : _consumed_packets)
    {
        if ((packet->size() > 16) &&
            (c_depth_stream_names.find(packet->stream_name) != c_depth_stream_names.end()))
        {
            // update cache
            _depth_packet_cached = packet;
        }
    }
}


void CNNHostPipeline::searchAndCacheStreamsForDepthMedian()
{
    for (auto &packet : _consumed_packets)
    {
        if ((packet->size() > 16) && (packet->stream_name == c_disparity_stream_name))
        {
            // update cache
            _depth_packet_cached = packet;
        }
    }
}


bool CNNHostPipeline::setHostCalcDepthConfigs(
    const std::string &depth_type, float padding_factor,
        float horizontal_fov_deg, float baseline_m
)
{
    bool result = false;

    do
    {
        // padding_factor
        _padding_factor = padding_factor;
        _horizontal_fov_deg = horizontal_fov_deg;
        _baseline_m = baseline_m;

        // string -> depth_type
        auto it = c_string_to_depth_type.find(depth_type);
        if (it == c_string_to_depth_type.end())
        {
            std::cout << "Wrong depth type: " << depth_type << "\n";
            break;
        }
        else
        {
            _depth_type = it->second;
        }

        // check available streams
        if (_depth_type == DepthType::DepthAverage)
        {
            // for average
            bool has_stream = false;

            for (const auto &sname : c_depth_stream_names)
            {
                if (_observing_stream_names.find(sname) !=
                    _observing_stream_names.end())
                {
                    has_stream = true;
                    break;
                }
            }

            if (!has_stream)
            {
                std::cout << "Theres is no required stream for depth calculation;\n";
                break;
            }
        }
        else if (_depth_type == DepthType::DepthMedian)
        {
            // for median
            if (_observing_stream_names.find(c_disparity_stream_name) ==
                _observing_stream_names.end())
            {
                std::cout << "Theres is no required stream for depth calculation: " << c_disparity_stream_name << "\n";
                break;
            }
        }

        result = true;
    }
    while (false);

    return result;
}


std::list<std::shared_ptr<NNetPacket>> CNNHostPipeline::getConsumedNNetPackets()
{
    std::list<std::shared_ptr<NNetPacket>> result;

    searchAndCacheStreamsForDepth();

    // search for cnn result packet
    for (auto &packet : _consumed_packets)
    {
        if ((packet->size() > 16) && (packet->stream_name == c_nn_result_stream_name))
        {
            std::vector<std::shared_ptr<HostDataPacket>> tensors;
            tensors.push_back(packet);

            std::shared_ptr<NNetPacket> tensor_result(new NNetPacket(
                tensors, _tensors_info, this));
            result.push_back(tensor_result);
        }
    }

    return result;
}


std::tuple<
    std::list<std::shared_ptr<NNetPacket>>,
    std::list<std::shared_ptr<HostDataPacket>>
    >
CNNHostPipeline::getAvailableNNetAndDataPackets()
{
    Timer consume_dur;

    consumePackets();
    auto result = std::make_tuple(
        getConsumedNNetPackets(),
        getConsumedDataPackets()
    );

    int n = std::get<0>(result).size() + std::get<1>(result).size();

    if (n > 0)
    {
        // std::cout << "===> c++ getAvailableNNetAndDataPackets " << consume_dur.ellapsed_us() / n << " us (per 1)\n";
    }

    return result;
}


bool CNNHostPipeline::canCalculateDistance() const
{
    return
        _depth_type != DepthType::DepthNone &&
        nullptr != _depth_packet_cached;
}


float CNNHostPipeline::getDistanceForRectangle(
    float rx1, float ry1,
    float rx2, float ry2,
    int width, int height
)
{
    assert(_depth_type != DepthType::DepthNone);
    assert(nullptr != _depth_packet_cached);

    float result = -1.f;

    auto x1 = rx1;
    auto y1 = ry1;
    auto x2 = rx2;
    auto y2 = ry2;

    if (_depth_type == DepthType::DepthAverage)
    {
        result = DepthPacket(_depth_packet_cached)
            .getAverageDistanceForRectangle(x1, y1, x2, y2, _padding_factor);
    }
    else if (_depth_type == DepthType::DepthMedian)
    {
        result = DepthPacket(_depth_packet_cached)
            .getMedianDistanceForRectangle(x1, y1, x2, y2,
                                          _padding_factor, _horizontal_fov_deg, _baseline_m);
    }

    return result;
}
