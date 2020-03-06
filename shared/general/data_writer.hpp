#include <fstream>
#include <string>


class DataWriter
{

private:
    std::fstream _file;
    
public:
    ~DataWriter();
    bool     init      (const std::string& file_path);
    unsigned writeData (unsigned char* buffer, unsigned buff_size);
};
