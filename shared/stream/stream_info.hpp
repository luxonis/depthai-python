#ifndef STREAM_INFO_HPP
#define STREAM_INFO_HPP


#include <stdint.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>


struct StreamInfo
{
    const char*       name       = nullptr;           // name passed to Myriad
    uint32_t          size       = 0;                 // packet transfer size
    std::vector<int>  dimensions = {-1};              // examples: {640, 480, 3}, {1000, -1}; -1 means dimention depend on size
    int               elem_size  = 1;

    StreamInfo() {}
    StreamInfo(const char* name_)
        : name(name_)
        { do_size_correction_if_needed(); }
    StreamInfo(const char* name_, uint32_t size_)
        : name(name_), size(size_)
        { do_size_correction_if_needed(); }
    StreamInfo(const char* name_, uint32_t size_, const std::vector<int> &dims)
        : name(name_), size(size_), dimensions(dims)
        { do_size_correction_if_needed(); }
    StreamInfo(const char* name_, uint32_t size_, const std::vector<int> &dims, int elem_size_)
        : name(name_), size(size_), dimensions(dims), elem_size(elem_size_)
        { do_size_correction_if_needed(); }

    const char*      getName() const { return name; }
    std::vector<int> getDimensionsForSize(uint32_t req_size) const;

private:
    void do_size_correction_if_needed()
    {
        if (size == 0)
        {
            auto unknown_dims = std::count(std::begin(dimensions), std::end(dimensions), -1);
            if (unknown_dims == 0)
            {
                auto dims_mul = std::accumulate(std::begin(dimensions), std::end(dimensions), 1, std::multiplies<int>());
                size = dims_mul * elem_size;
            }
        }
    }
};


#endif  // STREAM_INFO_HPP
