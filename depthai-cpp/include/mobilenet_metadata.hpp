#pragma once

#include "float16_to_float32_converter.hpp"

typedef struct {
    uint16_t id;
    uint16_t label;
    uint16_t confidence;
    uint16_t left;
    uint16_t top;
    uint16_t right;
    uint16_t bottom;    
} mobilenet_out_struct;

typedef struct {
    int id;
    int label;
    float confidence;
    float left, top, right, bottom;
} mobilenet_detection;

static mobilenet_detection parseMobilenetMetadata(const void* data)
{
    const uint16_t* d = (const uint16_t*) data;
    
    mobilenet_detection det;

    // det.id = std::max(0.f, f16tof32(d[0])); // TODO: check id max is needed
    det.id = f16tof32(d[0]);
    // det.label = std::max(0.f, f16tof32(d[1])); // TODO: check id max is needed
    det.label = f16tof32(d[1]);

    det.confidence = f16tof32(d[2]);
    // det.confidence = (det.confidence > 1.0f || det.confidence < 0.0f) ? 0 : det.confidence;

    det.left = f16tof32(d[3]);
    det.top = f16tof32(d[4]);
    det.right = f16tof32(d[5]);
    det.bottom = f16tof32(d[6]);

    return det;
}
