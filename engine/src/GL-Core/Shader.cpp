#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "macros.hpp"
#include "utils.hpp"
#include "embedd/EmbeddedAsset.h"

namespace grafyte
{
    Shader::Shader(const std::string& filepath)
        :m_FilePath(filepath), m_RendererID(0)
    {
        auto [VertexSource, FragmentSource] = ParseShader(filepath);

        m_RendererID = CreateShader(VertexSource, FragmentSource);
    }

    Shader::~Shader()
    {
        if (glfwGetCurrentContext()) {
            GLCall(glDeleteProgram(m_RendererID));
        }
    }

    ShaderProgramSource Shader::ParseShader(const std::string& filePath)
    {
        const std::string src = GetShaderSource(filePath);
        return ParseShaderSource(src);
    }

    unsigned int Shader::CompileShader(const unsigned int type, const std::string& source)
    {
        const unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        const GLint len = source.size();
        glShaderSource(id, 1, &src, &len);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            const auto message = static_cast<char *>(alloca(length * sizeof(char)));
            glGetShaderInfoLog(id, length, &length, message);
            // std::cout << "[OpenGL Shader Compilation]" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            //           << "shader compilation failed! Error :" << std::endl;
            // std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        const unsigned int program = glCreateProgram();
        const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    void Shader::release() const
    {
        Unbind();
        glDeleteProgram(m_RendererID);
    }

    void Shader::SetUniform1i(const std::string& name, int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform1f(const std::string& name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }


    int Shader::GetUniformLocation(const std::string& name) const
    {
        if (m_UniformLocationCache.contains(name))
            return m_UniformLocationCache[name];

        const int location = glGetUniformLocation(m_RendererID, name.c_str());

        if (location == -1) {
            std::cout << "[OpenGL Shader Uniform](" << m_FilePath << ") : Shader uniform " << name << " doesn't exist !" << std::endl;
        }
        else
            m_UniformLocationCache[name] = location;

        return location;
    }

    std::string Shader::GetShaderSource(const std::string &idOrPath) {
        if (idOrPath == "@embed/Shaders/Basic") {
            const auto&[data, size] = embedded::basicShader;

            return {
                reinterpret_cast<const char *>(data),
                size
            };
        }
        if (idOrPath == "@embed/Shaders/Texture") {
            const auto&[data, size] = embedded::textureShader;

            return {
                reinterpret_cast<const char *>(data),
                size
            };
        }
        if (idOrPath == "@embed/Shaders/Text") {
            const auto&[data, size] = embedded::textShader;

            return {
                reinterpret_cast<const char *>(data),
                size
            };
        }

        std::ifstream file(idOrPath, std::ios::binary);
        if (!file) throw std::runtime_error("[OpenGL Shader Processing] Could not open shader file: " + idOrPath);

        return {
            (std::istreambuf_iterator(file)),
            std::istreambuf_iterator<char>()
        };
    }

    ShaderProgramSource Shader::ParseShaderSource(const std::string &source) {
        std::istringstream stream(utils::StripUtf8Bom(source));

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

        return { ss[0].str(), ss[1].str() };
    }
}
