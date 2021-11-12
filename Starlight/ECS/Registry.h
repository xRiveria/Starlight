#pragma once
#include <vector>
#include <memory>

/*
    - A registry exists for each Scene/World. 
*/

namespace ECS
{
    class Entity;

    class Registry final
    {
    public:
        static Registry& GetInstance()
        {
            static Registry registryInstance;
            return registryInstance;
        }

        Entity* CreateEntity();
        bool RemoveEntity(uint32_t entityID);

    private:
        std::vector<std::shared_ptr<Entity>> m_Entities;
    };
}