#include "LayerRenderer.h"

#include "../memory/memory.h"
#include "Layer.h"

namespace lab
{
    void LayerRenderer::Render(const Ref<Layer>& scene)
    {
        m_Current = scene;
    }

    void LayerRenderer::OnUpdate(float deltaTime)
    {
        m_Current->OnUpdate(deltaTime);
    }

    void LayerRenderer::OnRender()
    {
        m_Current->OnRender();
    }

    void LayerRenderer::OnGui()
    {
        m_Current->OnGui();
    }
}
