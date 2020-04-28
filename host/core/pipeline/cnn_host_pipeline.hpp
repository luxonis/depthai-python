#pragma once

#include <unordered_map>
#include <vector>

#include "host_pipeline.hpp"
#include "../nnet/tensor_info.hpp"
#include "../nnet/nnet_packet.hpp"


class CNNHostPipeline
    : public HostPipeline
{
private:

    const std::string               cnn_result_stream_name = "metaout";
    const std::string               c_disparity_stream_name = "disparity";

    const std::vector<TensorInfo>   _tensors_info;


    std::list<std::shared_ptr<NNetPacket>> getConsumedNNetPackets();

public:
    CNNHostPipeline(const std::vector<TensorInfo>& tensors_info)
        : _tensors_info(tensors_info)
    {}
    virtual ~CNNHostPipeline() {}


    std::tuple<
        std::list<std::shared_ptr<NNetPacket>>,
        std::list<std::shared_ptr<HostDataPacket>>
        >
        getAvailableNNetAndDataPackets();

};
