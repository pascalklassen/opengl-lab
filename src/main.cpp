#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <chrono>

#include "core/core.h"
#include "test/TestScene.h"

int main()
{
    auto t_start = std::chrono::high_resolution_clock::now();

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "OpenGL Lab", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    {
        GLuint fbo;
        GL_CHECK(glGenFramebuffers(1, &fbo));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        GLuint texColBuffer;
        GL_CHECK(glGenTextures(1, &texColBuffer));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, texColBuffer));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColBuffer, 0));

        GLuint rbo;
        GL_CHECK(glGenRenderbuffers(1, &rbo));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));

        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        GLfloat fvertices[] =
        {
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        lab::VertexArray fvao;
        fvao.Bind();

        lab::VertexBuffer fvbo{ sizeof(fvertices), fvertices };
        fvbo.Bind();

        lab::VertexBufferLayout flayout;
        flayout.Push<GLfloat>(2);
        flayout.Push<GLfloat>(2);

        fvao.AddBuffer(fvbo, flayout);

        lab::VertexShader fvs{ "resources/shaders/FrameBuffer_Vertex.glsl" };
        lab::FragmentShader ffs{ "resources/shaders/FrameBuffer_Fragment.glsl" };
        lab::ShaderProgram fprogram;
        fprogram.AttachShader(fvs);
        fprogram.AttachShader(ffs);
        fprogram.Link();
        fprogram.Bind();
        fprogram.SetUniform1i("u_FrameBuffer", 0);

        lab::LayerRenderer renderer;
        lab::Ref<lab::TestScene> test = lab::MakeRef<lab::TestScene>();
        renderer.Render(test);

        bool showMetrics = false;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (showMetrics)
                ImGui::ShowMetricsWindow(&showMetrics);

            ImGui::BeginMainMenuBar();
            if (ImGui::Button("Metrics"))
                showMetrics = !showMetrics;
            ImGui::Text((char*) glGetString(GL_VERSION));
            ImGui::Separator();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("Delta Time: %.4fs", deltaTime);
            ImGui::EndMainMenuBar();

            ImGui::Begin("Debug");
            ImGui::End();

            /* FrameBuffer Begin */
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

            renderer.OnUpdate(deltaTime);
            renderer.OnRender();
            renderer.OnGui();

            /* FrameBuffer End */
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            fvao.Bind();
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            fprogram.Bind();
            GL_CHECK(glActiveTexture(GL_TEXTURE0));
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, texColBuffer));
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

            ImGui::Begin("Scene");
            ImGui::BeginChild("Renderer");
            ImVec2 wsize = ImGui::GetWindowSize();
            ImGui::Image((ImTextureID) texColBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
            ImGui::End();

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        GL_CHECK(glDeleteRenderbuffers(1, &rbo));
        GL_CHECK(glDeleteFramebuffers(1, &fbo));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
