#include "Object.h"

#include <cstring>
#include <iostream>
#include <GLFW/glfw3.h>

namespace Grafyte 
{
	Object::Object(const void *positions, const unsigned int positionsSize, const unsigned int vertexCount,
	               const unsigned int *indices, const unsigned int indicesCount, const std::string &shaderSourcePath,
	               const float pos_x, const float pos_y, const int layer)
		: m_vertexCount(vertexCount), m_pos{pos_x, pos_y},
		  m_renderData(ObjectRenderData{
			  IndexBuffer(indices, indicesCount), VertexArray(), VertexBuffer(positions, positionsSize),
			  VertexBufferLayout(), Shader(shaderSourcePath), Texture(), false, 0, layer
		  })
	{

		m_bytes.resize(positionsSize);
		m_bytesRel.resize(positionsSize);
		std::memcpy(m_bytesRel.data(), positions, positionsSize);
		std::memcpy(m_bytes.data(), m_bytesRel.data(), positionsSize);
	}

	Object::~Object() {
		if (!glfwGetCurrentContext()) {
			return;
		}

		m_renderData.va.Unbind();
		m_renderData.ib.Unbind();
		m_renderData.vb.Unbind();
		m_renderData.shader.Unbind();
		m_renderData.texture.Unbind();
	}

	void Object::SetTexture(const std::string& textureSourcePath, unsigned int slot) {
		m_renderData.shader.Bind();
		m_renderData.texture.Set(textureSourcePath);
		m_renderData.texture.Bind(slot);
		m_renderData.shader.SetUniform1i("u_Texture", slot);
		m_renderData.hasTexture = true;
		m_renderData.textureSlot = slot;
	}

	void Object::AddLayoutSlot(const std::string &type, const unsigned int size) {
		if (type == "float")
			m_renderData.layout.Push<float>(size);
		else if (type == "unsigned int")
			m_renderData.layout.Push<unsigned int>(size);
		else if (type == "unsigned char")
			m_renderData.layout.Push<unsigned char>(size);
		else
			DEBUG_BREAK();
	}

	void Object::AddBufferToVertexArray() {
		m_renderData.va.Bind();
		m_renderData.va.AddBuffer(m_renderData.vb, m_renderData.layout);

		m_strideBytes = m_renderData.layout.GetStride();

		m_posOffsetBytes = 0;
		bool found = false;
		for (const auto& e : m_renderData.layout.GetElements()) {
			if (e.type == GL_FLOAT && e.count >= 2) { m_posOffsetBytes = e.offset; found = true; break; }
		}
		if (!found) throw std::runtime_error("No float2 position attribute in layout");

		m_finalized = true;

		UpdatePosition();
	}

	void Object::SetShaderUniform1f(const std::string& name, const float value) const {
		m_renderData.shader.Bind();
		m_renderData.shader.SetUniform1f(name, value);
	}

	void Object::SetShaderUniform4f(const std::string& name, const float x, const float y, const float z, const float w) const {
		m_renderData.shader.Bind();
		m_renderData.shader.SetUniform4f(name, x, y, z, w);
	}

	void Object::SetShaderUniformMat4f(const std::string& name, const glm::mat4& matrix) const {
		m_renderData.shader.Bind();
		m_renderData.shader.SetUniformMat4f(name, matrix);
	}

	void Object::Move(const float offsetX, const float offsetY) {
		EnsureReady();
		m_pos[0] += offsetX;
		m_pos[1] += offsetY;

		UpdatePosition();
	}

	void Object::MoveTo(const float posX, const float posY) {
		EnsureReady();
		m_pos[0] = posX;
		m_pos[1] = posY;

		UpdatePosition();
	}


	void Object::UpdatePosition() {
		EnsureReady();

		for (unsigned i = 0; i < m_vertexCount; ++i) {
			const auto* src = reinterpret_cast<const float*>(m_bytesRel.data() + i*m_strideBytes + m_posOffsetBytes);
			auto* dst = reinterpret_cast<float*>(m_bytes.data() + i*m_strideBytes + m_posOffsetBytes);
			dst[0] = src[0] + m_pos[0];
			dst[1] = src[1] + m_pos[1];
		}


		UpdateVertexBuffer();
	}

	void Object::EnsureReady() const {
		if (m_strideBytes == 0)
			throw std::runtime_error("Invalid stride.");
		if (!m_finalized)
			throw std::runtime_error("Object not finalized. Call AddBufferToVertexArray() first.");
	}

	void Object::UpdateVertexBuffer() const {
		m_renderData.vb.Bind();
		glBufferData(GL_ARRAY_BUFFER, m_bytes.size(), m_bytes.data(), GL_DYNAMIC_DRAW);
	}

}
