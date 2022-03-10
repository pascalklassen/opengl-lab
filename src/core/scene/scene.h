#pragma once

#include <vector>

#include "../memory/_includes.h"
#include "Entity.h"

namespace lab
{
    class Scene : public Entity
    {
    private:
        std::vector<Ref<Entity>> m_Entities;
    public:
        Scene();
        ~Scene()
        {
        }

        void Add(const Ref<Entity>& entity);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
