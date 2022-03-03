#include "Shader.h"

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

#include "Debug.h"

namespace lab
{
    static void CheckShaderInfoLog(GLuint shader)
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

    Shader::Shader(std::ifstream file)
        : m_Handle{ 0 }, m_Source{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() }
    {}

    Shader::~Shader()
    {
        GL_CHECK(glDeleteShader(m_Handle));
    }

    GLint Shader::Compile()
    {
        const GLchar* source = m_Source.c_str();
        GL_CHECK(glShaderSource(m_Handle, 1, &source, NULL));
        GL_CHECK(glCompileShader(m_Handle));

        GLint result;
        GL_CHECK(glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result));
        return result;
    }

    void Shader::Assemble()
    {
        if (Compile() == GL_FALSE)
        {
            CheckShaderInfoLog(m_Handle);
            GL_CHECK(glDeleteShader(m_Handle));
        }
    }

    VertexShader::VertexShader(const std::string& filepath)
        : Shader(std::ifstream{ filepath })
    {
        GL_CHECK(m_Handle = glCreateShader(GL_VERTEX_SHADER));
        Assemble();
    }

    FragmentShader::FragmentShader(const std::string& filepath)
        : Shader(std::ifstream{ filepath })
    {
        GL_CHECK(m_Handle = glCreateShader(GL_FRAGMENT_SHADER));
        Assemble();
    }

    ShaderProgram::ShaderProgram()
        : m_Handle{ 0 }
    {
        GL_CHECK(m_Handle = glCreateProgram());
    }

    ShaderProgram::~ShaderProgram()
    {
        GL_CHECK(glDeleteProgram(m_Handle));
    }

    void ShaderProgram::AttachShader(const Shader& shader) const
    {
        GL_CHECK(glAttachShader(m_Handle, shader.m_Handle));
    }

    void ShaderProgram::Link() const
    {
        GL_CHECK(glLinkProgram(m_Handle));
    }

    void ShaderProgram::Bind() const
    {
        GL_CHECK(glUseProgram(m_Handle));
    }

    void ShaderProgram::Unbind() const
    {
        GL_CHECK(glUseProgram(0));
    }

    void ShaderProgram::SetUniform1f(const std::string& name, GLfloat value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform1f(location, value));
    }

    void ShaderProgram::SetUniform2f(const std::string& name, const glm::vec2& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform2f(location, value[0], value[1]));
    }

    void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform3f(location, value[0], value[1], value[2]));
    }

    void ShaderProgram::SetUniform4f(const std::string& name, const glm::vec4& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform4f(location, value[0], value[1], value[2], value[3]));
    }

    void ShaderProgram::SetUniform1i(const std::string& name, GLint value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform1i(location, value));
    }

    void ShaderProgram::SetUniform2i(const std::string& name, const glm::ivec2& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform2i(location, value[0], value[1]));
    }

    void ShaderProgram::SetUniform3i(const std::string& name, const glm::ivec3& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform3i(location, value[0], value[1], value[2]));
    }

    void ShaderProgram::SetUniform4i(const std::string& name, const glm::ivec4& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniform4i(location, value[0], value[1], value[2], value[3]));
    }

    void ShaderProgram::SetUniformMatrix4fv(const std::string& name, const glm::mat4& value)
    {
        Bind();
        GL_CHECK(GLint location = glGetUniformLocation(m_Handle, name.c_str()));
        GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }
}
