#pragma once
#include <string>
#include <iostream>
#include <map>

enum class Serialization_Target
{
    Serialization_Target_Scene,
    Serialization_Target_Settings,
    Serialization_Target_Assets,
    Serialization_Target_Unknown
};

enum class Serialization_CoreField
{
    Serialization_Field_Type,
    Serialization_Field_VersionMain,
    Serialization_Field_VersionPatch,
    Serialization_Field_Unknown
};

#define ValidateTypes(T) typename std::enable_if<std::is_same<T, bool>::value                        || \
                                                 std::is_same<T, int>::value                         || \
                                                 std::is_same<T, long>::value                        || \
                                                 std::is_same<T, long long>::value                   || \
                                                 std::is_same<T, unsigned long>::value               || \
                                                 std::is_same<T, unsigned long long>::value          || \
                                                 std::is_same<T, uint8_t>::value                     || \
                                                 std::is_same<T, uint16_t>::value                    || \
                                                 std::is_same<T, uint32_t>::value                    || \
                                                 std::is_same<T, uint64_t>::value                    || \
                                                 std::is_same<T, double>::value                      || \
                                                 std::is_same<T, long double>::value                 || \
                                                 std::is_same<T, float>::value>::type       
                                

class SerializerCore
{
public:
    SerializerCore() = delete;
    SerializerCore(Serialization_Target targetSerializable, const std::string& filePath) : m_TargetSerializable(targetSerializable), m_OperatingFilePath(filePath) 
    {
        m_CoreFields[Serialization_CoreField::Serialization_Field_Type] = "Target_Type";
        m_CoreFields[Serialization_CoreField::Serialization_Field_VersionMain] = "Version_Main";
        m_CoreFields[Serialization_CoreField::Serialization_Field_VersionPatch] = "Version_Patch";
    }

    virtual void BeginSerialization() = 0;
    virtual void EndSerialization() = 0;

    virtual void BeginDeserialization() = 0;
    virtual void EndDeserialization() = 0;

    template <typename T, typename = ValidateTypes(T)>
    void Write(const std::string& keyName, T& value)
    {
        if (_WriteInternal(keyName, value))
        {
            std::cout << "Successfully Serialized: " << keyName << " (Value: " << value << ")\n";
        }
    }

    template <typename T, typename = ValidateTypes(T)>
    void Read(const std::string& keyName, T* value)
    {
        if (_ReadInternal(keyName, value))
        {
            std::cout << "Successfully Deserialized: " << keyName << "\n";
        }
    }

protected:
    std::string SerializationTargetToString()
    {
        switch (m_TargetSerializable)
        {
        case Serialization_Target::Serialization_Target_Scene:
            return "Scene";

        case Serialization_Target::Serialization_Target_Settings:
            return "Settings";

        case Serialization_Target::Serialization_Target_Assets:
            return "Assets";

        case Serialization_Target::Serialization_Target_Unknown:
            std::cout << "Incorrect Serializer Target Type!\n";
            return "Target Error";
        }

        return "Target Error";
    }

private: 
    virtual bool _WriteInternal(const std::string& keyName, float value) = 0;
    virtual bool _WriteInternal(const std::string& keyName, bool value) = 0;
    virtual bool _WriteInternal(const std::string& keyName, uint32_t value) = 0;
    virtual bool _WriteInternal(const std::string& keyName, int value) = 0;

    // =====

    virtual bool _ReadInternal(const std::string& keyName, float* value) = 0;
    virtual bool _ReadInternal(const std::string& keyName, bool* value) = 0;
    virtual bool _ReadInternal(const std::string& keyName, uint32_t* value) = 0;
    virtual bool _ReadInternal(const std::string& keyName, int* value) = 0;

protected:
    bool m_IsVersionVerified = false;
    std::string m_OperatingFilePath = "";       // The file we're operating on, whether it be a file to serialize to or deserialize from.
    Serialization_Target m_TargetSerializable = Serialization_Target::Serialization_Target_Unknown;

    unsigned int m_VersionCount_Main = 1;       // Backwards compatability breaking changes.
    unsigned int m_VersionCount_Patch = 0;      // Minor changes, backwards compatability preserved.
    std::map<Serialization_CoreField, std::string> m_CoreFields;
};