#pragma once

#include <assert.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "../types.hpp"


struct AprilDetection
{
    int id;
    int hamming;
    float decision_margin;
    double c[2];
    double p[4][2];

    std::array<double,2> getCenter() const
    {
        std::array<double, 2> result;
    	std::copy(std::begin(c), std::end(c), result.begin());

        return result;
    }

    std::array<std::array<double, 2>, 4> getCorners() const
    {
        std::array<std::array<double, 2>, 4> result;
    	result[0][0] = p[0][0];
    	result[0][1] = p[0][1];
    	result[1][0] = p[1][0];
    	result[1][1] = p[1][1];
    	result[2][0] = p[2][0];
    	result[2][1] = p[2][1];
    	result[3][0] = p[3][0];
    	result[3][1] = p[3][1];

        return result;
    }
};

struct AprilInfo
{
    int el_sz;
    int size;
    int alloc;

    std::vector<AprilDetection> detections;

/*
    int getEntriesNumber() const
    {
        if (-1 == output_entry_iteration_index)
        {
            return 1;
        }
        else
        {
            assert(output_entry_iteration_index < output_dimensions.size());
            return output_dimensions[output_entry_iteration_index];
        }
    }

    int getTensorSize() const
    {
        unsigned sz = size_of_type(output_properties_type);
        for (auto dim : output_dimensions)
        {
            sz *= dim;
        }
        return sz;
    }

    int getEntryByteSize() const
    {
        if (-1 == output_entry_iteration_index)
        {
            return getTensorSize();
        }
        else
        {
            assert(output_entry_iteration_index < output_dimensions.size());
            //assert(false); // TODO: correct ?
            return getTensorSize() / output_dimensions[output_entry_iteration_index];
        }
    }
*/


};


