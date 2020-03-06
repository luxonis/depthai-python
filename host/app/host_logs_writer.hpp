#include <iostream>

#include <../../shared/logger/logs_writer.hpp>

class HostLogsWriter
    : public LogsWriter
{
protected:
    HostLogsWriter         () {}
    virtual ~HostLogsWriter() {}
public:    
    bool                   init          (LogType log_type, bool logs_to_file, bool logs_to_screen, const std::string& file_path) override;
    void                   log           (LogType log_type, const std::string& msg) override; 
    static HostLogsWriter* createInstanse();
};
