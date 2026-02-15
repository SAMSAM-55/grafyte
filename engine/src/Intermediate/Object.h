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
		int zIndex;
	};

	class Object
	{
	public:
		Object(const void *positions, unsigned int positionsSize, unsigned int vertexCount, const unsigned int *indices,
		       unsigned int indicesCount, const std::string &shaderSourcePath, float pos_x, float pos_y, int layer = 0);
		~Object();

		void SetTexture(const std::string& textureSourcePath, unsigned int slot);
		void AddLayoutSlot(const std::string &type, unsigned int size);
		void AddBufferToVertexArray();

		/* Shader uniforms */
		void SetShaderUniform1f(const std::string& name, float value) const;
		void SetShaderUniform4f(const std::string& name, float x, float y, float z, float w) const;
		void SetShaderUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

		/* Actual object functions */
		void Move(float offsetX, float offsetY);
		void MoveTo(float posX, float posY);

		const ObjectRenderData& GetRenderData() { return m_renderData; }

	private:

		std::vector<uint8_t> m_bytes;
		std::vector<uint8_t> m_bytesRel;
		unsigned int m_strideBytes = 0;
		unsigned int m_posOffsetBytes = 0;

		const unsigned int m_vertexCount;
		float m_pos[2];

		ObjectRenderData m_renderData;
		bool m_finalized = false;

		void UpdatePosition();
		void EnsureReady() const;
		void UpdateVertexBuffer() const;
	};

}