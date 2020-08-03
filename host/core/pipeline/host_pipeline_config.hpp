#pragma once

#include <string>
#include <vector>

#include "../../shared/json_helper.hpp"


struct HostPipelineConfig
{
    struct StreamRequest {
        std::string name;
        std::string data_type;
        float       max_fps   = 0.f;

        StreamRequest(const std::string &name_) : name(name_) {}
    };

    std::vector<StreamRequest> streams;
    std::vector<std::string>   streams_public;

    struct Depth {
        std::string calibration_file;
        std::string type;
        float       padding_factor = 0.3f;
        float       depth_limit_m = 10.0f;
        float       confidence_threshold = 0.5f;
    } depth;

    struct AI
    {
        std::string blob_file;
        std::string blob_file_config;
        std::string blob_file2;
        std::string blob_file_config2;
        std::string camera_input = "rgb";
        bool calc_dist_to_bb = false;
        bool keep_aspect_ratio = true;
        int32_t shaves = 7;
        int32_t cmx_slices = 7;
        int32_t NN_engines = 1;
    } ai;

    struct OT
    {
        int32_t max_tracklets        = 20;
        float   confidence_threshold = 0.5;
    } ot;

    struct BoardConfig
    {
        bool  clear_eeprom = false;
        bool  store_to_eeprom = false;
        bool  override_eeprom = false;
        bool  swap_left_and_right_cameras = false;
        float left_fov_deg = 69.f;
        float rgb_fov_deg = 69.f;
        float left_to_right_distance_m = 0.035f; // meters, not centimeters
        float left_to_rgb_distance_m = 0;
        bool stereo_center_crop = false;
        std::string name;
        std::string revision;
    } board_config;

    struct RGBCamConfig
    {
        int32_t resolution_w = 0; //auto
        int32_t resolution_h = 1080;
        float fps = 30.f;
    } rgb_cam_config;

    struct MonoCamConfig
    {
        int32_t resolution_w = 0; //auto
        int32_t resolution_h = 720;
        float fps = 30.f;
    } mono_cam_config;

    struct AppConfig
    {
        bool sync_video_meta_streams = false;
    } app_config;

    bool initWithJSON(const json &json_obj);
    bool hasStream(const std::string &stream_name) const;
};
