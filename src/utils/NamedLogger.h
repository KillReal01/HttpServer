#ifndef HAMEDLOGGER_H
#define HAMEDLOGGER_H

#include <string>
#include <utility>
#include <spdlog/spdlog.h>


class NamedLogger
{
public:

    explicit NamedLogger(const std::string& prefix)
        : _prefix("[" + prefix + "] ")
    {}

    template <typename... Args>
    void Trace(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::trace("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Debug(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::debug("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Info(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::info("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Warning(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::warn("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Error(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::error("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Critical(fmt::format_string<Args...> fmt, Args&&... args) const
    {
        spdlog::critical("{}{}", _prefix, fmt::format(fmt, std::forward<Args>(args)...));
    }

private:
    std::string _prefix;
};

#endif // HAMEDLOGGER_H
