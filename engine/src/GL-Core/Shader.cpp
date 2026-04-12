#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "GlContextState.h"
#include "embedd/EmbeddedAsset.h"
#include "macros.hpp"
#include "utils.hpp"

namespace grafyte
{
Shader::Shader(const std::string &filepath) : m_FilePath(filepath), m_RendererID(0)
{
    auto [VertexSource, FragmentSource] = parseShader(filepath);
    m_RendererID = createShader(VertexSource, FragmentSource);
}

Shader::~Shader()
{
    release();
}

// ==================================================
// [SECTION] Move constructors
// ==================================================

Shader::Shader(Shader &&other) noexcept
    : m_FilePath(std::move(other.m_FilePath)), m_RendererID(other.m_RendererID),
      m_UniformLocationCache(std::move(other.m_UniformLocationCache))
{
    other.m_RendererID = 0;
    other.m_UniformLocationCache = std::unordered_map<std::string, int>();
    other.m_FilePath = "";
};
Shader &Shader::operator=(Shader &&other) noexcept
{
    if (this == &other)
        return *this;

    release();

    m_FilePath = std::move(other.m_FilePath);
    m_RendererID = other.m_RendererID;
    m_UniformLocationCache = std::move(other.m_UniformLocationCache);

    other.m_RendererID = 0;
    other.m_UniformLocationCache = std::unordered_map<std::string, int>();
    other.m_FilePath = "";
    return *this;
}

// ==================================================
// [SECTION] Shader compilation
// ==================================================

GLuint Shader::compileShader(GLenum type, const std::string &source)
{
    GLuint id = 0;
    GL_CALL(id = glCreateShader(type));
    const char *src = source.c_str();
    const auto len = static_cast<GLint>(source.size());
    GL_CALL(glShaderSource(id, 1, &src, &len));
    GL_CALL(glCompileShader(id));

    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::string message(length, '\0');
        GL_CALL(glGetShaderInfoLog(id, length, &length, message.data()));
        fprintf(stderr, "[OpenGL Shader Compilation] %s shader compilation failed! Error:\n%s\n",
                (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message.c_str());
        GL_CALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

GLuint Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    GLuint program = 0;
    GL_CALL(program = glCreateProgram());
    const GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    const GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return program;
}

void Shader::bind() const
{
    GL_CALL(glUseProgram(m_RendererID));
}

void Shader::unbind()
{
    GL_CALL(glUseProgram(0));
}

void Shader::release()
{
    // Guard to avoid deleting the shader without a valid OpenGL context
    if (m_RendererID && glContextAlive())
    {
        unbind();
        GL_CALL(glDeleteProgram(m_RendererID));
        m_RendererID = 0;
    }
}

// ==================================================
// [SECTION] Uniforms
// ==================================================

void Shader::setUniform1i(const std::string &name, int value) const
{
    GL_CALL(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string &name, float value) const
{
    GL_CALL(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) const
{
    GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix) const
{
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string &name) const
{
    // Cache uniform locations to avoid repeated glGetUniformLocation calls.
    if (m_UniformLocationCache.contains(name))
        return m_UniformLocationCache.at(name);

    int location;
    GL_CALL(location = glGetUniformLocation(m_RendererID, name.c_str()));

    if (location == -1)
    {
        fprintf(stderr, "[OpenGL Shader Uniform](%s) : Uniform '%s' not found.\n", m_FilePath.c_str(), name.c_str());
    }
    else
        m_UniformLocationCache.insert_or_assign(name, location);

    return location;
}

// ==================================================
// [SECTION] Shader source parsing
// ==================================================

ShaderProgramSource Shader::parseShader(const std::string &filePath)
{
    const std::string src = getShaderSource(filePath);
    return parseShaderSource(src);
}

std::string Shader::getShaderSource(const std::string &idOrPath)
{
    // Special virtual paths resolve shaders embedded in the binary.
    if (idOrPath == "@embed/Shaders/Basic")
    {
        const auto &[data, size] = embedded::BasicShader;

        return {reinterpret_cast<const char *>(data), size};
    }
    if (idOrPath == "@embed/Shaders/Texture")
    {
        const auto &[data, size] = embedded::TextureShader;

        return {reinterpret_cast<const char *>(data), size};
    }
    if (idOrPath == "@embed/Shaders/Text")
    {
        const auto &[data, size] = embedded::TextShader;

        return {reinterpret_cast<const char *>(data), size};
    }

    std::ifstream file(idOrPath, std::ios::binary);
    if (!file)
        throw std::runtime_error("[OpenGL Shader Processing] Could not open shader file: " + idOrPath);

    return {(std::istreambuf_iterator(file)), std::istreambuf_iterator<char>()};
}

ShaderProgramSource Shader::parseShaderSource(const std::string &source)
{
    std::istringstream stream(utils::stripUtf8Bom(source));

    std::string line;
    std::stringstream ss[2];
    auto type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else if (static_cast<int>(type) >= 0)
        {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
}
} // namespace grafyte
