#include <iostream>

#include "cnn_host_pipeline.hpp"
#include "../../../shared/timer.hpp"



std::list<std::shared_ptr<NNetPacket>> CNNHostPipeline::getConsumedNNetPackets()
{
    std::list<std::shared_ptr<NNetPacket>> result;

    // search for cnn result packet
    for (auto &packet : _consumed_packets)
    {
        if ((packet->size() > 0) && (packet->stream_name == cnn_result_stream_name))
        {
            std::vector<std::shared_ptr<HostDataPacket>> tensors;
            tensors.push_back(packet);

            std::shared_ptr<NNetPacket> tensor_result(new NNetPacket(
                tensors, _tensors_info));
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

