#include "Texture.h"

#include <gl/glew.h>
#include <stb_image.h>

#include <string>

#include "Debug.h"

namespace lab
{
    bool Texture::flip = false;

    Texture::Texture(const std::string& filepath, GLenum slot /*= GL_TEXTURE0*/)
        : m_Slot{ slot }
    {
        GL_CHECK(glActiveTexture(m_Slot));
        GL_CHECK(glGenTextures(1, &m_Handle));
        Bind();

        stbi_set_flip_vertically_on_load(Texture::flip);
        GLubyte* img = stbi_load(filepath.c_str(), &m_Width, &m_Height, 0, 4);
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
        stbi_image_free(img);

        /* Wrapping */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        /* Filtering */
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    Texture::~Texture()
    {
        GL_CHECK(glDeleteTextures(1, &m_Handle));
    }

    void Texture::Bind() const
    {
        GL_CHECK(glActiveTexture(m_Slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_Handle));
    }

    void Texture::Unbind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }
}
