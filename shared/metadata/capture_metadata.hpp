#pragma once

#include <cstdint>
#include <cstring>

// POD
struct CaptureMetadata {

    enum class Command : uint8_t {
        NO_COMMAND = 0,
        CAPTURE_STILL,
        AUTOFOCUS_TRIGGER,
        AUTOFOCUS_MODE
    };

    enum class AutofocusMode : uint8_t {
        AF_MODE_AUTO = 0,
        AF_MODE_MACRO, 
        AF_MODE_CONTINUOUS_VIDEO,
        AF_MODE_CONTINUOUS_PICTURE,
        AF_MODE_EDOF
    };

    Command command;
    uint8_t data[32];

    // Helper functions
    static CaptureMetadata createStillCapture(){
        CaptureMetadata metadata = {};
        metadata.command = Command::CAPTURE_STILL;
        return metadata;
    }

    static CaptureMetadata createAfMode(AutofocusMode mode){
        CaptureMetadata metadata = {};
        metadata.command = Command::AUTOFOCUS_MODE;
        memcpy(metadata.data, &mode, sizeof(AutofocusMode));
        return metadata;
    }

    static CaptureMetadata createAfTrigger(){
        CaptureMetadata metadata = {};
        metadata.command = Command::AUTOFOCUS_TRIGGER;
        return metadata;
    }

    CaptureMetadata::AutofocusMode getAfMode(){
        CaptureMetadata::AutofocusMode mode = {};
        memcpy(&mode, data, sizeof(AutofocusMode));
        return mode;
    }

};



