#pragma once

#include <cstdint>


struct StreamData
{
    void*    data;
    uint32_t size;
    uint32_t packet_number;

    uint32_t duration_xlink_read_us = 0;
};
