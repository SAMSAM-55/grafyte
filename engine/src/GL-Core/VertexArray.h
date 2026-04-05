#pragma once

#include "VertexBuffer.h"

namespace grafyte
{
	class VertexBufferLayout;

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&& other) noexcept : m_RendererID(other.m_RendererID) {
			other.m_RendererID = 0;
		}

		VertexArray& operator=(VertexArray&& other) noexcept {
			if (this != &other) {
				release();
				m_RendererID = other.m_RendererID;
				other.m_RendererID = 0;
			}
			return *this;
		}

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

		void Bind() const;
		void Unbind() const;
		void release();


	private:
		unsigned int m_RendererID = 0;
	};
}
