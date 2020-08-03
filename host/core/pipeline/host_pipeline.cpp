#include <iostream>
#include <memory>
#include <mutex>

#include "host_pipeline.hpp"

#include "../../../shared/timer.hpp"


HostPipeline::HostPipeline()
    : _data_queue_lf(c_data_queue_size)
{}

void HostPipeline::onNewData(
    const StreamInfo& info,
    const StreamData& data
)
{
    Timer t;

    // std::cout << "--- new data from " << info.name << " , size: " << data.size << "\n";
    bool keep_frame = _public_stream_names.empty() ||
            (_public_stream_names.find(info.getName()) != _public_stream_names.end());
    if(!keep_frame)
    {
        std::cout << "Stream " << info.name << "is not in the stream list" << ":\n";
        return;
    }

    if(data.size > info.size)
    {
        keep_frame = false;
    }

    if(keep_frame == false)
    {
        std::cout << "Received frame " << info.name << " is wrong size: " << data.size << ", expected: " << info.size <<":\n";
        return;
    }

    std::shared_ptr<HostDataPacket> host_data(
        new HostDataPacket(
            data.size,
            data.data,
            info.name,
            info.getDimensionsForSize(data.size),
            info.elem_size
            ));

    std::unique_lock<std::mutex> guard(q_lock);
    if (!_data_queue_lf.push(host_data))
    {
        _data_queue_lf.pop();
        if (!_data_queue_lf.push(host_data))
        {
            std::cerr << "Data queue is full " << info.name << ":\n";
        }
    }

    // std::cout << "===> onNewData " << t.ellapsed_us() << " us\n";
}

void HostPipeline::onNewDataSubject(const StreamInfo &info)
{
    _observing_stream_names.insert(info.name);
}

std::list<std::shared_ptr<HostDataPacket>> HostPipeline::getAvailableDataPackets()
{
    std::list<std::shared_ptr<HostDataPacket>> result;

    _data_queue_lf.consume_all(
        [&result]
        (std::shared_ptr<HostDataPacket>& data)
        {
            result.push_back(data);
        }
    );

    return result;
}

void HostPipeline::consumePackets()
{
    Timer consume_dur;
    _consumed_packets.clear();

    std::unique_lock<std::mutex> guard(q_lock);
    _data_queue_lf.consume_all(
        [this]
        (std::shared_ptr<HostDataPacket>& data)
        {
            // std::cout << "===> c++ wait " << data->constructor_timer.ellapsed_us() << " us\n";            
            this->_consumed_packets.push_back(data);
        }
    );
    guard.unlock();

    if (!this->_consumed_packets.empty())
    {
        // std::cout << "===> c++ consumePackets " << consume_dur.ellapsed_us() / this->_consumed_packets.size() << " us (per 1)\n";
    }
}

std::list<std::shared_ptr<HostDataPacket>> HostPipeline::getConsumedDataPackets()
{
    std::list<std::shared_ptr<HostDataPacket>> result;

    for (auto &packet : _consumed_packets)
    {
        if ((packet->size() > 0) && (packet->stream_name != "metaout"))
        {
            result.push_back(packet);
        }
    }

    return result;
}


