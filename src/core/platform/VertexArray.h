#pragma once

#include <gl/glew.h>

#include "VertexBuffer.h"

namespace lab
{
    class VertexArray
    {
    private:
        GLuint m_Handle;
    public:
        VertexArray();
        ~VertexArray();
        void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
    };
}
