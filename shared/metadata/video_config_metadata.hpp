#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <algorithm>

struct VideoConfig{
    enum class RateCtrlMode{
        CBR,
        VBR
    } rateCtrlMode;
    
    enum class Profile{
        H264_BASELINE,
        H264_MAIN,
        H264_HIGH,
        H265_MAIN
    } profile;

    int bitrate, maxBitrate; //bps
    int keyframeFrequency; //every x number of frames a keyframe is inserted
    int numBFrames; 
    int quality; //0-100

    nlohmann::json toJson(){
        
        nlohmann::json obj;

        obj["rateCtrlMode"] = (int) rateCtrlMode;
        obj["profile"] = (int) profile;

        obj["bitrate"] = bitrate;
        obj["maxBitrate"] = maxBitrate;
        obj["keyframeFrequency"] = keyframeFrequency;
        obj["numBFrames"] = numBFrames;
        obj["quality"] = quality;

        return obj;

    }

    static VideoConfig fromJson(nlohmann::json obj){
        
        VideoConfig config = {};

        //Check if rateCtrlMode is string or int
        if(obj.count("rateCtrlMode") > 0){
            if(obj["rateCtrlMode"].is_string()){
                auto mode = obj["rateCtrlMode"].get<std::string>();
                std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

                if(mode == "cbr"){
                    config.rateCtrlMode = RateCtrlMode::CBR;
                } else if(mode == "vbr"){
                    config.rateCtrlMode = RateCtrlMode::VBR;
                }
            } else if(obj["rateCtrlMode"].is_number_integer()){
                config.rateCtrlMode = (RateCtrlMode) obj["rateCtrlMode"].get<int>();
            }
        }
        if(obj.count("profile") > 0){
            if(obj["profile"].is_string()){
                auto profile = obj["profile"].get<std::string>();
                std::transform(profile.begin(), profile.end(), profile.begin(), ::tolower);
                
                if(profile == "h264_baseline"){
                    config.profile = Profile::H264_BASELINE;
                } else if(profile == "h264_main"){
                    config.profile = Profile::H264_MAIN;
                } else if(profile == "h264_high"){
                    config.profile = Profile::H264_HIGH;
                } else if(profile == "h265_main"){
                    config.profile = Profile::H265_MAIN;
                }
            } else if(obj["profile"].is_number_integer()){
                config.profile = (Profile) obj["profile"].get<int>();
            }
        }


        if(obj.count("bitrate") > 0){
            config.bitrate = obj["bitrate"].get<int>();
        }
        if(obj.count("maxBitrate") > 0){
            config.maxBitrate = obj["maxBitrate"].get<int>();
        }
        if(obj.count("keyframeFrequency") > 0){
            config.keyframeFrequency = obj["keyframeFrequency"].get<int>();
        }
        if(obj.count("numBFrames") > 0){
            config.numBFrames = obj["numBFrames"].get<int>();
        }
        if(obj.count("quality") > 0){
            config.quality = obj["quality"].get<int>();
        }

        return config;
    }

};
