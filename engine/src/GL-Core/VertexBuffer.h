#pragma once

#include "glad/glad.h"

namespace grafyte
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept : m_RendererID(other.m_RendererID) {
			other.m_RendererID = 0;
		}

		VertexBuffer& operator=(VertexBuffer&& other) noexcept {
			if (this != &other) {
				release();
				m_RendererID = other.m_RendererID;
				other.m_RendererID = 0;
			}
			return *this;
		}

		void UpdateData(const void* data, unsigned int size) const;

		void Bind() const;
		void Unbind() const;
		void release();


	private:
		unsigned int m_RendererID = 0;
	};
}
