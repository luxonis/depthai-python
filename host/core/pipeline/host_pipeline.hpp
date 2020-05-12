#pragma once


#include <list>
#include <memory>
#include <set>
#include <tuple>
#include <vector>
#include <mutex>

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>

#include "host_data_packet.hpp"
#include "stream/stream_info.hpp"
#include "general/data_observer.hpp"
#include "stream/stream_data.hpp"


class HostPipeline
    : public DataObserver<StreamInfo, StreamData>
{
protected:
    const unsigned c_data_queue_size = 30;

    boost::lockfree::spsc_queue<std::shared_ptr<HostDataPacket>> _data_queue_lf;
    std::list<std::shared_ptr<HostDataPacket>> _consumed_packets; // TODO: temporary solution

    std::set<std::string> _public_stream_names;    // streams that are passed to public methods
    std::set<std::string> _observing_stream_names; // all streams that pipeline is subscribed

public:
    using DataObserver<StreamInfo, StreamData>::observe;


    HostPipeline();
    virtual ~HostPipeline() {}

    std::list<std::shared_ptr<HostDataPacket>> getAvailableDataPackets();

    void makeStreamPublic(const std::string& stream_name) { _public_stream_names.insert(stream_name); }

    // TODO: temporary solution
    void consumePackets();
    std::list<std::shared_ptr<HostDataPacket>> getConsumedDataPackets();

private:
    std::mutex     q_lock;
    // from DataObserver<StreamInfo, StreamData>
    virtual void onNewData(const StreamInfo& info, const StreamData& data) final;
    // from DataObserver<StreamInfo, StreamData>
    virtual void onNewDataSubject(const StreamInfo &info) final;
};
