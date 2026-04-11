#pragma once

#include <string>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace grafyte
{
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
  public:
    Shader(Shader const &) = delete;
    Shader &operator=(Shader const &) = delete;

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    explicit Shader(const std::string &filepath);
    ~Shader();

    void bind() const;
    static void unbind();

    // Set uniforms
    void setUniform1i(const std::string &name, int value) const;
    void setUniform1f(const std::string &name, float value) const;
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) const;
    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix) const;

    void release();
  private:
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string m_FilePath;
    GLuint m_RendererID = 0;
    // Caching for uniforms
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    static ShaderProgramSource parseShader(const std::string &filePath);
    static GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader);

    static GLuint compileShader(GLenum type, const std::string &source);
    int getUniformLocation(const std::string &name) const;

    static std::string getShaderSource(const std::string &idOrPath);
    static ShaderProgramSource parseShaderSource(const std::string &source);
};
} // namespace grafyte
