#pragma once

#include <cstdint>

#include "glad/glad.h"

namespace grafyte
{
class IndexBuffer
{
  public:
    IndexBuffer(const uint32_t *data, uint32_t count, GLenum usage = GL_STATIC_DRAW);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept;
    IndexBuffer &operator=(IndexBuffer &&other) noexcept;

    void updateData(const uint32_t *data, uint32_t count);

    void bind() const;
    static void unbind();

    [[nodiscard]] GLsizei getCount() const
    {
        return static_cast<GLsizei>(m_Count);
    }
    void release();

  private:
    GLuint m_RendererID = 0;
    uint32_t m_Count = 0;
    GLenum m_Usage;
};
} // namespace grafyte
