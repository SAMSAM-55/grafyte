#pragma once

#include <string>

namespace Grafyte
{
	class Texture
	{
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	public:
		Texture();
		~Texture();

		void Set(const std::string& path);
		void Bind(unsigned int slot = 0) const;
		void Unbind();

		inline int GetWidth() const { return m_Width; };
		inline int GetHeight() const { return m_Height; };
	};
}
