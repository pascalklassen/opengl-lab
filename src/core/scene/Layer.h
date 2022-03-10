#pragma once

#include <vector>

#include "../memory/memory.h"
#include "Entity.h"

namespace lab
{
    class Layer : public Entity
    {
    private:
        std::vector<Ref<Entity>> m_Entities;
    public:
        Layer();
        ~Layer()
        {
        }

        void Add(const Ref<Entity>& entity);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
