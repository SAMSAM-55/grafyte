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
	GLuint textureID;
	int bakedRes;
	int atlasWidth, atlasHeight;
	std::unordered_map<char, Glyph> glyphs;

	static float TextScaleFromPt(const float pt, const float dpi, const int baked)
	{
		const float desired = pt * dpi / 72.0f;
		return desired / static_cast<float>(baked);
	}
};