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


const int TRANS_INT_SIZE = 4;
const int TRANS_FLOAT_SIZE = 4;
const int TRANS_DOUBLE_SIZE = 8;

const int PT_SIZE = 2;
const int RECT_SIZE = 4;

int isLittleEndian(){
    int x = 1;
    return (int) (((char *)&x)[0]);
}

void read4bytes(unsigned char * output, unsigned char * input){
    if( isLittleEndian() ){
        output[0] = input[0];
        output[1] = input[1];
        output[2] = input[2];
        output[3] = input[3];
    } else {
        output[0] = input[3];
        output[1] = input[2];
        output[2] = input[1];
        output[3] = input[0];
    }
}

void read8bytes(unsigned char * output, unsigned char * input){
    if( isLittleEndian() ){
        output[0] = input[0];
        output[1] = input[1];
        output[2] = input[2];
        output[3] = input[3];
        output[4] = input[4];
        output[5] = input[5];
        output[6] = input[6];
        output[7] = input[7];
    } else {
        output[0] = input[7];
        output[1] = input[6];
        output[2] = input[5];
        output[3] = input[4];
        output[4] = input[3];
        output[5] = input[2];
        output[6] = input[1];
        output[7] = input[0];
    }
}

// data being sent by depthai is little endian, we'll need to convert it if the host is expecting the opposite.
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
            read4bytes( (unsigned char*) &aprilInfo.el_sz, &packet->data[headerOffset]);
            headerOffset = headerOffset + TRANS_INT_SIZE;

            read4bytes( (unsigned char*) &aprilInfo.size, &packet->data[headerOffset]);
            headerOffset = headerOffset + TRANS_INT_SIZE;

            read4bytes( (unsigned char*) &aprilInfo.alloc, &packet->data[headerOffset]);
            headerOffset = headerOffset + TRANS_INT_SIZE;

            for (int i = 0; i < aprilInfo.size; i++) {
                int dataSize = TRANS_INT_SIZE+TRANS_INT_SIZE+TRANS_FLOAT_SIZE+2*TRANS_DOUBLE_SIZE+4*2*TRANS_DOUBLE_SIZE;

                AprilDetection currDetection;

                unsigned char* dataPtr = (unsigned char*) &packet->data[headerOffset];

                // adding id
                read4bytes( (unsigned char*) &currDetection.id, dataPtr);
                dataPtr = dataPtr + TRANS_INT_SIZE;

                // adding hamming (how many error bits were corrected)
                read4bytes( (unsigned char*) &currDetection.hamming, dataPtr);
                dataPtr = dataPtr + TRANS_INT_SIZE;

                // adding decision margin
                read4bytes( (unsigned char*) &currDetection.decision_margin, dataPtr);
                dataPtr = dataPtr + TRANS_FLOAT_SIZE;

                // adding center
                for(int j=0; j<PT_SIZE; j++){
                    read8bytes( (unsigned char*) &currDetection.c[j], dataPtr);
                    dataPtr = dataPtr + TRANS_DOUBLE_SIZE;
                }

                // reading points for the 4 corners
                for(int j=0; j<RECT_SIZE; j++){
                    for(int k=0; k<PT_SIZE; k++){
                        read8bytes( (unsigned char*) &currDetection.p[j][k], dataPtr);
                        dataPtr = dataPtr + TRANS_DOUBLE_SIZE;
                    }
                }

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
