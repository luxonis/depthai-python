#pragma once

#include <string>

#include "depthai-shared/json_helper.hpp"


bool getJSONFromFile(
    const std::string& fpath,
    nlohmann::json &json_obj,
    nlohmann::json* validation_json_obj = nullptr
);
