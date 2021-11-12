#pragma once
#include "DebugCode.h"
#include "../Log/LogMacros.h"

namespace Aurora
{
    inline bool CheckResult(LogLayer logLayer, DebugCode debugCode)
    {
        if (debugCode == DebugCode::AURORA_SUCCESS)
        {
            return true;
        }

        AURORA_ERROR(logLayer, debugCode, DebugCodeToString(debugCode));
        return false;
    }
}
