#pragma once

#include <gl/glew.h>

namespace lab
{
    class IndexBuffer
    {
    private:
        GLuint m_Handle;
        GLuint m_Count;
    public:
        IndexBuffer(GLuint count, const GLuint* data, GLenum usage = GL_STATIC_DRAW);
        ~IndexBuffer();
        void Bind() const;
        void Unbind() const;
        GLuint GetCount() const { return m_Count; }
    };
}
