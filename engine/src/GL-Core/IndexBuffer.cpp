#include "IndexBuffer.h"

#include "GlContextState.h"
#include "glad/glad.h"
#include "macros.hpp"

namespace grafyte
{
IndexBuffer::IndexBuffer(const uint32_t *data, uint32_t count, GLenum usage) : m_Count(count), m_Usage(usage)
{
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage));
}

IndexBuffer::~IndexBuffer()
{
    release();
}

void IndexBuffer::release()
{
    // Guard to avoid deleting the shader without a valid OpenGL context
    if (m_RendererID && glContextAlive())
    {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
        m_RendererID = 0;
    }
}

// ==================================================
// [SECTION] Move contructors
// ==================================================

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : m_RendererID(other.m_RendererID), m_Count(other.m_Count), m_Usage(other.m_Usage)
{
    other.m_RendererID = 0;
    other.m_Count = 0;
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
{
    if (this != &other)
    {
        release();
        m_RendererID = other.m_RendererID;
        m_Count = other.m_Count;
        other.m_RendererID = 0;
        other.m_Count = 0;
    }
    return *this;
}

void IndexBuffer::updateData(const uint32_t *data, uint32_t count)
{
    m_Count = count;
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, m_Usage));
}

void IndexBuffer::bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind()
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
} // namespace grafyte
