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
    FrameType type;
    uint32_t height;    // width in pixels
    uint32_t width;     // width in pixels
    uint32_t stride;  
    uint32_t bytesPP;

};

struct Timestamp {
    uint64_t sec, nsec;
};

struct FrameMetadata {

    FrameSpec  spec;       // Frame Specifications
    uint32_t    categ;    // still, preview, ...
    uint32_t    instNo;   // which source
    int32_t     seqNo;    // sequence number of frame
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

    int getFrameType(){
        return (int) spec.type;
    }

    unsigned int getFrameWidth(){
        return spec.width;
    }

    unsigned int getFrameHeight(){
        return spec.height;
    }

    unsigned int getFrameBytesPP(){
        return spec.bytesPP;
    }

    unsigned int getStride(){
        return spec.stride;
    }

    int getCategory(){
        return categ;
    }

    int getInstanceNum(){
        return instNo;
    }

    int getSequenceNum(){
        return seqNo;
    }


};