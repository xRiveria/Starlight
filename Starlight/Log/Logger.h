#pragma once
#include "LogClassification.h"
#include "../Debug/DebugCode.h"

// Set loggers that systems project wide use for logging.

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




        void LogMessageWarning(LogLayer logLayer, DebugCode debugCode, const char* logMessage);
        void LogMessageError(LogLayer logLayer, DebugCode debugCode, const char* logMessage);

        // Everything resolves to this.
        void LogMessageFinal(LogLayer logLayer, const char* logMessage);
    };
}