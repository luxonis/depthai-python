#include <assert.h>

#include <iostream>
#include <array>


#include "../../host/core/3rdparty/loguru/loguru.hpp"
#include <host_logs_writer.hpp>

HostLogsWriter* HostLogsWriter::createInstanse()
{
	if(instance) { std::cout << "HostLogsWriter instannce has already been created" << std::endl; }
	else { instance = new HostLogsWriter; };
}

bool HostLogsWriter::init
(
    LogType log_type,
    bool logs_to_file,
    bool logs_to_screen,
    const std::string& file_path
)
{
    loguru::g_preamble_uptime  = false;
	loguru::g_preamble_thread = false;
	loguru::g_preamble_file = false;


    std::array<loguru::Verbosity, 4> loguru_log_level =
    {
        loguru::Verbosity_FATAL,
        loguru::Verbosity_ERROR,
        loguru::Verbosity_WARNING,
        loguru::Verbosity_INFO
    };

    assert(log_type < loguru_log_level.size());
    loguru::g_stderr_verbosity = loguru_log_level[log_type];

    if (logs_to_file) { loguru::add_file(file_path.c_str(), loguru::Append, loguru_log_level[log_type]); };
    
}

void HostLogsWriter::log
(
    LogType log_type,
    const std::string& msg
)
{
    if (log_type == 0)      { LOG_F(FATAL, msg.c_str()); } 
    else if (log_type == 1) { LOG_F(ERROR, msg.c_str()); }
    else if (log_type == 2) { LOG_F(WARNING, msg.c_str()); }
    else if (log_type == 3) { LOG_F(INFO, msg.c_str()); }
}