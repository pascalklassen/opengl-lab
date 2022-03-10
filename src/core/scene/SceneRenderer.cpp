#include "SceneRenderer.h"

#include <gl/glew.h>
#include <imgui.h>

#include "../debug/_includes.h"
#include "../memory/_includes.h"
#include "../platform/_includes.h"
#include "Scene.h"

namespace lab
{

    SceneRenderer::SceneRenderer()
        : m_FBO{ 0 }, m_RBO{ 0 }, m_TEX{ 0 }
    {
        GL_CHECK(glGenFramebuffers(1, &m_FBO));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

        GL_CHECK(glGenTextures(1, &m_TEX));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TEX));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TEX, 0));

        GL_CHECK(glGenRenderbuffers(1, &m_RBO));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));

        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO));
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        GLfloat vertices[] =
        {
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        m_VertexArray = MakeRef<VertexArray>();
        m_VertexArray->Bind();

        m_VertexBuffer = MakeRef<VertexBuffer>(sizeof(vertices), vertices);
        m_VertexBuffer->Bind();

        lab::VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        layout.Push<GLfloat>(2);

        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        lab::VertexShader vs{ "resources/shaders/FrameBuffer_Vertex.glsl" };
        lab::FragmentShader fs{ "resources/shaders/FrameBuffer_Fragment.glsl" };
        m_Shader = MakeRef<ShaderProgram>();
        m_Shader->AttachShader(vs);
        m_Shader->AttachShader(fs);
        m_Shader->Link();
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_FrameBuffer", 0);
    }

    SceneRenderer::~SceneRenderer()
    {
        GL_CHECK(glDeleteTextures(1, &m_TEX));
        GL_CHECK(glDeleteRenderbuffers(1, &m_RBO));
        GL_CHECK(glDeleteFramebuffers(1, &m_FBO));
    }

    void SceneRenderer::Select(const Ref<Scene>& layer)
    {
        m_Current = layer;
    }

    void SceneRenderer::OnUpdate(float deltaTime)
    {
        m_Current->OnUpdate(deltaTime);
    }

    void SceneRenderer::OnRender()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

        m_Current->OnRender();

        /* FrameBuffer End */
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        m_VertexArray->Bind();
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        m_Shader->Bind();
        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TEX));
    }

    void SceneRenderer::OnGui()
    {
        m_Current->OnGui();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene");
        ImGui::BeginChild("Renderer");
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)m_TEX, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
