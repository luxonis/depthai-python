
#pragma once

#include <string>

#include "3rdparty/json/include/nlohmann/json.hpp"


using json = nlohmann::json;


bool getJSONFromString(const std::string& str, json& json_obj);
