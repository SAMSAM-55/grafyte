#pragma once

#include <string>

#include "glad/glad.h"

namespace grafyte
{
class Texture
{
  public:
    Texture() = default;
    ~Texture();

    Texture(Texture const &) = delete;
    Texture &operator=(Texture const &) = delete;

    Texture(Texture &&other) noexcept;

    Texture &operator=(Texture &&other) noexcept;

    void set(const std::string &path);
    void bind(GLuint slot) const;
    static void unbind();

    void release();
  private:
    void loadDataToBuffer(const std::string &idOrPath, unsigned char *&buffer);
    GLuint m_RendererID = 0;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer = nullptr;
    int m_Width = 0, m_Height = 0, m_BPP = 0;
};
} // namespace grafyte
