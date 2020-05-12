#include <iostream>

#include "device_support_listener.hpp"
#include "depthai-shared/json_helper.hpp"


void DeviceSupportListener::onNewData(
    const StreamInfo &data_info,
    const StreamData &data
)
{
    do
    {
        const char* str = (const char *) data.data;

        nlohmann::json meta_json;
        if (!getJSONFromString(str, meta_json))
        {
            std::cout << "DeviceSupportListener: json parsing error;\n";
            break;
        }

        // print logs
        if (meta_json.contains("logs"))
        {
            assert(meta_json.at("logs").is_array());

            for (const auto &it : meta_json.at("logs"))
            {
                assert(it.is_string());
                std::cout << "DEVICE LOG: " << it.get<std::string>() << "\n";
            }
        }
    }
    while (false);    
}
