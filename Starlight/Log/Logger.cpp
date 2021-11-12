#include "Logger.h"
#include <iostream>

namespace Aurora
{
    void Logger::LogMessageWarning(LogLayer logLayer, DebugCode debugCode, const char* logMessage)
    {

    }

    void Logger::LogMessageError(LogLayer logLayer, DebugCode debugCode, const char* logMessage)
    {

    }

    void Logger::LogMessageFinal(LogLayer logLayer, const char* logMessage)
    {
        std::cout << LogLayerToString(logLayer) << ": " << logMessage;
    }
}