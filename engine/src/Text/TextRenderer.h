#pragma once

#include "Font.h"

#include "GL-Core/Shader.h"
#include "glm/glm.hpp"

namespace Grafyte
{
	class TextRenderer
	{
	public:
		TextRenderer(const std::string& fontPath, int pixelSize);
		~TextRenderer();

		const Font& GetFont();

		float MeasureTextWidth(const std::string& text, float scale) const;

		void DrawText(const std::string& text, float x, float y, float scale,
			const glm::vec4& color, const glm::mat4& mvp) const;
	private:
		Font font;
		Shader shader;
		unsigned int vao, vbo;
	};
}
