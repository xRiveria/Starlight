#pragma once
#include <cstdint>

namespace Aurora
{
    // Each level of logs.
    enum class LogLevel
    {
        Info,
        Warning,
        Error,
        Critical,
        Unknown
    };

    inline const char* LogLevelToString(LogLevel logLevel)
    {
        switch (logLevel)
        {
            case LogLevel::Info:
                return "Info";

            case LogLevel::Warning:
                return "Warning";

            case LogLevel::Error:
                return "Error";

            case LogLevel::Critical:
                return "Critical";

            case LogLevel::Unknown:
                return "Unknown";
        }

        return "Unknown Log Level";
    }

    // Log layers for each kind of system in the engine. While we could use engine enums themselves, our layer implementation allow for more fine grained situations.
    enum class LogLayer
    {
        Initialization,
        Engine,
        Graphics,
        Physics,
        Serialization,
        ECS,
        Audio,
        Scripting,
        Unknown
    };

    inline const char* LogLayerToString(LogLayer logLevel)
    {
        switch (logLevel)
        {
            case LogLayer::Initialization:
                return "Initialization";

            case LogLayer::Engine:
                return "Engine";

            case LogLayer::Graphics:
                return "Graphics";

            case LogLayer::Physics:
                return "Physics";

            case LogLayer::Serialization:
                return "Serialization";

            case LogLayer::ECS:
                return "ECS";

            case LogLayer::Audio:
                return "Audio";

            case LogLayer::Scripting:
                return "Scripting";

            case LogLayer::Unknown:
                return "Unknown";
        }

        return "Unknown Log Layer";
    }
}