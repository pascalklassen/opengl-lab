#include "Scene.h"

#include "../memory/_includes.h"
#include "Entity.h"

namespace lab
{
    Scene::Scene()
    {
    }

    void Scene::Add(const Ref<Entity>& entity)
    {
        m_Entities.push_back(entity);
    }

    void Scene::OnUpdate(float deltaTime)
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnUpdate(deltaTime);
    }

    void Scene::OnRender()
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnRender();
    }

    void Scene::OnGui()
    {
        for (const Ref<Entity>& entity : m_Entities)
            entity->OnGui();
    }
}
