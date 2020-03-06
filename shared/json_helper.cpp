#include <string>

#include "json_helper.hpp"


bool getJSONFromString(const std::string& str, json& json_obj)
{
    json_obj = json::parse(str);
    return true;
}
