#include <string.h>

#include <iostream>

#include "program_raw_options.hpp"


ProgramRawOptions readProgramRawOptions(
    int argc,
    char** argv
)
{
    ProgramRawOptions options;

    // trace argv
    for (int i = 0; i < argc; ++i)
    {
        auto* eq_pointer = strchr(argv[i], '=');
        if (eq_pointer == NULL)
        {
            options.options.insert(argv[i]);
        }
        else
        {
            std::string str(argv[i]);
            int eq_pos = eq_pointer - argv[i];
            std::string key = str.substr(0, eq_pos);
            std::string value = str.substr(eq_pos+1);

            options.key_values[key] = value;
        }
    }

    return options;
}


bool ProgramRawOptions::hasOption(
    const char* search_option
) const
{
    return options.find(search_option) != options.end();
}

const char* ProgramRawOptions::getValueOrDefault(
    const char* key,
    const char* default_value
) const
{
    if (key_values.find(key) == key_values.end())
    {
        return default_value;
    }
    else
    {
        return key_values.at(key).c_str();
    }
}
