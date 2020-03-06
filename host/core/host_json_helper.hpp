#pragma once

#include <string>

#include "../../shared/json_helper.hpp"


bool getJSONFromFile(
    const std::string& fpath,
    json &json_obj,
    json* validation_json_obj = nullptr
);
