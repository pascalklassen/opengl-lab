#include "SceneRenderer.h"

namespace lab
{
    void SceneRenderer::Render(const Ref<Scene>& scene)
    {
        m_Current = scene;
    }

    void SceneRenderer::OnUpdate(float deltaTime)
    {
        m_Current->OnUpdate(deltaTime);
    }

    void SceneRenderer::OnRender()
    {
        m_Current->OnRender();
    }

    void SceneRenderer::OnGui()
    {
        m_Current->OnGui();
    }
}
