#include "IndexBuffer.h"

#include <gl/glew.h>

#include "Debug.h"

namespace lab
{

    IndexBuffer::IndexBuffer(GLsizeiptr size, const void* data, GLenum usage /*= GL_STATIC_DRAW*/)
        : m_Handle{ 0 }
    {
        GL_CHECK(glGenBuffers(1, &m_Handle));
        Bind();
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
        Unbind();
    }

    IndexBuffer::~IndexBuffer()
    {
        GL_CHECK(glDeleteBuffers(1, &m_Handle));
    }

    void IndexBuffer::Bind() const
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
    }

    void IndexBuffer::Unbind() const
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}
