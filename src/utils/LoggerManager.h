#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <string>
#include <mutex>

class LoggerManager
{
public: 
    static void SetupLogger();
    static const std::string& GetLogFilename();

private:
    static std::string CreateLogFilename();

private:
    static std::string LogFilename;
    static bool Initialized;
    static std::mutex LogFilenameGuard;
};

#endif // LOGGERMANAGER_H