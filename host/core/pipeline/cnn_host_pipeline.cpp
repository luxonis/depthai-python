#include <iostream>

#include "cnn_host_pipeline.hpp"
#include "cnn_host_pipeline.hpp"
#include "../apriltag/april_info.hpp"
#include <apriltag.h>



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


std::list<AprilInfo> CNNHostPipeline::getConsumedAprilPackets()
{
    std::list<std::shared_ptr<HostDataPacket>> result;
    std::list<AprilInfo> resultApril;

    // search for cnn result packet
    for (auto &packet : _consumed_packets)
    {
        if (packet->stream_name == april_stream_name)
        {
            // deserialize the packet and stick it in a zarray.
            zarray_t* detections;
            int headerOffset = 0;

            AprilInfo aprilInfo;
            std::vector<AprilDetection> packetDetections;

            // create a zarray to access data before packing it into our output type
// asdfasdf TODO: we're making some assumptions about type sizes and endian. Do we care to handle it and how do we want to do that?
            memcpy(&aprilInfo.el_sz, &packet->data[headerOffset], sizeof(int));
            headerOffset = headerOffset + sizeof(int);

            memcpy(&aprilInfo.size, &packet->data[headerOffset], sizeof(int));
            headerOffset = headerOffset + sizeof(int);

            memcpy(&aprilInfo.alloc, &packet->data[headerOffset], sizeof(int));
            headerOffset = headerOffset + sizeof(int);

            for (int i = 0; i < aprilInfo.size; i++) {
                int dataSize = sizeof(int)+sizeof(int)+sizeof(float)+2*sizeof(double)+4*2*sizeof(double);

                AprilDetection currDetection;
                int id;
                int hamming;
                float decision_margin;
                double c[2];
                double p[4][2];

                unsigned char* dataPtr = (unsigned char*) &packet->data[headerOffset];

                // adding id
                memcpy(&currDetection.id, dataPtr, sizeof(int));
                dataPtr = dataPtr + sizeof(int);

                // adding hamming (how many error bits were corrected)
                memcpy(&currDetection.hamming, dataPtr, sizeof(int));
                dataPtr = dataPtr + sizeof(int);

                // adding decision margin
                memcpy(&currDetection.decision_margin, dataPtr, sizeof(float));
                dataPtr = dataPtr + sizeof(float);

                // adding center
                memcpy(&currDetection.c[0], dataPtr, 2*sizeof(double));
                dataPtr = dataPtr + 2*sizeof(double);

                // reading points for the 4 corners
                memcpy(&currDetection.p[0][0], dataPtr, 4*2*sizeof(double));
                dataPtr = dataPtr + 4*2*sizeof(double);

                packetDetections.push_back(currDetection);

                headerOffset = headerOffset + dataSize;
            }

            aprilInfo.detections = packetDetections;

            resultApril.push_back(aprilInfo);
        }
    }

    return resultApril;
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
