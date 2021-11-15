#pragma once
#include "Logger.h"
#include "LogFormatter.h"

#include <fmt/core.h>
#include <fmt/format.h>

namespace Aurora
{
#ifdef AURORA_DEBUG

#define AURORA_TEST(...) Aurora::LogError(__VA_ARGS__);

// #define AURORA_INFO(logLayer, logMessage)                 Aurora::Logger::GetInstance().LogMessageFinal(logLayer, logMessage, __FUNCTION__);
#define AURORA_ERROR(logLayer, debugCode, logMessage)     Aurora::Logger::GetInstance().LogMessageError(logLayer, debugCode, logMessage, Aurora::LogMetadata(__FILE__, __FUNCSIG__, __LINE__));
// #define AURORA_WARNING(logLayer, debugCode, logMessage)   Aurora::Logger::GetInstance().LogMessageWarning(logLayer, debugCode, logMessage);
#define AURORA_TRACE(traceInfo) std::cout << "[TRACE] " #traceInfo " = [" << traceInfo << "]" << std::endl;

#else

#define AURORA_INFO(logLayer, logMessage)
#define AURORA_ERROR(logLayer, debugCode, logMessage)
#define AURORA_WARNING(logLayer, debugCode, logMessage)

#endif

    /*
    template <typename FormatString, typename ...Args>
    void Log_(LogMetadata logMetadata, LogLevel logLevel, const FormatString& fmt, const Args& ...args)
    {
        fmt::basic_memory_buffer<char, 250> buffer;
        fmt::format_to(buffer, fmt, args...);
        std::cout << fmt::string_view::basic_string_view(buffer.data(), buffer.size()).data();
    }

    template <typename ...Args>
    void Log(LogMetadata logMetadata, LogLevel logLevel, fmt::basic_string_view<char> fmt, const Args& ...args)
    {
        Log_(logMetadata, logLevel, fmt, args...);
    }

    // T cannot be statistically converted to string_view or wstring_view.
    template <typename T>
    void Log(LogMetadata logMetadata, LogLevel logLevel, const T& message)
    {
        Log(logMetadata, logLevel, "{}", message);
    }

    template <typename T>
    inline void Log(LogLevel logLevel, const T& message)
    {
        Log(LogMetadata(), logLevel, message);
    }

    template <typename FormatString, typename ...Args>
    inline void Log(LogLevel logLevel, const FormatString& formatString, const Args& ...args)
    {
        Log(LogMetadata(), logLevel, formatString, args...);
    }

    template <typename T>
    inline void LogError(const T& message)
    {
        Log(LogLevel::Error, message);
    }

    template <typename FormatString, typename ...Args>
    inline void LogError(const FormatString& formatString, const Args& ...args)
    {
        Log(LogLevel::Error, formatString, args...);
    }
    */
}