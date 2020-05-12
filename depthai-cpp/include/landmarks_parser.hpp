#pragma once

#include <iostream>

#include "float16_to_float32_converter.hpp"

typedef struct {
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2; 
    uint16_t x3;
    uint16_t y3;
    uint16_t x4;
    uint16_t y4;
} landmarks_out_struct;



static std::vector<float> parseLandmarks(const void* data)
{
    const uint16_t* d = (const uint16_t*) data;
    
    // mobilenet_detection det;


    // det.id = std::max(0.f, f16tof32(d[0])); // TODO: check id max is needed
    std::vector<float> landmarks;
    for(int i = 0; i < 10; i++)
    {
        float c = f16tof32(d[i]);
        landmarks.push_back(c);

    }
    

    
    

    
    // det.label = std::max(0.f, f16tof32(d[1])); // TODO: check id max is needed
    // det.label = f16tof32(d[1]);

    // det.confidence = f16tof32(d[2]);
    // det.confidence = (det.confidence > 1.0f || det.confidence < 0.0f) ? 0 : det.confidence;

    // det.left = f16tof32(d[3]);
    // det.top = f16tof32(d[4]);
    // det.right = f16tof32(d[5]);
    // det.bottom = f16tof32(d[6]);

    return landmarks;
}
