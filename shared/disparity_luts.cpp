#include <cmath>

#include <cstdint>
#include <vector>

#include "disparity_luts.hpp"


float calculateDisparityToDepthMM(
    unsigned disparity,
    unsigned width,
    float fov,
    float base_line_dist
)
{
    if (disparity == 0)
    {
        return -1.f;
    }
    else
    {
        const float  pi             = std::acos(-1);
        float        focal_length   = width / (2.f * std::tan(fov / 2 / 180.f * pi));
        float z_m = (focal_length * base_line_dist / disparity);
        return z_m * 1000.f;
    }
}

std::vector<uint16_t> generateLUTDisparityToDepthMM(
    unsigned width,
    float fov,
    float base_line_dist
)
{
    std::vector<uint16_t> result(256, 0);

    const float  pi             = std::acos(-1);
    float        focal_length   = width / (2.f * std::tan(fov / 2 / 180.f * pi));

    for (int i = 0; i < DISPARITY_SIZE; ++i)
    {
        float z_m = (focal_length * base_line_dist / i);
        result[i] = std::min(65535.f, z_m * 1000.f); // m -> mm
    }

    return result;
}
