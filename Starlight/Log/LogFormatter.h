#pragma once
#include <string>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>

namespace Aurora
{
    inline std::string LogFormatter_WithTimestamps(const LogPayload& logPackage)
    {
        return fmt::format("[{0}][{1}] {2} (Line {3}, {4}) \n", LogLevelToString(logPackage.m_LogLevel), logPackage.m_LogMetadata.m_LogTime, logPackage.m_LogMessage, logPackage.m_LogMetadata.m_LineNumber, logPackage.m_LogMetadata.m_FilePath);
    }

    inline std::string LogFormatter_Complete(const LogPayload& logPackage)
    {
        return fmt::format("[{0}] {2} (Line {3}, {4}) \n", LogLevelToString(logPackage.m_LogLevel), logPackage.m_LogMessage, logPackage.m_LogMetadata.m_LineNumber, logPackage.m_LogMetadata.m_FilePath);
    }
}