#pragma once
#include "LogClassification.h"
#include "LogStructures.h"
#include "../Debug/DebugCode.h"
#include <string>
#include <vector>

namespace Aurora
{
    class Logger
    {
    public:
        static Logger& GetInstance()
        {
            static Logger loggerInstance;
            return loggerInstance;
        }

        void EnableTimestamps();
        void EnableBackbuffer();


        // Core
        // void LogMessageWarning(LogLayer logLayer, DebugCode debugCode, const char* logMessage, const LogMetadata& metaData);
        void LogMessageError(LogLayer logLayer, DebugCode debugCode, const char* logMessage, const LogMetadata& metaData);

        // Everything resolves to this.
        void LogMessageFinal(const LogPayload& logPackage);

    private:
        bool m_TimestampsEnabled = true;
        bool m_BackbufferEnabled = true;

        // Buffer to store messages for future retrieval. To implement custom ringbuffer in the future. 
        std::vector<LogPayload> m_LogPayloadBuffer;
    };
}