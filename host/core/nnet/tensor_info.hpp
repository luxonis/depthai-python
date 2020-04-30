#pragma once

#include <assert.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "../types.hpp"


// Tensor -> Entry -> Propery

struct TensorInfo
{
    std::string output_tensor_name;

    std::vector<int> output_dimensions;
    int output_entry_iteration_index = -1;
    int nnet_input_width  = 0;
    int nnet_input_height = 0;

    uint32_t offset = 0;

    std::vector<int> output_properties_dimensions;

    std::vector<std::vector<std::string>>                               output_property_key_index_to_string;
    std::vector<std::unordered_map<std::string, unsigned>>              output_property_key_string_to_index;
    std::vector<std::vector<std::vector<std::string>>>                  output_property_value_index_to_string;
    std::vector<std::vector<std::unordered_map<std::string, unsigned>>> output_property_value_string_to_index;

    std::string output_postprocess_filtration; // "softmax", ...

    Type output_properties_type = Type::UNDEFINED;


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
            assert(output_dimensions[output_entry_iteration_index] != 0);
            return getTensorSize() / output_dimensions[output_entry_iteration_index];
        }
    }
};
