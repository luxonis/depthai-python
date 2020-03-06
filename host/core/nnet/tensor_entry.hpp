#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "tensor_info.hpp"
#include "../types.hpp"
#include "../pipeline/depth_calculation_interface.hpp"


struct TensorEntry
{
    const unsigned char* raw_data                    = nullptr;
    Type                 output_properties_type      = Type::UNDEFINED;
    unsigned             output_properties_type_size = 0;
    unsigned             properties_number           = 0;
    int                  nnet_input_width            = 0;
    int                  nnet_input_height           = 0;
    DepthCalculationInterface* depth_calulator       = nullptr;

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
        assert(output_property_key_string_to_index->find(str_index) != output_property_key_string_to_index->end());

        auto arr_index = output_property_key_string_to_index->at(str_index);
        return getFloatByIndex(arr_index);
    }

    float getDistance()
    {
        assert(nullptr != depth_calulator);
        assert(0 != nnet_input_height);
        assert(0 != nnet_input_width);

        float result = depth_calulator->c_distance_undefined;

        if (depth_calulator->canCalculateDistance())
        {
            // TODO: remove hardcode
            auto rx1 = getFloat("left");
            auto ry1 = getFloat("top");
            auto rx2 = getFloat("right");
            auto ry2 = getFloat("bottom");

            return depth_calulator->getDistanceForRectangle(
                rx1, ry1, rx2, ry2, nnet_input_width, nnet_input_height);
        }

        return result;
    }
};
