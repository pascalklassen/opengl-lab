#include "TestScene.h"

#include "../core/core.h"

namespace lab
{
    TestScene::TestScene()
        : Scene(),  m_Model{ 1.0f }, m_View{ 1.0f }, m_Proj{ 1.0f }
    {
        m_VertexArray = MakeRef<VertexArray>();
        m_VertexArray->Bind();

        m_VertexBuffer = MakeRef<VertexBuffer>(sizeof(vertices), vertices);
        m_VertexBuffer->Bind();

        lab::VertexBufferLayout layout;
        /* Position */
        layout.Push<GLfloat>(3);
        /* Texture */
        layout.Push<GLfloat>(2);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        lab::VertexShader vs{ "resources/shaders/Vertex.glsl" };
        lab::FragmentShader fs{ "resources/shaders/Fragment.glsl" };

        m_Shader = MakeRef<ShaderProgram>();
        m_Shader->AttachShader(vs);
        m_Shader->AttachShader(fs);
        m_Shader->Link();
        m_Shader->Bind();

        m_Container = MakeRef<Texture>("resources/images/container.jpg", GL_TEXTURE0);
        m_Face = MakeRef<Texture>("resources/images/awesomeface.png", GL_TEXTURE1);

        m_Shader->SetUniform1i("u_Container", 0);
        m_Shader->SetUniform1i("u_Face", 1);

        m_Camera = MakeRef<Camera>(glm::vec3{ 0.0f, 0.0f, 3.0f });
        Add(m_Camera);
        m_Proj = glm::perspective(glm::radians(m_Camera->GetFieldOfView()), 800.0f / 600.0f, 0.1f, 100.f);
    }

    void TestScene::OnUpdate(float deltaTime)
    {
        Scene::OnUpdate(deltaTime);
        
        m_Proj = glm::perspective(glm::radians(m_Camera->GetFieldOfView()), 800.0f / 600.0f, 0.1f, 100.0f);
        m_Shader->SetUniformMatrix4fv("u_Proj", m_Proj);
        m_Shader->SetUniformMatrix4fv("u_View", m_Camera->GetView());
    }

    void TestScene::OnRender()
    {
        Scene::OnRender();

        GL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        m_VertexArray->Bind();
        m_Shader->Bind();
        m_Container->Bind();
        m_Face->Bind();

        for (int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_Shader->SetUniformMatrix4fv("u_Model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void TestScene::OnGui()
    {
        Scene::OnGui();
    }
}
