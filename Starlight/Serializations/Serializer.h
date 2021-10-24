#pragma once
#include <type_traits>
#include "Core/SerializerCore.h"
#include "Serializers/YAMLSerializer.h"
#include "Serializers/BinarySerializer.h"

namespace Serializer
{
    template <typename T, typename = typename std::enable_if<std::is_base_of<SerializerCore, T>::value, T>::type>
    T GetSerializer(Serialization_Target targetSerializable, const std::string& filePath)
    {
        return T(targetSerializable, filePath);
    }

    void Serialize()
    {
        float m_Damage = 50.5f;
        int m_Health = 500;

        YAMLSerializer serializer = GetSerializer<YAMLSerializer>(Serialization_Target::Serialization_Target_Scene, "YAMLSerialTest.starlight");
        serializer.BeginSerialization();
        serializer.Write("Damage", m_Damage);
        serializer.Write("Health", m_Health);
        serializer.EndSerialization();

        BinarySerializer binarySerializer = GetSerializer<BinarySerializer>(Serialization_Target::Serialization_Target_Assets, "BinarySerialTest.starlight");
        binarySerializer.BeginSerialization();
        binarySerializer.Write("Damage", m_Damage);
        binarySerializer.Write("Health", m_Health);
        binarySerializer.EndSerialization();

        float m_DamageReinitialized = 0.0f;
        int m_HealthReinitialized = 5;
        BinarySerializer binaryDeserializer = GetSerializer<BinarySerializer>(Serialization_Target::Serialization_Target_Assets, "BinarySerialTest.starlight");
        binaryDeserializer.BeginDeserialization();
        binaryDeserializer.Read("Damage", &m_DamageReinitialized);
        binaryDeserializer.Read("Health", &m_HealthReinitialized);
        binaryDeserializer.EndDeserialization();
    }
}