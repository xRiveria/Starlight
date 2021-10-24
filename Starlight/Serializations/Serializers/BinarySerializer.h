#pragma once
#include "../Core/SerializerCore.h"

class BinarySerializer : public SerializerCore
{
public:
    BinarySerializer() = delete;
    BinarySerializer(Serialization_Target targetSerializable, const std::string& filePath) : SerializerCore(targetSerializable, filePath) { }

    void BeginSerialization() override
    {
        m_StreamFlags |= std::ios::out;

        m_OutputStream.open(m_OperatingFilePath, m_StreamFlags);
        if (m_OutputStream.fail())
        {
            std::cout << "Failed to open " << m_OperatingFilePath << " for writing.\n";
            return;
        }

        // Version Control
        WriteBasicType(m_CoreFields[Serialization_CoreField::Serialization_Field_VersionMain], m_VersionCount_Main);
        WriteBasicType(m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch], m_VersionCount_Patch);

        // Type
        WriteBasicType(m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch], SerializationTargetToString());
    }

    void EndSerialization() override
    {
        m_OutputStream.flush();
        m_OutputStream.close();

        std::cout << "Successfully serialized " << m_OperatingFilePath << "\n";
    }

    void BeginDeserialization() override
    {
        m_StreamFlags |= std::ios::out;

        m_InputStream.open(m_OperatingFilePath, m_StreamFlags);
        if (m_InputStream.fail())
        {
            std::cout << "Failed to open " << m_OperatingFilePath << " for reading.\n";
            return;
        }

        // Ensure that our version matches the serialized file.
        if (ReadBasicTypeAs<unsigned int>(m_CoreFields[Serialization_CoreField::Serialization_Field_VersionMain]) != m_VersionCount_Main)
        {
            std::cout << "Invalid Main Version. The loaded file is not compatible with the current engine.\n";
            return;
        }

        if (ReadBasicTypeAs<unsigned int>(m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch]) > m_VersionCount_Patch)
        {
            std::cout << "Invalid Patch Version. The loaded file is not compatible with the current engine.\n";
            return;
        }

        // Type
        ReadBasicTypeAs<std::string>(m_CoreFields[Serialization_CoreField::Serialization_Field_Type]);

        m_IsVersionVerified = true;
    }

    void EndDeserialization() override
    {
        if (m_IsVersionVerified)
        {
            m_InputStream.clear();
            m_InputStream.close();
            std::cout << "Successfully Deserialized File: " << m_OperatingFilePath << "\n";
        }
    }

private:
    template <typename T>
    bool WriteBasicType(const std::string& keyName, T value)
    {
        m_OutputStream.write(reinterpret_cast<char*>(&value), sizeof(value));
        return true;
    }

    template <typename T>
    bool ReadBasicType(const std::string& keyName, T* value)
    {
        m_InputStream.read(reinterpret_cast<char*>(value), sizeof(T));
        return true;
    }

    template <typename T>
    T ReadBasicTypeAs(const std::string& keyName)
    {
        T value;
        ReadBasicType(keyName, &value);
        return value;
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
    int m_StreamFlags = std::ios::binary; // Accesses the file as a binary file - no formatting is done.

    std::ofstream m_OutputStream;
    std::ifstream m_InputStream;
};