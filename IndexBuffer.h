#pragma once

#include <gl/glew.h>

namespace lab
{
    class IndexBuffer
    {
    private:
        GLuint m_Handle;
    public:
        IndexBuffer(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        ~IndexBuffer();
        void Bind() const;
        void Unbind() const;
    };
}
