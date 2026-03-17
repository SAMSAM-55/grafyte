#include "Texture.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"
#include "macros.hpp"

#include "embedd/EmbeddedAsset.h"

namespace grafyte {
	Texture::~Texture()
	{
		release();
	}

	void Texture::Set(const std::string& path) {
		stbi_set_flip_vertically_on_load(1);
		loadDataToBuffer(path, m_LocalBuffer);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	void Texture::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture::Unbind()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::loadDataToBuffer(const std::string& idOrPath, unsigned char*& buffer)
	{
		if (idOrPath == "@embed/Textures/Default"
			|| idOrPath == "@embed/Textures/No")
		{
			buffer = stbi_load_from_memory(embedded::noTexture.data, embedded::noTexture.size, &m_Width, &m_Height,
				&m_BPP, 4);
			return;
		}

		buffer = stbi_load(idOrPath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	}

	void Texture::release() {
		if (glfwGetCurrentContext()) {
			Unbind();
			GLCall(glDeleteTextures(1, &m_RendererID));
		}
	}
}
