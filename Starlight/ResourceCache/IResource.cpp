#include "IResource.h"

namespace Resources
{
    template <typename T>
    constexpr ResourceType IResource::TypeToEnum()
    {
        return ResourceType::ResourceType_Unknown;
    }

    template <typename T>
    constexpr void ValidateResourceType()
    {
        static_assert(std::is_base_of<IResource, T>::value, "Provided value does not implement IResource.");
    }

    // Explicit template instantiation.
    #define INSTANTIATE_RESOURCE_TYPE(T, enumT) \
    template<>                                       \
    ResourceType IResource::TypeToEnum<T>()          \
    {                                                \
        ValidateResourceType<T>();                   \
        return enumT;                                \
    }         

    // To add a new resource to the engine, simply register it here.
    INSTANTIATE_RESOURCE_TYPE(Model, ResourceType::ResourceType_Model)
    INSTANTIATE_RESOURCE_TYPE(Animation, ResourceType::ResourceType_Animation)
    INSTANTIATE_RESOURCE_TYPE(Texture, ResourceType::ResourceType_Texture)
}