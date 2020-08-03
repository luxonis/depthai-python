#include <iostream>

#include "host_pipeline_config.hpp"


static std::unordered_map<int, int> rgb_cam_supported_configs =
{
    {1080, 0},
    {2160, 1},
    {3040, 2}
};

static std::unordered_map<int, int> mono_cam_supported_configs =
{
    {720, 0},
    {800, 1},
    {400, 2},
};

#define WARNING "\033[1;5;31m"
#define ENDC "\033[0m"


bool HostPipelineConfig::initWithJSON(const json &json_obj)
{
    bool result = false;

    do
    {
        // "streams"
        streams.clear();
        streams_public.clear();

        if (json_obj.contains("streams"))
        {
            for (auto it : json_obj.at("streams"))
            {
                // "metaout"
                if (it.is_string())
                {
                    const auto &str = it.get<std::string>();
                    streams.emplace_back(str);
                    streams_public.push_back(str);
                }
                else
                // {"name": "depth", "data_type": "uint16", "max_fps": 4.0}
                {
                    const auto &name = it.at("name").get<std::string>();
                    streams.emplace_back(name);
                    auto &stream = streams.back();

                    if (it.contains("data_type"))
                    {
                        stream.data_type = it.at("data_type").get<std::string>();
                    }

                    if (it.contains("max_fps"))
                    {
                        stream.max_fps   = it.at("max_fps").get<float>();
                    }
                }
            }
        }


        // "depth"
        if (json_obj.contains("depth"))
        {
            auto& depth_obj = json_obj.at("depth");

            // "calibration_file"
            if (depth_obj.contains("calibration_file"))
            {
                depth.calibration_file = depth_obj.at("calibration_file").get<std::string>();
            }

            // "type"
            if (depth_obj.contains("type"))
            {
                depth.type = depth_obj.at("type").get<std::string>();
            }

            // "padding_factor"
            if (depth_obj.contains("padding_factor"))
            {
                depth.padding_factor = depth_obj.at("padding_factor").get<float>();

                if (depth.padding_factor < 0.f || depth.padding_factor > 1.f)
                {
                    std::cerr << WARNING "padding_factor should be in the range [0 .. 1]\n" ENDC;
                    break;
                }
            }

            if (depth_obj.contains("depth_limit_m"))
            {
                depth.depth_limit_m = depth_obj.at("depth_limit_m").get<float>();
            }

            if (depth_obj.contains("confidence_threshold"))
            {
                depth.confidence_threshold = depth_obj.at("confidence_threshold").get<float>();

                if (depth.confidence_threshold < 0.f || depth.confidence_threshold > 1.f)
                {
                    std::cerr << WARNING "confidence_threshold should be in the range [0 .. 1]\n" ENDC;
                    break;
                }
            }
        }

        // "ai"
        if (json_obj.contains("ai"))
        {
            auto& ai_obj = json_obj.at("ai");

            // "blob_file"
            if (ai_obj.contains("blob_file"))
            {
                ai.blob_file = ai_obj.at("blob_file").get<std::string>();
            }

            // "blob_file_config"
            if (ai_obj.contains("blob_file_config"))
            {
                ai.blob_file_config = ai_obj.at("blob_file_config").get<std::string>();
            }

            if (ai_obj.contains("blob_file2"))
            {
                ai.blob_file2 = ai_obj.at("blob_file2").get<std::string>();
            }

            if (ai_obj.contains("blob_file_config2"))
            {
                ai.blob_file_config2 = ai_obj.at("blob_file_config2").get<std::string>();
            }

            if (ai_obj.contains("camera_input"))
            {
                ai.camera_input = ai_obj.at("camera_input").get<std::string>();
            }

            if (ai_obj.contains("calc_dist_to_bb"))
            {
                ai.calc_dist_to_bb = ai_obj.at("calc_dist_to_bb").get<bool>();
            }

            if (ai_obj.contains("keep_aspect_ratio"))
            {
                ai.keep_aspect_ratio = ai_obj.at("keep_aspect_ratio").get<bool>();
            }

            if (ai_obj.contains("shaves"))
            {
                ai.shaves = ai_obj.at("shaves").get<int32_t>();
            }
            if (ai.shaves <= 0 || ai.shaves > 16)
            {
                std::cerr << WARNING "ai.shaves should be in the range (0 .. 16]\n" ENDC;
                break;
            }

            if (ai_obj.contains("cmx_slices"))
            {
                ai.cmx_slices = ai_obj.at("cmx_slices").get<int32_t>();
            }
            if (ai.cmx_slices <= 0 || ai.cmx_slices > 19)
            {
                std::cerr << WARNING "ai.cmx_slices should be in the range (0 .. 19]\n" ENDC;
                break;
            }

            if (ai.shaves > ai.cmx_slices)
            {
                std::cerr << WARNING "ai.shaves should be <= than ai.cmx_slices\n" ENDC;
                break;
            }


            if (ai_obj.contains("NN_engines"))
            {
                ai.NN_engines = ai_obj.at("NN_engines").get<int32_t>();
            }
            if (ai.NN_engines < 0 || ai.NN_engines > 2)
            {
                std::cerr << WARNING "ai.NN_engines should be in the range [0 .. 2]\n" ENDC;
                break;
            }

        }

        // "ot"
        if (json_obj.contains("ot"))
        {
            auto& ot_obj = json_obj.at("ot");
            
            if (ot_obj.contains("max_tracklets"))
            {
                ot.max_tracklets = ot_obj.at("max_tracklets").get<int32_t>();
                if (ot.max_tracklets < 0 || ot.max_tracklets > 20)
                {
                    std::cerr << WARNING "ot max_tracklets should be in the range [0 .. 20]\n" ENDC;
                    break;
                }
            }

            if (ot_obj.contains("confidence_threshold"))
            {
                ot.confidence_threshold = ot_obj.at("confidence_threshold").get<float>();
                if (ot.confidence_threshold < 0.f || ot.confidence_threshold > 1.f)
                {
                    std::cerr << WARNING "ot confidence_threshold should be in the range [0 .. 1]\n" ENDC;
                    break;
                }
            }
        }

        // "board_config"
        if (json_obj.contains("board_config"))
        {
            auto& board_conf_obj = json_obj.at("board_config");

            if (board_conf_obj.contains("store_to_eeprom"))
            {
                board_config.store_to_eeprom = board_conf_obj.at("store_to_eeprom").get<bool>();
            }

            if (board_conf_obj.contains("clear_eeprom"))
            {
                board_config.clear_eeprom = board_conf_obj.at("clear_eeprom").get<bool>();
            }

            if (board_conf_obj.contains("override_eeprom"))
            {
                board_config.override_eeprom = board_conf_obj.at("override_eeprom").get<bool>();
            }

            if (board_conf_obj.contains("stereo_center_crop"))
            {
                board_config.stereo_center_crop = board_conf_obj.at("stereo_center_crop").get<bool>();
            }

            // "blob_file_config"
            if (board_conf_obj.contains("swap_left_and_right_cameras"))
            {
                board_config.swap_left_and_right_cameras = board_conf_obj.at("swap_left_and_right_cameras").get<bool>();
            }

            // "left_fov_deg"
            if (board_conf_obj.contains("left_fov_deg"))
            {
                board_config.left_fov_deg = board_conf_obj.at("left_fov_deg").get<float>();
            }

            if (board_conf_obj.contains("rgb_fov_deg"))
            {
                board_config.rgb_fov_deg = board_conf_obj.at("rgb_fov_deg").get<float>();
            }

            // "left_to_right_distance_cm"
            if (board_conf_obj.contains("left_to_right_distance_cm"))
            {
                board_config.left_to_right_distance_m =
                    board_conf_obj.at("left_to_right_distance_cm").get<float>() / 100.f; // cm -> m
            }

            if (board_conf_obj.contains("left_to_rgb_distance_cm"))
            {
                board_config.left_to_rgb_distance_m =
                    board_conf_obj.at("left_to_rgb_distance_cm").get<float>() / 100.f; // cm -> m
            }

            if (board_conf_obj.contains("name"))
            {
                board_config.name = board_conf_obj.at("name").get<std::string>();
            }

            if (board_conf_obj.contains("revision"))
            {
                board_config.revision = board_conf_obj.at("revision").get<std::string>();
            }
        }

        if (json_obj.contains("camera"))
        {
            auto& camera_conf_obj = json_obj.at("camera");

            if (camera_conf_obj.contains("rgb"))
            {
                auto& rgb_camera_conf_obj = camera_conf_obj.at("rgb");

                rgb_cam_config.resolution_h = rgb_camera_conf_obj.at("resolution_h").get<int32_t>();
                rgb_cam_config.fps = rgb_camera_conf_obj.at("fps").get<float>();

                auto it = rgb_cam_supported_configs.find(rgb_cam_config.resolution_h);

                if (it == rgb_cam_supported_configs.end()) {
                    std::cerr << WARNING "Requested rgb cam config not available!\n" ENDC;

                    std::cerr << "Supported configs.\n";
                    for(auto elem : rgb_cam_supported_configs)
                    {
                        std::cerr << elem.first << "\n";
                    }
                    break;
                }
            }
            // Defaults if the resolution width is not specified
            if (rgb_cam_config.resolution_w == 0) {
                if (rgb_cam_config.resolution_h == 1080)
                    rgb_cam_config.resolution_w =  1920;
                if (rgb_cam_config.resolution_h == 2160)
                    rgb_cam_config.resolution_w =  3840;
            }

            if (camera_conf_obj.contains("mono"))
            {
                auto& mono_camera_conf_obj = camera_conf_obj.at("mono");
                if (mono_camera_conf_obj.contains("resolution_w"))
                    mono_cam_config.resolution_w = mono_camera_conf_obj.at("resolution_w").get<int32_t>();
                mono_cam_config.resolution_h = mono_camera_conf_obj.at("resolution_h").get<int32_t>();
                mono_cam_config.fps = mono_camera_conf_obj.at("fps").get<float>();

                auto it = mono_cam_supported_configs.find(mono_cam_config.resolution_h);

                if (it == mono_cam_supported_configs.end()) {
                    std::cerr << WARNING "Requested mono cam config not available!\n" ENDC;

                    std::cerr << "Supported configs.\n";
                    for(auto elem : mono_cam_supported_configs)
                    {
                        std::cerr << elem.first << "\n";
                    }
                    break;
                }
            }
            // Defaults if the resolution width is not specified
            if (mono_cam_config.resolution_w == 0) {
                if (mono_cam_config.resolution_h == 400)
                    mono_cam_config.resolution_w =  640;
                if (mono_cam_config.resolution_h == 720 || mono_cam_config.resolution_h == 800)
                    mono_cam_config.resolution_w = 1280;
            }
        }

        if (json_obj.contains("app"))
        {
            auto& app_conf_obj = json_obj.at("app");

            if (app_conf_obj.contains("sync_video_meta_streams"))
            {
                app_config.sync_video_meta_streams = app_conf_obj.at("sync_video_meta_streams").get<bool>();
            }
        }


        result = true;
    }
    while (false);

    return result;
}

bool HostPipelineConfig::hasStream(
    const std::string &stream_name
) const
{
    for (const auto &it : streams)
    {
        if (it.name == stream_name)
        {
            return true;
        }
    }

    return false;
}

