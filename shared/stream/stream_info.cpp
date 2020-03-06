#include <assert.h>
#include <cstdio>

#include <algorithm>
#include <functional>
#include <numeric>

#include "stream_info.hpp"


std::vector<int> StreamInfo::getDimensionsForSize(uint32_t req_size) const
{
    int elems_n = req_size / elem_size;

    if (dimensions.empty())
    {
        return { int(elems_n) };
    }
    else
    {
        auto unknown_dims = std::count(std::begin(dimensions), std::end(dimensions), -1);

        if (unknown_dims > 1)
        {
            assert(false);
            return { int(elems_n) };
        }
        else
        {
            int dims_mul = std::accumulate(std::begin(dimensions), std::end(dimensions), 1,
                [](int a, int b)
                {
                    if (b == -1) { return a; }
                    else         { return a * b; }
                }
            );

            // there is no unknown dimension
            if (unknown_dims == 0)
            {
                // printf("Sizes: elems_n = %d, req_size = %d, elem_size = %d, dims_mul = %d\n", elems_n, req_size, elem_size, dims_mul);
                // assert(dims_mul == elems_n);  // TODO: remove this. Issue with previewout from cnn (+256 bytes)
                return dimensions;
            }
            // one unknown dimension
            else if (unknown_dims == 1)
            {
                assert(elems_n % dims_mul == 0);
                auto result = dimensions;
                auto it = std::find(std::begin(result), std::end(result), -1);
                int new_dim = elems_n / dims_mul;
                *it = new_dim;
                return result;
            }
            // this should not happend
            else
            {
                assert(false);
            }
        }
    }
}
