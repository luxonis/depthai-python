#pragma once

#include <unordered_map>
#include <vector>

#include "depth_calculation_interface.hpp"
#include "host_pipeline.hpp"
#include "../nnet/tensor_info.hpp"
#include "../nnet/nnet_packet.hpp"


class CNNHostPipeline
    : public HostPipeline
    , public DepthCalculationInterface
{
private:
    enum class DepthType {
        DepthNone,
        DepthAverage,
        DepthMedian
    };

    const std::unordered_map<std::string, DepthType> c_string_to_depth_type =
    {
        {"",        DepthType::DepthNone},
        {"average", DepthType::DepthAverage},
        {"median",  DepthType::DepthMedian}
    };


    const std::string               c_nn_result_stream_name = "metaout";
    const std::string               c_disparity_stream_name = "disparity";
    const std::set<std::string>     c_depth_stream_names = {"depth_mm_h", "depth"};

    const std::vector<TensorInfo>   _tensors_info;
    DepthType                       _depth_type = DepthType::DepthNone;
    float                           _padding_factor = 0.3f;
    float                           _horizontal_fov_deg = 69.f;
    float                           _baseline_m = 0.035f;
    std::shared_ptr<HostDataPacket> _depth_packet_cached;


    void searchAndCacheStreamsForDepth();
    void searchAndCacheStreamsForDepthAverage();
    void searchAndCacheStreamsForDepthMedian();

    std::list<std::shared_ptr<NNetPacket>> getConsumedNNetPackets();

public:
    CNNHostPipeline(const std::vector<TensorInfo>& tensors_info)
        : _tensors_info(tensors_info)
    {}
    virtual ~CNNHostPipeline() {}

    bool setHostCalcDepthConfigs(
        const std::string &depth_type, float padding_factor,
        float horizontal_fov_deg, float baseline_m);


    std::tuple<
        std::list<std::shared_ptr<NNetPacket>>,
        std::list<std::shared_ptr<HostDataPacket>>
        >
        getAvailableNNetAndDataPackets();


    virtual bool canCalculateDistance() const;

    virtual float getDistanceForRectangle(
        float rx1, float ry1,
        float rx2, float ry2,
        int width, int height
        );
};
