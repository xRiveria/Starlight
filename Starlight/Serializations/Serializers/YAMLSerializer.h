#pragma once
#include "../Core/SerializerCore.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

class YAMLSerializer : public SerializerCore
{
public:
    YAMLSerializer() = delete;
    YAMLSerializer(Serialization_Target targetSerializable, const std::string& filePath) : SerializerCore(targetSerializable, filePath) { }

    void BeginSerialization() override
    {
        m_StreamEmitter << YAML::BeginMap;
        
        // Version Control
        m_StreamEmitter << YAML::Key << m_CoreFields[Serialization_CoreField::Serialization_Field_VersionMain] << YAML::Value << m_VersionCount_Main;
        m_StreamEmitter << YAML::Key << m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch] << YAML::Value << m_VersionCount_Patch;

        // Type
        m_StreamEmitter << YAML::Key << m_CoreFields[Serialization_CoreField::Serialization_Field_Type] << YAML::Value << SerializationTargetToString();
    }

    void EndSerialization() override
    {
        m_StreamEmitter << YAML::EndMap;
        m_StreamEmitter << "This file is created for usage with Starlight. All rights reserved. GitHub Link : https://github.com/xRiveria/Starlight";

        std::ofstream outputFile(m_OperatingFilePath);
        outputFile << m_StreamEmitter.c_str();

        std::cout << "Serialization Complete. Outputted to: " << m_OperatingFilePath << "\n";
    }

    void BeginDeserialization() override
    {
        m_StreamNode = YAML::LoadFile(m_OperatingFilePath);

        // Ensure that our version matches the serialized file.
        if (m_StreamNode[m_CoreFields[Serialization_CoreField::Serialization_Field_VersionMain]].as<unsigned int>() != m_VersionCount_Main) // Loaded main versions must be equal.
        {
            std::cout << "Invalid Main Version. The loaded file is not compatible with the current engine.\n";
            return;
        }

        if (m_StreamNode[m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch]].as<unsigned int>() > m_VersionCount_Patch) // Loaded patch versions must be less than or equal to the current version.
        {
            std::cout << "Invalid Patch Version. The loaded file is not compatible with the current engine.\n";
            return;
        }

        m_IsVersionVerified = true;
    }

    void EndDeserialization() override
    {
        if (m_IsVersionVerified)
        {
            std::cout << "Successfully Deserialized File: " << m_OperatingFilePath << "\n";
        }
    }

private:
    template <typename T>
    bool WriteBasicType(const std::string& keyName, T value)
    {
        m_StreamEmitter << YAML::Key << keyName << YAML::Value << value;

        return true;
    }

    template <typename T>
    bool ReadBasicType(const std::string& keyName, T* value)
    {
        if (m_IsVersionVerified)
        {
            *value = m_StreamNode[keyName].as<T>();
            return true;
        }

        return false;
    }

private:
    bool _WriteInternal(const std::string& keyName, float value) override
    {
        return WriteBasicType(keyName, value);
    }

    bool _WriteInternal(const std::string& keyName, bool value) override
    {
        return WriteBasicType(keyName, value);
    }

    bool _WriteInternal(const std::string& keyName, uint32_t value) override
    {
        return WriteBasicType(keyName, value);
    }

    bool _WriteInternal(const std::string& keyName, int value) override
    {
        return WriteBasicType(keyName, value);
    }

    bool _ReadInternal(const std::string& keyName, float* value) override
    {
        return ReadBasicType(keyName, value);
    }

    bool _ReadInternal(const std::string& keyName, bool* value) override
    {
        return ReadBasicType(keyName, value);
    }

    bool _ReadInternal(const std::string& keyName, uint32_t* value) override
    {
        return ReadBasicType(keyName, value);
    }

    bool _ReadInternal(const std::string& keyName, int* value) override
    {
        return ReadBasicType(keyName, value);
    }

private:
    YAML::Emitter m_StreamEmitter;
    YAML::Node m_StreamNode;
};