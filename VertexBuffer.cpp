#include "VertexBuffer.h"

#include <gl/glew.h>

#include "Debug.h"

namespace lab
{
    template <>
    void VertexBufferLayout::Push<GLfloat>(GLint count)
    {
        m_Stride += count * sizeof(GLfloat);
        m_Elements.push_back(
            {
                GL_FLOAT,
                sizeof(GLfloat),
                count,
                GL_FALSE
            }
        );
    }

    VertexBuffer::VertexBuffer(GLsizeiptr size, const void* data, GLenum usage /*= GL_STATIC_DRAW*/)
        : m_Handle{ 0 }
    {
        GL_CHECK(glGenBuffers(1, &m_Handle));
        Bind();
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
        Unbind();
    }

    VertexBuffer::~VertexBuffer()
    {
        GL_CHECK(glDeleteBuffers(1, &m_Handle));
    }

    void VertexBuffer::Bind() const
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
    }

    void VertexBuffer::Unbind() const
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

}
