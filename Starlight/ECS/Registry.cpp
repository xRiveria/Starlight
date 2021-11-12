#include "Registry.h"
#include "Entity.h"

namespace ECS
{
    Entity* Registry::CreateEntity()
    {
        std::shared_ptr<Entity> newEntity = std::make_shared<Entity>();
        m_Entities.emplace_back(newEntity);
        return m_Entities.back().get();
    }

    bool Registry::RemoveEntity(uint32_t entityID)
    {
        for (int i = 0; i < m_Entities.size(); i++)
        {
            if (m_Entities[i]->GetEntityID() == entityID)
            {
                m_Entities.erase(m_Entities.begin() + i);
                return true;
            }
        }

        return false;
    }
}