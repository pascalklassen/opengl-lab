#pragma once

#include <gl/glew.h>

#include "../memory/_includes.h"
#include "../platform/_includes.h"
#include "Scene.h"

namespace lab
{
    class SceneRenderer : public Entity
    {
    private:
        Ref<Scene> m_Current;
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<ShaderProgram> m_Shader;

        GLuint m_FBO;
        GLuint m_RBO;
        GLuint m_TEX;
    public:
        SceneRenderer();
        ~SceneRenderer();

        void Select(const Ref<Scene>& scene);

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnGui() override;
    };
}
