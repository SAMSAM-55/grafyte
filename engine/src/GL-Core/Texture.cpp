#include "Texture.h"

#include "glad/glad.h"

#include "GlContextState.h"
#include "macros.hpp"
#include "stb_image/stb_image.h"

#include "embedd/EmbeddedAsset.h"

namespace grafyte
{
Texture::~Texture()
{
    release();
}

// ==================================================
// [SECTION] Move contructors
// ==================================================

Texture::Texture(Texture &&other) noexcept
    : m_RendererID(other.m_RendererID), m_FilePath(std::move(other.m_FilePath)), m_LocalBuffer(other.m_LocalBuffer),
      m_Width(other.m_Width), m_Height(other.m_Height), m_BPP(other.m_BPP)
{
    other.m_RendererID = 0;
    other.m_LocalBuffer = nullptr;
    other.m_Width = other.m_Height = other.m_BPP = 0;
}
Texture &Texture::operator=(Texture &&other) noexcept
{
    if (this == &other)
        return *this;

    release();

    m_RendererID = other.m_RendererID;
    m_FilePath = std::move(other.m_FilePath);
    m_LocalBuffer = other.m_LocalBuffer;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_BPP = other.m_BPP;

    other.m_RendererID = 0;
    other.m_LocalBuffer = nullptr;
    other.m_Width = other.m_Height = other.m_BPP = 0;
    return *this;
}

void Texture::set(const std::string &path)
{
    stbi_set_flip_vertically_on_load(1);
    loadDataToBuffer(path, m_LocalBuffer);

    GL_CALL(glGenTextures(1, &m_RendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
        m_LocalBuffer = nullptr;
    }
}

void Texture::bind(GLuint slot) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind()
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::loadDataToBuffer(const std::string &idOrPath, unsigned char *&buffer)
{
    if (idOrPath == "@embed/Textures/Default" || idOrPath == "@embed/Textures/No")
    {
        buffer =
            stbi_load_from_memory(embedded::NoTexture.data, embedded::NoTexture.size, &m_Width, &m_Height, &m_BPP, 4);
        return;
    }

    buffer = stbi_load(idOrPath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
}

void Texture::release()
{
    // Guard to avoid deleting the shader without a valid OpenGL context
    if (m_RendererID && glContextAlive())
    {
        unbind();
        GL_CALL(glDeleteTextures(1, &m_RendererID));
        m_RendererID = 0;
    }
}
} // namespace grafyte
