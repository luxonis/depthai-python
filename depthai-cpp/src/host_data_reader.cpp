#include <assert.h>

#include <fstream>

#include "host_data_reader.hpp"


bool HostDataReader::init(
    const std::string& file_path
)
{
    _file.open(file_path, std::fstream::in | std::fstream::binary);
    return _file.is_open();
}

unsigned HostDataReader::getSize()
{
    _file.seekg(0, _file.end);
    unsigned f_size = _file.tellg();
    _file.seekg(0, _file.beg);
    return f_size;
}

unsigned HostDataReader::readData(unsigned char* buffer, unsigned read_size)
{
    assert(nullptr != buffer);

    _file.read(reinterpret_cast<char *>(buffer), read_size);
    return _file.gcount();
}
