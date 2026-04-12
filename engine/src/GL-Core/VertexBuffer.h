#pragma once

#include "glad/glad.h"

namespace grafyte
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, GLuint size, GLenum usage = GL_STATIC_DRAW);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept;

		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		void updateData(const void* data, GLuint size) const;

		void bind() const;
        static void unbind();

		void release();
	private:
		GLuint m_RendererID = 0;
	};
}
