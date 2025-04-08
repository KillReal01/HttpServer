#ifndef HELPER_H
#define HELPER_H

#include <string>

class Helper
{
public: 
    static void SetupLogger();
    static std::string GetLogFilename();

private:
    static std::string CreateLogFilename();

private:
    static std::string _logFilename;

};

#endif // HELPER_H