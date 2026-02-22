#pragma once

namespace grafyte
{
	class VertexBuffer
	{
	private:
		unsigned int m_RendererID;

	public:
		VertexBuffer(const void* data, unsigned int size);
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

		void Bind() const;
		void Unbind() const;
		void release();
	};
}
