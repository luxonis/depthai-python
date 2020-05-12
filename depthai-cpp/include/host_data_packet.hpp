#pragma once

#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include <memory>
#include <numeric>
#include <vector>

#include "boost/optional.hpp"

#include "depthai-shared/timer.hpp"

#include "depthai-shared/metadata/frame_metadata.hpp"


#ifdef HOST_PYTHON_MODULE
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
#endif



struct HostDataPacket
{
    HostDataPacket(
        unsigned size,
        void* in_data,
        const std::string& stream_name_,
        std::vector<int> dimensions_,
        int elem_size_
    )
        : stream_name(stream_name_)
        , dimensions(dimensions_)
        , elem_size(elem_size_)
    {
        int frameSize = size;

        FrameMetadata metadata;
        // Copy metadata structure from end of packet
        memcpy( &metadata, ((uint8_t*) in_data) + size - sizeof(FrameMetadata), sizeof(FrameMetadata) );
        // Check if metadata is valid
        if(metadata.isValid()){
            // copy only frame data
            frameSize = metadata.frameSize;
            //printf("Stream: %s (size: %d, frameSize: %d), metadata packet valid: w:%d, h:%d, t:%d, %6.3f\n", stream_name_.c_str(),size, frameSize, metadata.spec.width,metadata.spec.height, metadata.spec.type, metadata.getTimestamp());
            // set opt_metadata
            opt_metadata = metadata;
        } else {
            //printf("Stream: %s (size: %d), Metadata packet NOT valid\n",stream_name_.c_str(), size);
        }

        data.resize(frameSize);
        memcpy(data.data(), in_data, frameSize);

        constructor_timer = Timer();
    }

    unsigned size()
    {
        return data.size();
    }

    const unsigned char* getData() const
    {
        return data.data();
    }

    std::string getDataAsString()
    {
        assert(data[data.size() - 1] == 0); // checking '\0'
        return reinterpret_cast<const char*>(&data[0]);
    }

    boost::optional<FrameMetadata> getMetadata(){
        return opt_metadata;
    }

    boost::optional<FrameMetadata> opt_metadata;
    std::vector<unsigned char> data;
    std::string stream_name;
    const std::vector<int> dimensions;
    int elem_size;

    Timer constructor_timer;
};
