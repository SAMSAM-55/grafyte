#pragma once

#include "glad/glad.h"

namespace grafyte
{
	class IndexBuffer
	{
	private:
		unsigned int m_RendererID{};
		unsigned int m_Count;

	public:
		IndexBuffer(const unsigned int* data, unsigned int count, unsigned int usage = GL_STATIC_DRAW);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer&& other) noexcept : m_RendererID(other.m_RendererID), m_Count(other.m_Count) {
			other.m_RendererID = 0;
			other.m_Count = 0;
		}

		IndexBuffer& operator=(IndexBuffer&& other) noexcept {
			if (this != &other) {
				release();
				m_RendererID = other.m_RendererID;
				m_Count = other.m_Count;
				other.m_RendererID = 0;
				other.m_Count = 0;
			}
			return *this;
		}

		void UpdateData(const unsigned int* data, unsigned int count);

		void Bind() const;
		void Unbind() const;
		void release();

		inline unsigned int GetCount() const { return m_Count; }
	};
}
