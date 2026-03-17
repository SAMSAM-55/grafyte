#include "IndexBuffer.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "macros.hpp"

namespace grafyte
{
    IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count)
        :m_RendererID(0), m_Count(count)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        release();
    }

    void IndexBuffer::release() {
        if (m_RendererID && glfwGetCurrentContext()) {
            GLCall(glDeleteBuffers(1, &m_RendererID));
            m_RendererID = 0;
        }
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
