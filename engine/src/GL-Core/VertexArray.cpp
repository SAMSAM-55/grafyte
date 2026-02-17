#include "VertexArray.h"

#include "VertexBufferLayout.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "macros.hpp"

namespace grafyte
{
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	VertexArray::~VertexArray()
	{
		if (glfwGetCurrentContext()) {
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			if (element.type == GL_UNSIGNED_INT || element.type == GL_INT) {
				GLCall(glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(),
					reinterpret_cast<void *>(static_cast<uintptr_t>(element.offset))));
			} else {
				GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
					reinterpret_cast<void *>(static_cast<uintptr_t>(element.offset))));
			}
		}
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}
