#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <chrono>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Debug.h"

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

    GLfloat vertices[] =
    { //  X      Y      Z     R     G     B     U     V
        /* Cube */
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        /* Floor */
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GL_CHECK(glEnable(GL_DEPTH_TEST));

    {
        lab::VertexArray vao;
        vao.Bind();

        lab::VertexBuffer vbo{ sizeof(vertices), vertices };
        vbo.Bind();

        lab::VertexBufferLayout layout;
        /* Position */
        layout.Push<GLfloat>(3);
        /* Color */
        layout.Push<GLfloat>(3);
        /* Texture */
        layout.Push<GLfloat>(2);

        vao.AddBuffer(vbo, layout);

        lab::IndexBuffer ibo{ sizeof(indices), indices };
        ibo.Bind();

        lab::ShaderProgram program;
        lab::VertexShader vs{ "Vertex.glsl" };
        lab::FragmentShader fs{ "Fragment.glsl" };

        program.AttachShader(vs);
        program.AttachShader(fs);
        program.Link();
        program.Bind();
        
        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        lab::Texture kitten{ "sample.png" };
        /* Wrapping */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        /* Filtering */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        program.SetUniform1i("u_Kitten", 0);
        
        GL_CHECK(glActiveTexture(GL_TEXTURE1));
        lab::Texture puppy{ "sample2.png" };
        /* Wrapping */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        /* Filtering */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        program.SetUniform1i("u_Puppy", 1);

        glm::mat4 view = glm::lookAt(
            glm::vec3{ 2.5f, 2.5f, 2.5f },
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, 1.0f }
        );
        program.SetUniformMatrix4fv("u_View", view);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
        program.SetUniformMatrix4fv("u_Proj", proj);

        /*GLuint fbo;
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

        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_RENDERBUFFER, rbo));*/

        bool showMetrics = false;
        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            auto t_now = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

            GL_CHECK(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (showMetrics)
                ImGui::ShowMetricsWindow(&showMetrics);

            ImGui::BeginMainMenuBar();
            if (ImGui::Button("Metrics"))
                showMetrics = !showMetrics;
            ImGui::Text((char*)glGetString(GL_VERSION));
            ImGui::Separator();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("Seconds since start: %.2fs", time);
            ImGui::EndMainMenuBar();

            ImGui::Begin("Debug");
            ImGui::End();

            /* Draw Cube */
            glm::mat4 model{ 1.0f };
            model = glm::rotate(
                model,
                time * glm::radians(180.0f),
                glm::vec3{ 0.0f, 0.0f, 1.0f }
            );
            program.SetUniformMatrix4fv("u_Model", model);
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));

            /* Draw Floor */
            GL_CHECK(glEnable(GL_STENCIL_TEST));
            GL_CHECK(glStencilFunc(GL_ALWAYS, 1, 0xFF));
            GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            GL_CHECK(glStencilMask(0xFF));
            GL_CHECK(glDepthMask(GL_FALSE));
            GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 36, 6));

            /* Draw Cube Reflection */
            GL_CHECK(glStencilFunc(GL_EQUAL, 1, 0xFF));
            GL_CHECK(glStencilMask(0x00));
            GL_CHECK(glDepthMask(GL_TRUE));

            model = glm::scale(
                glm::translate(model, glm::vec3{ 0, 0, -1 }),
                glm::vec3{ 1, 1, -1 }
            );
            program.SetUniformMatrix4fv("u_Model", model);

            program.SetUniform3f("u_OverrideColor", { 0.3f, 0.3f, 0.3f });
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));
            program.SetUniform3f("u_OverrideColor", { 1.0f, 1.0f, 1.0f });

            GL_CHECK(glDisable(GL_STENCIL_TEST));
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    }

    /*GL_CHECK(glDeleteRenderbuffers(1, &rbo));
    GL_CHECK(glDeleteFramebuffers(1, &fbo));*/

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
