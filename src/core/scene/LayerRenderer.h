#pragma once

#include "../memory/memory.h"
#include "Layer.h"

namespace lab
{
    class LayerRenderer : public Entity
    {
    private:
        Ref<Layer> m_Current;
    public:
        LayerRenderer()
        {
        }
        ~LayerRenderer()
        {
        }

        void Render(const Ref<Layer>& scene);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
