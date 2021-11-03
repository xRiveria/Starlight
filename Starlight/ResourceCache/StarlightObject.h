#pragma once
#include <string>

namespace Resources
{
    class StarlightObject
    {
    public:
        StarlightObject();

        // Name
        const std::string& GetObjectName() const { return m_ObjectName; }

        // ID
        const uint64_t GetObjectUUID() const { return m_ObjectUUID; }
        void SetObjectUUID(const uint64_t newUUID) { m_ObjectUUID = newUUID; }
        static uint64_t GenerateObjectID() {  }

    protected:
        std::string m_ObjectName = "Object";
        uint64_t m_ObjectUUID = 0;
    };
}