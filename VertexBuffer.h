#pragma once

#include <gl/glew.h>

#include <vector>

namespace lab
{
    class VertexBufferLayout
    {
    private:
        struct LayoutElement
        {
            GLenum type;
            GLsizeiptr size;
            GLint count;
            GLboolean normalized;
        };

        std::vector<LayoutElement> m_Elements;
        GLsizei m_Stride;
    public:
        VertexBufferLayout()
            : m_Stride{ 0 }
        {}

        template <typename T>
        void Push(GLint count);

        const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
        GLsizei GetStride() const { return m_Stride; }
    };

    class VertexBuffer
    {
    private:
        GLuint m_Handle;
    public:
        VertexBuffer(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;
    };
}
