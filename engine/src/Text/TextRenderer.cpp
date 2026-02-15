#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <iostream>
#include <minmax.h>

namespace Grafyte
{
    TextRenderer::TextRenderer(const std::string& fontPath, int pixelSize)
        : shader("res/shaders/Text.shader"), vao(0), vbo(0)
    {
        // --- Load FreeType ---
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cout << "[FreeType] Failed to initialize\n";
            return;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cout << "[FreeType] Failed to load font\n";
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelSize);

        font.atlasWidth = 1024;
        font.atlasHeight = 256;
        std::vector<unsigned char> pixels(font.atlasWidth * font.atlasHeight, 0);

        int xOffset = 0;
        int yOffset = 0;
        int maxRowHeight = 0;

        const int first = 32;
        const int last = 126;

        for (unsigned char c = first; c <= last; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

            FT_GlyphSlot g = face->glyph;

            if (xOffset + g->bitmap.width >= font.atlasWidth) {
                xOffset = 0;
                yOffset += maxRowHeight;
                maxRowHeight = 0;
            }

            for (int y = 0; y < g->bitmap.rows; y++) {
                for (int x = 0; x < g->bitmap.width; x++) {
                    pixels[(yOffset + y) * font.atlasWidth + (xOffset + x)] =
                        g->bitmap.buffer[y * g->bitmap.pitch + x];
                }
            }

            Glyph glyph;
            glyph.u0 = xOffset / float(font.atlasWidth);
            glyph.v0 = (yOffset + g->bitmap.rows) / float(font.atlasHeight);
            glyph.u1 = (xOffset + g->bitmap.width) / float(font.atlasWidth);
            glyph.v1 = yOffset / float(font.atlasHeight);

            glyph.width = g->bitmap.width;
            glyph.height = g->bitmap.rows;
            glyph.bearingX = g->bitmap_left;
            glyph.bearingY = g->bitmap_top;
            glyph.advance = g->advance.x >> 6; // pixels

            font.glyphs[c] = glyph;

            xOffset += g->bitmap.width;
            maxRowHeight = max(maxRowHeight, g->bitmap.rows);
        }

        // Upload atlas
        glGenTextures(1, &font.textureID);
        glBindTexture(GL_TEXTURE_2D, font.textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            font.atlasWidth, font.atlasHeight, 0,
            GL_RED, GL_UNSIGNED_BYTE, pixels.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Setup VAO/VBO for quads
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    TextRenderer::~TextRenderer()
    {
    }

    const Font& TextRenderer::GetFont()
    {
        return font;
    }

    void TextRenderer::DrawText(const std::string& text,
        float x, float y,
        float scale,
        const glm::vec4& color,
        const glm::mat4& mvp)
    {
        shader.Bind();
        shader.SetUniform4f("u_TextColor", color.r, color.g, color.b, color.a);
        shader.SetUniformMat4f("u_MVP", mvp);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.textureID);
        shader.SetUniform1i("u_TextTexture", 0);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (char c : text) {
            if (!font.glyphs.count(c)) continue;

            const Glyph& g = font.glyphs.at(c);

            float xpos = x + g.bearingX * scale;
            float ypos = y - (g.height - g.bearingY) * scale;

            float w = g.width * scale;
            float h = g.height * scale;

            float vertices[] = {

                xpos,     ypos + h, g.u0, g.v1,
                xpos + w, ypos,     g.u1, g.v0,
                xpos + w, ypos + h, g.u1, g.v1,

                xpos,     ypos + h, g.u0, g.v1,
                xpos,     ypos,     g.u0, g.v0,
                xpos + w, ypos,     g.u1, g.v0,
            };

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += g.advance * scale; // Move cursor
        }
    }

    float TextRenderer::MeasureTextWidth(const std::string& text, float scale) const
    {
        float width = 0.0f;

        for (char c : text) {
            if (!font.glyphs.count(c))
                continue;

            const Glyph& g = font.glyphs.at(c);
            width += g.advance * scale;
        }

        return width;
    }
}
