#include "VertexArray.h"

#include "VertexBufferLayout.h"

#include "glad/glad.h"

#include "GlContextState.h"
#include "macros.hpp"

namespace grafyte
{
VertexArray::VertexArray()
{
    GL_CALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    release();
}

void VertexArray::release()
{
    // Guard to avoid deleting the shader without a valid OpenGL context
    if (m_RendererID && glContextAlive())
    {
        GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
        m_RendererID = 0;
    }
}

// ==================================================
// [SECTION] Move contructors
// ==================================================

VertexArray::VertexArray(VertexArray &&other) noexcept : m_RendererID(other.m_RendererID)
{
    other.m_RendererID = 0;
}
VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
    if (this != &other)
    {
        release();
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}
void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const
{
    bind();
    vb.bind();
    const auto &elements = layout.getElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        if (element.type == GL_UNSIGNED_INT || element.type == GL_INT)
        {
            GL_CALL(glVertexAttribIPointer(i, element.count, element.type, layout.getStride(),
                                          reinterpret_cast<void *>(static_cast<uintptr_t>(element.offset))));
        }
        else
        {
            GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(),
                                         reinterpret_cast<void *>(static_cast<uintptr_t>(element.offset))));
        }
    }
}

void VertexArray::bind() const
{
    GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind()
{
    GL_CALL(glBindVertexArray(0));
}
} // namespace grafyte
