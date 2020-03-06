#pragma once

#include <fstream>
#include <string>


class HostDataReader
{
private:
    std::fstream _file;

public:
    bool     init     (const std::string& file_path);
    unsigned getSize  ();
    unsigned readData (unsigned char* buffer, unsigned read_size);
};
