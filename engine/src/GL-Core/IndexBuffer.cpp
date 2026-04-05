#include "IndexBuffer.h"

#include "glad/glad.h"

#include "macros.hpp"
#include "GlContextState.h"

namespace grafyte
{
    IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count, unsigned int usage)
        :m_RendererID(0), m_Count(count)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage));
    }

    IndexBuffer::~IndexBuffer()
    {
        release();
    }

    void IndexBuffer::release() {
        if (m_RendererID && GlContextAlive()) {
            GLCall(glDeleteBuffers(1, &m_RendererID));
            m_RendererID = 0;
        }
    }

    void IndexBuffer::UpdateData(const unsigned int* data, unsigned int count) {
        m_Count = count;
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
    }

    void IndexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    }

    void IndexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}
