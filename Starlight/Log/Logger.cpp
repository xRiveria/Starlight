#include "Logger.h"
#include "LogFormatter.h"
#include <iostream>

namespace Aurora
{
    void Logger::EnableTimestamps()
    {
        if (!m_TimestampsEnabled)
        {
            m_TimestampsEnabled = true;
        }
    }

    void Logger::EnableBackbuffer()
    {
        if (!m_BackbufferEnabled)
        {
            m_BackbufferEnabled = true;
        }
    }

    void Logger::LogMessageError(LogLayer logLayer, DebugCode debugCode, const char* logMessage, const LogMetadata& metaData)
    {
        LogPayload logPayload;

        logPayload.m_LogLevel = LogLevel::Error;
        logPayload.m_LogLayer = logLayer;
        logPayload.m_DebugCode = debugCode;
        logPayload.m_LogMessage = logMessage;
        logPayload.m_LogMetadata = metaData;

        LogMessageFinal(logPayload);
    }

    // Everything that arrives here are to have completed formatting/processing etc.
    void Logger::LogMessageFinal(const LogPayload& logPackage)
    {
        // Save to backtrace buffer at this stage and log appropriate message based on formatting type.
        if (m_BackbufferEnabled)
        {
            m_LogPayloadBuffer.push_back(logPackage);
        }

        if (m_TimestampsEnabled)
        {
            std::cout << LogFormatter_WithTimestamps(logPackage);
            return;
        }
        else
        {
            std::cout << LogFormatter_Complete(logPackage);
            return;
        }
    }
}