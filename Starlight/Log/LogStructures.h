#pragma once
#include "LogClassification.h"
#include "../Debug/DebugCode.h"
#include <string>
#include <time.h>

namespace Aurora
{
    // Contains metadata information such as line number, function name and executing file.
    struct LogMetadata
    {
        LogMetadata() = default;
        LogMetadata(const char* filePath, const std::string& functionName, int lineNumber) : m_FunctionName(functionName), m_FilePath(filePath), m_LineNumber(lineNumber)
        {
            // Capture time as a string.
            time_t timeNow = time(nullptr);
            struct std::tm timeStruct;
            char buffer[80];
            timeStruct = *localtime(&timeNow);
            strftime(buffer, sizeof(buffer), "%X", &timeStruct);

            m_LogTime = std::string(buffer);
        }

        int m_LineNumber = 0;
        const char* m_FilePath = "";
        std::string m_FunctionName = "";
        std::string m_LogTime = "";
    };

    struct LogPayload
    {
        LogLayer m_LogLayer = LogLayer::Unknown;
        LogLevel m_LogLevel = LogLevel::Unknown;
        DebugCode m_DebugCode = DebugCode::AURORA_MAX_ENUM;
        const char* m_LogMessage = "";

        LogMetadata m_LogMetadata;
    };
}