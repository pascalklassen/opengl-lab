#pragma once

#include <gl/glew.h>

#include <string>

namespace lab
{
    class Texture
    {
    private:
        static bool flip;

        GLsizei m_Width, m_Height;
        GLuint m_Handle;
        GLenum m_Slot;
    public:
        static void SetFlip(bool flip) { Texture::flip = flip; }

        Texture(const std::string& filepath, GLenum slot = GL_TEXTURE0);
        ~Texture();
        void Bind() const;
        void Unbind() const;
        GLsizei GetWidth() const { return m_Width; }
        GLsizei GetHeight() const { return m_Height; }
    };
}
