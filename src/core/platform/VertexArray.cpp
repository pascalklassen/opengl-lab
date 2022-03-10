#include "VertexArray.h"

#include <gl/glew.h>

#include "../debug/debug.h"
#include "VertexBuffer.h"

namespace lab
{

    VertexArray::VertexArray()
        : m_Handle{ 0 }
    {
        GL_CHECK(glGenVertexArrays(1, &m_Handle));
    }

    VertexArray::~VertexArray()
    {
        GL_CHECK(glDeleteVertexArrays(1, &m_Handle));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
    {
        Bind();
        vbo.Bind();

        const auto& elements = layout.GetElements();
        for (GLuint index = 0, offset = 0; index < elements.size(); index++)
        {
            const auto& element = elements[index];
            GL_CHECK(glEnableVertexAttribArray(index));
            GL_CHECK(glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.GetStride(), (void*) offset));
            offset += element.count * element.size;
        }
    }

    void VertexArray::Bind() const
    {
        GL_CHECK(glBindVertexArray(m_Handle));
    }

    void VertexArray::Unbind() const
    {
        GL_CHECK(glBindVertexArray(0));
    }
}
