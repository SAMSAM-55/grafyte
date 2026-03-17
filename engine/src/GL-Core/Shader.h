#pragma once

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"

namespace grafyte
{
	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	public:
		Shader(Shader const&) = delete;
		Shader& operator=(Shader const&) = delete;

		Shader(Shader&& other) noexcept
		: m_FilePath(std::move(other.m_FilePath)), m_RendererID(other.m_RendererID),
		  m_UniformLocationCache(std::move(other.m_UniformLocationCache))
		{
			other.m_RendererID = 0;
			other.m_UniformLocationCache = std::unordered_map<std::string, int>();
			other.m_FilePath = "";
		};
		Shader& operator=(Shader&& other) noexcept
		{
			if (this == &other) return *this;

			release();

			m_FilePath = std::move(other.m_FilePath);
			m_RendererID = other.m_RendererID;
			m_UniformLocationCache = std::move(other.m_UniformLocationCache);

			other.m_RendererID = 0;
			other.m_UniformLocationCache = std::unordered_map<std::string, int>();
			other.m_FilePath = "";
			return *this;
		}

		explicit Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		static void Unbind();
		void release() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value) const;
		void SetUniform1f(const std::string& name, float value) const;
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

	private:
		enum class ShaderType {
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1
		};

		std::string m_FilePath;
		unsigned int m_RendererID;
		// Caching for uniforms
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

		static ShaderProgramSource ParseShader(const std::string& filePath);
		static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

		static unsigned int CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name) const;

		static std::string GetShaderSource(const std::string& idOrPath);
		static ShaderProgramSource ParseShaderSource(const std::string& source);
	};
}
