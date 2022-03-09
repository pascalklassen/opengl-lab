#include "Scene.h"

#include "core.h"

namespace lab
{
    Scene::Scene()
    {
    }

    void Scene::Add(const Ref<Component>& component)
    {
        m_Components.push_back(component);
    }

    void Scene::OnUpdate(float deltaTime)
    {
        for (const Ref<Component>& comp : m_Components)
            comp->OnUpdate(deltaTime);
    }

    void Scene::OnRender()
    {
        for (const Ref<Component>& comp : m_Components)
            comp->OnRender();
    }

    void Scene::OnGui()
    {
        for (const Ref<Component>& comp : m_Components)
            comp->OnGui();
    }
}
