#include <string>

enum LogType
{
    FATAL = 0,
    ERROR,
    WARNING,
    INFO,
};


class LogsWriter
{

protected:
    LogsWriter         ()                  {}
	LogsWriter         (LogsWriter const&) {}
    virtual ~LogsWriter()                  {}

    static LogsWriter* instance;

public:
    static void        releaseInstance();
    static LogsWriter* getInstanse    ();

    virtual bool init (LogType log_type, bool logs_to_file, bool logs_to_screen, const std::string& file_path) = 0;
    virtual void log  (LogType log_type, const std::string& msg) = 0;
};
