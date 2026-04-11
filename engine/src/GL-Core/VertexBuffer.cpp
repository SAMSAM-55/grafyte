#include "VertexBuffer.h"

#include "GlContextState.h"
#include "glad/glad.h"

namespace grafyte
{
VertexBuffer::VertexBuffer(const void *data, GLuint size, GLenum usage)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
    release();
}

void VertexBuffer::release()
{
    // Guard to avoid deleting the shader without a valid OpenGL context
    if (m_RendererID && glContextAlive())
    {
        glDeleteBuffers(1, &m_RendererID);
        m_RendererID = 0;
    }
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept : m_RendererID(other.m_RendererID)
{
    other.m_RendererID = 0;
}
VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept
{
    if (this != &other)
    {
        release();
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}

void VertexBuffer::updateData(const void *data, GLuint size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
} // namespace grafyte
