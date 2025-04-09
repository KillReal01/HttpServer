#include "utils/LoggerManager.h"

#include <chrono>
#include <iomanip>
#include <sstream>

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


std::string LoggerManager::LogFilename = "logs/log.txt";
bool LoggerManager::Initialized = false;
std::mutex LoggerManager::LogFilenameGuard;


void LoggerManager::SetupLogger()
{
    std::lock_guard<std::mutex> lock(LogFilenameGuard);

    if (Initialized)
        return;

    LogFilename = CreateLogFilename();

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug); 

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LogFilename, true);
    file_sink->set_level(spdlog::level::trace);    

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end());

    logger->set_level(spdlog::level::debug);  

    spdlog::set_default_logger(logger);
    logger->flush_on(spdlog::level::debug);
    
    Initialized = true;
}


const std::string& LoggerManager::GetLogFilename()
{
    if (!Initialized)
        SetupLogger();
    return LogFilename;
}


std::string LoggerManager::CreateLogFilename()
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::tm tm = *std::localtime(&now_time_t);
    std::stringstream ss;
    
    ss << "logs/" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << "_log.txt";
    return ss.str();
}