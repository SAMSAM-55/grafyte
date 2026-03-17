#pragma once

#include <string>

namespace grafyte {
	class Texture {
	public:
		Texture() = default;
		~Texture();

		Texture(Texture const&) = delete;
		Texture& operator=(Texture const&) = delete;

		Texture(Texture&& other) noexcept
			: m_RendererID(other.m_RendererID),
			  m_FilePath(std::move(other.m_FilePath)),
			  m_LocalBuffer(other.m_LocalBuffer),
			  m_Width(other.m_Width),
			  m_Height(other.m_Height),
			  m_BPP(other.m_BPP)
		{
			other.m_RendererID = 0;
			other.m_LocalBuffer = nullptr;
			other.m_Width = other.m_Height = other.m_BPP = 0;
		}

		Texture& operator=(Texture&& other) noexcept {
			if (this == &other) return *this;

			release();

			m_RendererID  = other.m_RendererID;
			m_FilePath    = std::move(other.m_FilePath);
			m_LocalBuffer = other.m_LocalBuffer;
			m_Width       = other.m_Width;
			m_Height      = other.m_Height;
			m_BPP         = other.m_BPP;

			other.m_RendererID = 0;
			other.m_LocalBuffer = nullptr;
			other.m_Width = other.m_Height = other.m_BPP = 0;
			return *this;
		}

		void Set(const std::string& path);
		void Bind(unsigned int slot) const;
		static void Unbind();
	private:
		void loadDataToBuffer(const std::string& idOrPath, unsigned char*& buffer);
		void release();
		unsigned int m_RendererID = 0;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer = nullptr;
		int m_Width = 0, m_Height = 0, m_BPP = 0;
	};
}
