#pragma once

#include "Font.h"

#include <ft2build.h>

#include "types.h"

#include FT_FREETYPE_H

#include "GL-Core/Shader.h"
#include "glm/glm.hpp"
#include "World/Camera.h"

namespace grafyte
{
	constexpr int PAD = 1;

	class TextRenderer
	{
	public:
		TextRenderer(const std::string& fontPath, int pixelSize);
		~TextRenderer();

		void SetDpi(const types::Vec2& dpi) {m_dpi = dpi;};

		const Font& GetFont();

		float MeasureTextWidth(const std::string& text, float scale) const;

		void DrawText(const std::string& text, float x, float y, float scale,
		              const glm::vec4& color, const types::Vec2& windowDimensions) const;

		void DrawTextObject(const std::string& text, float x, float y, float scale,
					  const glm::vec4& color, Camera* camera) const;

		void Render(const std::vector<types::TextData>& renderList, Camera* camera) const;
	private:
		Font font;
		Shader shader;
		unsigned int vao, vbo;
		types::Vec2 m_dpi = {0.0f, 0.0f};

		static void InitFaceFromSource(const std::string& idOrPath, const FT_Library& library, FT_Face* face);
	};
}
