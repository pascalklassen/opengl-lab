#include "Layer.h"

#include "../memory/memory.h"
#include "Entity.h"

namespace lab
{
    Layer::Layer()
    {
    }

    void Layer::Add(const Ref<Entity>& entity)
    {
        m_Entities.push_back(entity);
    }

    void Layer::OnUpdate(float deltaTime)
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnUpdate(deltaTime);
    }

    void Layer::OnRender()
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnRender();
    }

    void Layer::OnGui()
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnGui();
    }
}
