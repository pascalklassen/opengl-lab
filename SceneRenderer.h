#pragma once

#include "core.h"

namespace lab
{
    class SceneRenderer : public Component
    {
    private:
        Ref<Scene> m_Current;
    public:
        SceneRenderer()
        {
        }
        ~SceneRenderer()
        {
        }

        void Render(const Ref<Scene>& scene);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
