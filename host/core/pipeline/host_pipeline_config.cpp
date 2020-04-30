#include <iostream>

#include "host_pipeline_config.hpp"


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

        // if 'meta_d2h' not specified - add it
        // if (!hasStream("meta_d2h"))
        // {
        //     streams.emplace_back("meta_d2h");
        // }

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

                if (depth.padding_factor < 0.f || depth.padding_factor >= 0.5)
                {
                    std::cout << "padding_factor should be in the range [0 .. 0.5)\n";
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

            if (ai_obj.contains("calc_dist_to_bb"))
            {
                ai.calc_dist_to_bb = ai_obj.at("calc_dist_to_bb").get<bool>();
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

