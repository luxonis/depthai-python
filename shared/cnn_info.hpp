#pragma once

#include <cstdint>

struct nn_to_depth_bbox_map
{
    uint16_t offset_x;
    uint16_t offset_y;
    uint16_t max_width;
    uint16_t max_height;
};

struct cnn_info
{
    uint16_t cnn_input_width;
    uint16_t cnn_input_height;
    uint16_t cnn_input_num_channels;
    uint16_t number_of_cmx_slices;
    uint16_t number_of_shaves;
    uint32_t offsets[7];
    nn_to_depth_bbox_map nn_to_depth;
    int32_t satisfied_resources;
};
