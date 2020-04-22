#pragma once

#include <cstdint>


struct cnn_info
{
    uint16_t cnn_input_width;
    uint16_t cnn_input_height;
    uint16_t cnn_input_num_channels;
    uint16_t number_of_cmx_slices;
    uint16_t number_of_shaves;
    uint32_t offsets[7];
};
