#pragma once

#include "macros.hpp"
#include <glad/glad.h>
#include <vector>

namespace grafyte
{
struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    unsigned int offset;

    static unsigned int getSizeOfType(GLenum type)
    {
        switch (type)
        {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            DEBUG_BREAK();
            return 0;
        }
    }
};

class VertexBufferLayout
{
  public:
    VertexBufferLayout() : m_Stride(0)
    {
    }

    template <typename T> void push(GLuint count)
    {
        static_assert(sizeof(T) == 0);
    }

    [[nodiscard]] const std::vector<VertexBufferElement> &getElements() const
    {
        return m_Elements;
    }
    [[nodiscard]] unsigned int getStride() const
    {
        return m_Stride;
    }

  private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

template <> inline void VertexBufferLayout::push<float>(GLuint count)
{
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE, m_Stride});
    m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
}
template <> inline void VertexBufferLayout::push<unsigned int>(GLuint count)
{
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE, m_Stride});
    m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
}
template <> inline void VertexBufferLayout::push<unsigned char>(GLuint count)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE, m_Stride});
    m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}
} // namespace grafyte
