#pragma once

#include <vector>

#include "core.h"

namespace lab
{
    class Scene : public Component
    {
    private:
        std::vector<Ref<Component>> m_Components;
    public:
        Scene();
        ~Scene()
        {
        }

        void Add(const Ref<Component>& component);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
