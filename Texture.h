#pragma once

#include <gl/glew.h>

#include <string>

namespace lab
{
    class Texture
    {
    private:
        GLsizei m_Width, m_Height;
        GLuint m_Handle;
    public:
        Texture(const std::string& filepath, bool flip = false);
        ~Texture();
        void Bind() const;
        void Unbind() const;
        GLsizei GetWidth() const { return m_Width; }
        GLsizei GetHeight() const { return m_Height; }
    };
}
