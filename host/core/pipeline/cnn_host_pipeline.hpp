#pragma once

#include <unordered_map>
#include <vector>

#include "host_pipeline.hpp"
#include "../apriltag/april_info.hpp"
#include "../nnet/tensor_info.hpp"
#include "../nnet/nnet_packet.hpp"


class CNNHostPipeline
    : public HostPipeline
{
private:

    const std::string               cnn_result_stream_name = "metaout";
    const std::string               april_stream_name = "aprilout";
    const std::string               c_disparity_stream_name = "disparity";

    const std::vector<TensorInfo>   _tensors_info;

public:
    CNNHostPipeline(const std::vector<TensorInfo>& tensors_info)
        : _tensors_info(tensors_info)
    {}
    virtual ~CNNHostPipeline() {}

    // TODO: is making this public okay?
    std::list<std::shared_ptr<NNetPacket>> getConsumedNNetPackets();
    std::list<AprilInfo> getConsumedAprilPackets();

    std::tuple<
        std::list<std::shared_ptr<NNetPacket>>,
        std::list<std::shared_ptr<HostDataPacket>>
        >
        getAvailableNNetAndDataPackets();
};
