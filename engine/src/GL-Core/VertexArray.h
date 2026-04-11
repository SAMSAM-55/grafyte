#pragma once

#include "VertexBuffer.h"

namespace grafyte
{
class VertexBufferLayout;

class VertexArray
{
  public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept;

    VertexArray &operator=(VertexArray &&other) noexcept;

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const;

    void bind() const;
    static void unbind() ;

    void release();
  private:
    GLuint m_RendererID = 0;
};
} // namespace grafyte
