#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "tensor_info.hpp"
#include "../host_data_reader.hpp"
#include "../shared/json_helper.hpp"


std::unordered_map<std::string, unsigned> inverseVectorMapping(
    const std::vector<std::string> &mapping
)
{
    std::unordered_map<std::string, unsigned> result;

    for (size_t i = 0; i < mapping.size(); ++i)
    {
        const std::string &str = mapping[i];

        if (result.find(str) == result.end())
        {
            result[str] = i;
        }
        else
        {
            printf("Duplication of value in mapping: %s\n", str.c_str());
        }
    }

    return result;
}


bool parseTensorInfosFromJsonString(const std::string& json_str, std::vector<TensorInfo> &tensor_infos)
{
    bool result = false;
    do
    {
        auto json_obj = json::parse(json_str);

        // trace mappings (optional array)
        std::unordered_map<std::string, std::vector<std::string>> mappings;

        if (json_obj.find("mappings") != json_obj.end())
        {
            auto &mappings_json = json_obj.at("mappings");

            if (!mappings_json.is_object())
            {
                std::cout << "json mappings have to be object\n";
                break;
            }

            bool trace_error = false;
            for (auto &it : mappings_json.items())
            {
                if (!it.value().is_array())
                {
                    std::cout << "mapping obejct have to be ARRAY with strings\n";
                    trace_error = true;
                    break;
                }

                std::vector<std::string> mapping;
                for (auto &it_v : it.value())
                {
                    if (!it_v.is_string())
                    {
                        std::cout << "mapping obejct have to be array with STRINGS\n";
                        trace_error = true;
                        break;
                    }

                    mapping.push_back(it_v.get<std::string>());
                }

                mappings[ it.key() ] = mapping;
            }
            if (trace_error) { break; }
        }

        // trace tensors
        if (json_obj.find("tensors") == json_obj.end() || !json_obj.at("tensors").is_array())
        {
            std::cout << "json should contain tensors array\n";
            break;
        }

        // Try to determine the current stage based on the size of tensor_infos
        int stage = tensor_infos.size() + 1;

        for (auto &tensor_it : json_obj.at("tensors"))
        {
            // TODO: check content format !!!

            TensorInfo tensor_info;
            tensor_info.output_tensor_name = tensor_it.at("output_tensor_name").get<std::string>();
            if (stage > 1) {
                printf("INFO: Suffixing output_tensor_name for stage %d: %s",
                        stage, tensor_info.output_tensor_name.c_str());
                tensor_info.output_tensor_name += "_stage";
                tensor_info.output_tensor_name += '0' + stage;
                printf(" -> %s\n", tensor_info.output_tensor_name.c_str());
            }
            for (auto &v : tensor_it.at("output_dimensions")) { tensor_info.output_dimensions.push_back(v.get<unsigned>()); }
            tensor_info.output_entry_iteration_index = tensor_it.at("output_entry_iteration_index").get<unsigned>();
            for (auto &v : tensor_it.at("output_properties_dimensions")) { tensor_info.output_properties_dimensions.push_back(v.get<unsigned>()); }

            // "property_key_mapping":
            // [
            //     [],
            //     [],
            //     [],
            //     ["id", "label", "confidence", "left", "top", "right", "bottom"]
            // ],

            for (auto &v : tensor_it.at("property_key_mapping"))
            {
                tensor_info.output_property_key_index_to_string.emplace_back();

                // trace: ["id", "label", "confidence", "left", "top", "right", "bottom"]
                for (auto &vv : v)
                {
                    tensor_info.output_property_key_index_to_string.back().push_back(vv.get<std::string>());
                }

                tensor_info.output_property_key_string_to_index.push_back(
                    inverseVectorMapping(tensor_info.output_property_key_index_to_string.back())
                );
            }

            // "property_value_mapping":
            // [
            //     [],
            //     [],
            //     [],
            //     [null, "labels", null, null, null, null, null]
            // ],
            // ...
            // "labels":
            // [
            //     "background",
            //     "aeroplane",
            //     "bicycle",
            //     "bird",
 
            // for (auto &v : tensor_it.at("property_value_mapping"))
            // {
            //     tensor_info.output_property_value_index_to_string.emplace_back();
            //     tensor_info.output_property_value_string_to_index.emplace_back();

            //     for (auto &vv : v)
            //     {
            //         if (vv.is_string())
            //         {
            //             auto mapping_name = vv.get<std::string>();
            //             // TODO: check existence of mapping_name in mappings
            //             tensor_info.output_property_value_index_to_string.back().push_back( mappings[mapping_name] );
            //             tensor_info.output_property_value_string_to_index.back().push_back(
            //                 inverseVectorMapping(mappings[mapping_name])
            //             );
            //         }
            //         else
            //         {
            //             tensor_info.output_property_value_index_to_string.back().emplace_back();
            //             tensor_info.output_property_value_string_to_index.back().emplace_back();
            //         }
            //     }
            // }

            tensor_info.output_properties_type = string_to_type(tensor_it.at("output_properties_type").get<std::string>());            

            tensor_infos.push_back(tensor_info);
        }

        result = true;
    }
    while (false);

    return result;
}

bool parseTensorInfosFromJsonFile(const std::string& json_fpath, std::vector<TensorInfo> &tensor_infos)
{
    bool result = false;
    do
    {
        HostDataReader json_reader;
        if (!json_reader.init(json_fpath))
        {
            std::cout << "Error opening json file: " << json_fpath.c_str() << "\n";
            break;
        }

        int json_file_size = json_reader.getSize();
        std::string json_str(json_file_size + 1, 0);
        json_reader.readData((unsigned char *) (void *) json_str.data(), json_file_size);

        if (!parseTensorInfosFromJsonString(json_str, tensor_infos))
        {
            std::cout << "Error parsing json file: " << json_fpath.c_str() << "\n";
            break;
        }

        result = true;
    }
    while (false);

    return result;
}
