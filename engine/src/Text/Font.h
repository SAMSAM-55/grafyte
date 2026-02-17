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
	int atlasWidth, atlasHeight;
	std::unordered_map<char, Glyph> glyphs;
};