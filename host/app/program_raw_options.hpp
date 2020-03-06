#pragma once

#include <map>
#include <set>
#include <string>


struct ProgramRawOptions
{
    std::set<std::string> options;
    std::map<std::string, std::string> key_values;

    bool        hasOption        (const char* search_option) const;
    const char* getValueOrDefault(const char* key, const char* default_value) const;
};


ProgramRawOptions readProgramRawOptions(int argc, char** argv);
