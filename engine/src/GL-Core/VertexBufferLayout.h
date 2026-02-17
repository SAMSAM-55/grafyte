#pragma once

#include <vector>
#include <glad/glad.h>
#include "macros.hpp"

namespace grafyte
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
		unsigned int offset;

		static unsigned int GetSizeOfType(const unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
				default: DEBUG_BREAK(); return 0;
			}
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		VertexBufferLayout()
			: m_Stride(0) {
		}

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(sizeof(T) == 0);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};

	template<>
	inline void VertexBufferLayout::Push<float>(const unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, static_cast<unsigned int>(count), GL_FALSE, m_Stride });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<>
	inline  void VertexBufferLayout::Push<unsigned int>(const unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE, m_Stride });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	template<>
	inline void VertexBufferLayout::Push<unsigned char>(const unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE, m_Stride });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
}
