#pragma once
#include <memory>

namespace Aurora
{
    enum DebugCode : uint32_t
    {
        AURORA_SUCCESS                      = 0,
        AURORA_TIMEOUT                      = 1,
        AURORA_ERROR_INITIALIZATION_FAILURE = 2,
        AURORA_ERROR_INVALID_PARAMETERS     = 3,
        AURORA_ERROR_RETRIEVAL_FAILURE      = 4,
        AURORA_MAX_ENUM
    };

    inline const char* DebugCodeToString(DebugCode debugCode)
    {
        switch (debugCode)
        {
            case DebugCode::AURORA_SUCCESS:                         return "AURORA_SUCCESS";
            case DebugCode::AURORA_TIMEOUT:                         return "AURORA_TIMEOUT";
            case DebugCode::AURORA_ERROR_INITIALIZATION_FAILURE:    return "AURORA_INITIALIZATION FAILURE";
            case DebugCode::AURORA_ERROR_INVALID_PARAMETERS:        return "AURORA_INVALID_PARAMETERS";
            case DebugCode::AURORA_ERROR_RETRIEVAL_FAILURE:         return "AURORA_RETRIEVAL_FAILURE";
            case DebugCode::AURORA_MAX_ENUM:                        return "AURORA_MAX_ENUM";
        }

        return "Unknown Error Code";
    }
}