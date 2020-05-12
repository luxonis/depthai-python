#include <iostream>
#include <string>

#include "host_json_helper.hpp"
#include "host_data_reader.hpp"
#include "nlohmann/json-schema.hpp"


bool getJSONFromFile(
    const std::string& fpath,
    nlohmann::json &json_obj,
    nlohmann::json* validation_json_obj
)
{
    bool result = false;

    do
    {
        // read file
        HostDataReader json_reader;
        if (!json_reader.init(fpath))
        {
            std::cout << "Error opening json file: " << fpath.c_str() << "\n";
            break;
        }

        int json_file_size = json_reader.getSize();
        std::string json_str(json_file_size + 1, 0);
        json_reader.readData((unsigned char *) (void *) json_str.data(), json_file_size);

        // parse
        if (!getJSONFromString(json_str, json_obj))
        {
            std::cout << "json parsing error\n";
            break;
        }

        // validate
        if (nullptr != validation_json_obj)
        {
            nlohmann::json_schema::json_validator validator;

            try
            {
                validator.set_root_schema(*validation_json_obj);
            }
            catch (const std::exception &e)
            {
                std::cout << "json validation schema has failed!\n";
                break;
            }

            try
            {
                validator.validate(json_obj);
            }
            catch (const std::exception &e)
            {
                std::cout << "Validation failed" << e.what() << "\n";
                break;
            }
        }

        result = true;
    } while (false);

    return result;
}
