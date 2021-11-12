#pragma once
#include "Logger.h"

namespace Aurora
{
#ifdef AURORA_DEBUG

#define AURORA_INFO(logLayer, logMessage)                 Aurora::Logger::GetInstance().LogMessageFinal(logLayer, logMessage);
#define AURORA_ERROR(logLayer, debugCode, logMessage)     Aurora::Logger::GetInstance().LogMessageError(logLayer, debugCode, logMessage);
#define AURORA_WARNING(logLayer, debugCode, logMessage)   Aurora::Logger::GetInstance().LogMessageWarning(logLayer, debugCode, logMessage);

#else

#define AURORA_INFO()
#define AURORA_ERROR()
#define AURORA_WARNING()

#endif
}