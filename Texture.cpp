#include "Texture.h"

#include <gl/glew.h>
#include <stb_image.h>

#include <string>

#include "Debug.h"

namespace lab
{
    Texture::Texture(const std::string& filepath, bool flip /*= false*/)
    {
        stbi_set_flip_vertically_on_load(flip);
        GL_CHECK(glGenTextures(1, &m_Handle));
        Bind();

        GLubyte* img = stbi_load("sample.png", &m_Width, &m_Height, 0, 4);
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
        stbi_image_free(img);
    }

    Texture::~Texture()
    {
        GL_CHECK(glDeleteTextures(1, &m_Handle));
    }

    void Texture::Bind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_Handle));
    }

    void Texture::Unbind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }
}
