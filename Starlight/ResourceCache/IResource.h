#pragma once
#include <string>
#include <atomic>
#include "StarlightObject.h"

namespace Resources
{
    enum class ResourceType
    {
        ResourceType_Model,
        ResourceType_Texture,
        ResourceType_Animation,
        ResourceType_Font,
        ResourceType_Shader,
        ResourceType_Unknown
    };

    enum class LoadState
    {
        LoadState_Idle,
        LoadState_Started,
        LoadState_Completed,
        LoadState_Failed
    };

    class IResource : public StarlightObject
    {
    public:
        IResource(ResourceType resourceType);
        

        ResourceType GetResourceType() const { return m_ResourceType; }

        // Type
        template <typename T>
        static constexpr ResourceType TypeToEnum();

    protected:
        ResourceType m_ResourceType = ResourceType::ResourceType_Unknown;
        std::atomic<LoadState> m_LoadState = LoadState::LoadState_Idle;

    private:
        std::string m_ResourceName = "Empty_Resource_Name";
        std::string m_ResourceDirectory = "Empty_Directory";
        std::string m_ResourceFilePathNative = "Empty_Path_Native";
        std::string m_ResourceFilePathForeign = "Empty_Path_Foreign";
    };

    // Examples
    class Model : public IResource
    {

    };

    class Animation : public IResource
    {

    };

    class Texture : public IResource
    {

    };
}