#pragma once

#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "GL-Core/Shader.h"
#include "glm/glm.hpp"

namespace grafyte
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

		static void InitFaceFromSource(const std::string& idOrPath, const FT_Library& library, FT_Face* face);
	};
}
