#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <cfloat>

#include "tensor_info.hpp"
#include "../types.hpp"


struct TensorEntry
{
    const unsigned char* raw_data                    = nullptr;
    Type                 output_properties_type      = Type::UNDEFINED;
    unsigned             output_properties_type_size = 0;
    unsigned             properties_number           = 0;
    int                  nnet_input_width            = 0;
    int                  nnet_input_height           = 0;

    const std::unordered_map<std::string, unsigned>*              output_property_key_string_to_index   = nullptr;
    const std::vector<std::unordered_map<std::string, unsigned>>* output_property_value_string_to_index = nullptr;


    unsigned getPropertiesNumber() const
    {
        return properties_number;
    }

    float getFloatByIndex(int index) const // TODO: make template
    {
        assert(output_properties_type == Type::F16); // TODO: remove this
        const void* void_ptr = raw_data + index * output_properties_type_size;

        const uint16_t* float_ptr = (uint16_t*) void_ptr;
        return float16to32(*float_ptr);
    }

    float getFloat(const std::string &str_index) const
    {
        assert(output_properties_type == Type::F16); // TODO: remove this
        assert(nullptr != output_property_key_string_to_index);
        if(output_property_key_string_to_index->find(str_index) == output_property_key_string_to_index->end()) {
		      throw std::runtime_error("There is no \"" + str_index + "\" property defined in 'property_key_mapping' field, check blob config file.");
        }

        auto arr_index = output_property_key_string_to_index->at(str_index);
        return getFloatByIndex(arr_index);
    }

    bool checkValidTensorEntry() const
    {
        for(int idx = 0; idx < getPropertiesNumber(); idx++)
        {
            float tensorValue = getFloatByIndex(idx);
            if(std::isnan(tensorValue) || std::isinf(tensorValue))
            {
                printf("invalid tensor packet, discarding \n");
                return false;
            }
        }
        return true;
    }

};
