#include "VertexArray.h"

#include "VertexBufferLayout.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "macros.hpp"

namespace Grafyte
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

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
			                             reinterpret_cast<const void *>(offset)));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
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
