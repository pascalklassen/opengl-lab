#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <filesystem>

namespace lab
{
    class Shader
    {
    protected:
        friend class ShaderProgram;

        GLuint m_Handle;
        std::string m_Source;
        GLint Compile();
        void Assemble();
    public:
        Shader(std::ifstream file);
        ~Shader();
    };

    class VertexShader : public Shader
    {
    public:
        VertexShader(const std::string& filepath);
    };

    class FragmentShader : public Shader
    {
    public:
        FragmentShader(const std::string& filepath);
    };

    class ShaderProgram
    {
    private:
        GLuint m_Handle;
    public:
        ShaderProgram();
        ~ShaderProgram();

        void AttachShader(const Shader& shader) const;
        void Link() const;
        void Bind() const;
        void Unbind() const;

        void SetUniform1f(const std::string& name, GLfloat value);
        void SetUniform2f(const std::string& name, const glm::vec2& value);
        void SetUniform3f(const std::string& name, const glm::vec3& value);
        void SetUniform4f(const std::string& name, const glm::vec4& value);

        void SetUniform1i(const std::string& name, GLint value);
        void SetUniform2i(const std::string& name, const glm::ivec2& value);
        void SetUniform3i(const std::string& name, const glm::ivec3& value);
        void SetUniform4i(const std::string& name, const glm::ivec4& value);

        void SetUniformMatrix4fv(const std::string& name, const glm::mat4& value);
    };
}
