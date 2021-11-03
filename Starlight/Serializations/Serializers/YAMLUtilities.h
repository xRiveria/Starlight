#pragma once
#include <yaml-cpp/yaml.h>

namespace Math
{
    struct Vector2
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Vector3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct Vector4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };
}

namespace YAML
{
    inline YAML::Emitter& operator << (YAML::Emitter& output, const Math::Vector2& vector2)
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << vector2.x << vector2.y << YAML::EndSeq;

        return output;
    }

    inline YAML::Emitter& operator << (YAML::Emitter& output, const Math::Vector3& vector3)
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << vector3.x << vector3.y << vector3.z << YAML::EndSeq;

        return output;
    }

    inline YAML::Emitter& operator << (YAML::Emitter& output, const Math::Vector4& vector4)
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << vector4.x << vector4.y << vector4.z << vector4.w << YAML::EndSeq;

        return output;
    }
}