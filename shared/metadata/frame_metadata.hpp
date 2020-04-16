#pragma once

#include <cstdint>

enum class FrameType
{
    GRAY = 15,
    BISTREAM = 21,
    // TODO, fill
};

struct FrameSpec
{
    FrameType      type;
    unsigned int   height;    // width in pixels
    unsigned int   width;     // width in pixels
    unsigned int   stride;  
    unsigned int   bytesPP;

};

struct Timestamp {
    uint64_t sec, nsec;
};

struct FrameMetadata {

    FrameSpec  spec;       // Frame Specifications
    uint32_t    categ;    // still, preview, ...
    uint32_t    instNo;   // which source
    int         seqNo;    // sequence number of frame
    Timestamp   ts;       // generation timestamp
    uint32_t    frameSize; // Size of frame at the beggining of buffer
private:
    uint32_t    magic;

public:
    constexpr static uint32_t MAGIC_EXPECTED = 0xCAFEF00D;
    FrameMetadata() : magic(MAGIC_EXPECTED){ }
    bool isValid(){
        
        // Test for magic field
        if(magic != MAGIC_EXPECTED){
            return false;
        }
        
        // TODO, add additional tests like dataSize, specs, etc in case a pixel matches MAGIC_EXPECTED

        return true;
    }

    double getTimestamp(){
        return ts.sec + ts.nsec / 1000000000.0;
    }

};