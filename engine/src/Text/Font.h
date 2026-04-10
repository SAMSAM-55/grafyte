#pragma once

#include <unordered_map>

#include <glad/glad.h>

struct Glyph
{
	float u0, v0, u1, v1; // Atlas uv
	int width, height;
	int bearingX, bearingY;
	int advance;
};

class Font 
{
public:
	GLuint textureID = 0;
	int bakedRes = 0;
	int atlasWidth = 0, atlasHeight = 0;
	std::unordered_map<char, Glyph> glyphs;

	// font metrics (FreeType)
	int ascent = 0;
	int descent = 0;
	int lineHeight = 0;

	static float TextScaleFromPt(const float pt, const float dpi, const int baked)
	{
		const float desired = pt * dpi / 72.0f;
		return desired / static_cast<float>(baked);
	}

	float MeasureTextHeight(const float scale, const float dpi, const int baked) const
	{
		const float finalScale = Font::TextScaleFromPt(scale, dpi, baked);
		return static_cast<float>(lineHeight) * finalScale;
	}
};
