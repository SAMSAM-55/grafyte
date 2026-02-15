#pragma once

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"

namespace Grafyte
{
	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	private:
		std::string m_FilePath;
		unsigned int m_RendererID;
		// Caching for uniforms
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value) const;
		void SetUniform1f(const std::string& name, float value) const;
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

	private:
		ShaderProgramSource ParseShader(const std::string& filePath);
		unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name) const;
	};
}
