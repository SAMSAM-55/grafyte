#pragma once

#include "GL-Core/IndexBuffer.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBuffer.h"
#include "GL-Core/VertexBufferLayout.h"
#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"

#include <string>

namespace Grafyte 
{
	struct ObjectRenderData
	{
		IndexBuffer ib;
		VertexArray va;
		VertexBuffer vb;
		VertexBufferLayout layout;
		Shader shader;
		Texture texture;
		bool hasTexture;
		unsigned int textureSlot;
	};

	class Object
	{
	private:

		std::vector<float> data; // Data in vertex buffer
		const unsigned int vertexCount;

		ObjectRenderData renderData;

		void UpdateVertexBuffer();

	public:
		Object(const void* positions, unsigned int positionsSize, unsigned int vertexCount, const unsigned int* indices, unsigned int indicesCount, const std::string& shaderSourcePath);
		~Object();

		void SetTexture(const std::string& textureSourcePath, unsigned int slot);
		void AddLayoutSlot(std::string type, unsigned int size);
		void AddBufferToVertexArray();

		/* Shader uniforms */
		void SetShaderUniform1f(const std::string& name, float value);
		void SetShaderUniform4f(const std::string& name, float float_x, float float_y, float float_z, float float_w);
		void SetShaderUniformMat4f(const std::string& name, const glm::mat4& matrix);

		/* Actual object functions */
		void Move(float offsetX, float offsetY);

		const ObjectRenderData& GetRenderData() { return renderData; }
	};

}