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

#include "Debug.h"

void CheckShaderInfoLog(GLuint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar* message = new GLchar[length];
        glGetShaderInfoLog(shader, length, NULL, message);
        std::cout << "[OPENGL] [SHADER] " << message << std::endl;
        delete[] message;
    }
}

int main()
{
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
    { //  x      y    //  R     G     B    //  S     T
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, // Bottom-left
    };

    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const GLchar* vertexSource = R"glsl(
        #version 330 core
        
        in vec2 i_Position;
        in vec3 i_Color;
        in vec2 i_TexCoord;

        out vec3 p_Color;
        out vec2 p_TexCoord;

        uniform mat4 u_Model;
        uniform mat4 u_View;
        uniform mat4 u_Proj;
    
        void main()
        {
            p_TexCoord = i_TexCoord;
            p_Color = i_Color;
            gl_Position = u_Proj * u_View * u_Model * vec4(i_Position, 0.0, 1.0);
        }
    )glsl";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderInfoLog(vertexShader);

    const GLchar* fragmentSource = R"glsl(
        #version 330 core

        in vec3 p_Color;
        in vec2 p_TexCoord;

        out vec4 color;

        uniform sampler2D u_Kitten;
        uniform sampler2D u_Puppy;
        
        void main()
        {
            vec4 colKitten = texture(u_Kitten, p_TexCoord);
            vec4 colPuppy = texture(u_Puppy, p_TexCoord);
            color = mix(colKitten, colPuppy, 0.5);
        }
    )glsl";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderInfoLog(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "i_Position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    
    GLint colAttrib = glGetAttribLocation(shaderProgram, "i_Color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "i_TexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));

    GLint width, height;
    stbi_set_flip_vertically_on_load(1);
    GLubyte* img;
    GLuint textures[2];
    GL_CHECK(glGenTextures(2, textures));

    img = stbi_load("sample.png", &width, &height, 0, 4);
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textures[0]));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
    stbi_image_free(img);
    GL_CHECK(GLint u_Kitten = glGetUniformLocation(shaderProgram, "u_Kitten"));
    GL_CHECK(glUniform1i(u_Kitten, 0));
    /* Wrapping */
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    /* Filtering */
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    img = stbi_load("sample2.png", &width, &height, 0, 4);
    GL_CHECK(glActiveTexture(GL_TEXTURE1));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textures[1]));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
    stbi_image_free(img);
    GL_CHECK(GLint u_Puppy = glGetUniformLocation(shaderProgram, "u_Puppy"));
    GL_CHECK(glUniform1i(u_Puppy, 1));
    /* Wrapping */
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    /* Filtering */
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    glm::mat4 view = glm::lookAt(
        glm::vec3{ 1.2f, 1.2f, 1.2f },
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 1.0f }
    );
    GL_CHECK(GLint u_View = glGetUniformLocation(shaderProgram, "u_View"));
    GL_CHECK(glUniformMatrix4fv(u_View, 1, GL_FALSE, glm::value_ptr(view)));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GL_CHECK(GLint u_Proj = glGetUniformLocation(shaderProgram, "u_Proj"));
    GL_CHECK(glUniformMatrix4fv(u_Proj, 1, GL_FALSE, glm::value_ptr(proj)));

    auto t_start = std::chrono::high_resolution_clock::now();
    bool showMetrics = false;
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showMetrics)
            ImGui::ShowMetricsWindow(&showMetrics);

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        ImGui::BeginMainMenuBar();
        if (ImGui::Button("Metrics"))
            showMetrics = !showMetrics;
        ImGui::Text((char*) glGetString(GL_VERSION));
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::Text("Seconds since start: %.2fs", time);
        ImGui::EndMainMenuBar();

        ImGui::Begin("Debug");
        ImGui::End();

        glm::mat4 model{ 1.0f };
        model = glm::rotate(model, time * glm::radians(180.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
        GL_CHECK(GLint u_Model = glGetUniformLocation(shaderProgram, "u_Model"));
        GL_CHECK(glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model)));
        
        glm::mat4 view = glm::lookAt(
            glm::vec3{ 1.2f, 1.2f, 1.2f },
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, 1.0f }
        );
        GL_CHECK(GLint u_View = glGetUniformLocation(shaderProgram, "u_View"));
        GL_CHECK(glUniformMatrix4fv(u_View, 1, GL_FALSE, glm::value_ptr(view)));

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
        GL_CHECK(GLint u_Proj = glGetUniformLocation(shaderProgram, "u_Proj"));
        GL_CHECK(glUniformMatrix4fv(u_Proj, 1, GL_FALSE, glm::value_ptr(proj)));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    glDeleteTextures(2, textures);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
