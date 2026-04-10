#include "TextRenderer.h"

#include <vector>
#include <iostream>
#include <stdexcept>

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>

#include "embedd/EmbeddedAsset.h"
#include "High/Application.h"
#include "GlContextState.h"

namespace grafyte
{
    TextRenderer::TextRenderer(const std::string& fontPath, const int pixelSize)
        : shader("@embed/Shaders/Text"), vao(0), vbo(0)
    {
        // --- Load FreeType ---
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("[TextRenderer] Could not initialize FreeType.");
        }

        FT_Face face;
        InitFaceFromSource(fontPath, ft, &face);

        FT_Set_Pixel_Sizes(face, 0, pixelSize);

        font.atlasWidth = 2048;
        font.atlasHeight = 2048;
        font.bakedRes = pixelSize;
        font.ascent = face->size->metrics.ascender >> 6;
        font.descent = face->size->metrics.descender >> 6;
        font.lineHeight = face->size->metrics.height >> 6;
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

            if (yOffset + PAD + g->bitmap.rows > font.atlasHeight) {
                throw std::runtime_error("[TextRenderer] Font atlas is too small for the requested glyph set.");
            }

            for (int y = 0; y < g->bitmap.rows; y++) {
                for (int x = 0; x < g->bitmap.width; x++) {
                    const int dstY = yOffset + PAD + y;
                    const int dstX = xOffset + PAD + x;
                    if (dstX < 0 || dstX >= font.atlasWidth || dstY < 0 || dstY >= font.atlasHeight) {
                        throw std::runtime_error("[TextRenderer] Font atlas write would exceed allocated bounds.");
                    }
                    pixels[(yOffset + PAD + y) * font.atlasWidth + (xOffset + PAD + x)] =
                        g->bitmap.buffer[y * g->bitmap.pitch + x];
                }
            }

            Glyph glyph;
            glyph.u0 = (xOffset + PAD) / static_cast<float>(font.atlasWidth);
            glyph.v0 = (yOffset + PAD + g->bitmap.rows) / static_cast<float>(font.atlasHeight);
            glyph.u1 = (xOffset + PAD + g->bitmap.width) / static_cast<float>(font.atlasWidth);
            glyph.v1 = (yOffset + PAD) / static_cast<float>(font.atlasHeight);

            glyph.width = g->bitmap.width;
            glyph.height = g->bitmap.rows;
            glyph.bearingX = g->bitmap_left;
            glyph.bearingY = g->bitmap_top;
            glyph.advance = g->advance.x >> 6; // pixels

            font.glyphs[c] = glyph;

            xOffset += g->bitmap.width + PAD*2;
            maxRowHeight = std::max(maxRowHeight, static_cast<int>(g->bitmap.rows) + PAD*2);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Setup VAO/VBO for quads
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, static_cast<void *>(nullptr));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(sizeof(float) * 2));

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    TextRenderer::~TextRenderer() {
        if (GlContextAlive()) {
            if (font.textureID) glDeleteTextures(1, &font.textureID);
            if (vbo) glDeleteBuffers(1, &vbo);
            if (vao) glDeleteVertexArrays(1, &vao);
        }
    }

    const Font& TextRenderer::GetFont()
    {
        return font;
    }

    void TextRenderer::DrawText(const std::string& text,
        float x, const float y,
        const float scale,
        const types::Color4& color,
        const types::Vec2& windowDimensions) const {

        const float finalScale = Font::TextScaleFromPt(scale, m_dpi.x, font.bakedRes);

        const glm::mat4 uiProj = glm::ortho(
        0.0f, windowDimensions.x,
        0.0f, windowDimensions.y
        );
        constexpr auto uiView = glm::mat4(1.0f);
        const glm::mat4 mvp = uiProj * uiView;

        shader.Bind();
        shader.SetUniform4f("u_TextColor", color.x, color.y, color.z, color.w);
        shader.SetUniformMat4f("u_MVP", mvp);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.textureID);
        shader.SetUniform1i("u_TextTexture", 0);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (char c : text) {
            if (!font.glyphs.count(c)) continue;

            const Glyph& g = font.glyphs.at(c);

            float xpos = x + g.bearingX * finalScale;
            float ypos = y - (g.height - g.bearingY) * finalScale;

            float w = g.width * finalScale;
            float h = g.height * finalScale;

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

            x += g.advance * finalScale; // Move cursor
        }
    }

    void TextRenderer::DrawTextObject(const ::std::string &text, float x, const float y, const float scale,
                                      const types::Color4 & color,
                                      const Camera *camera
    ) const {

        const glm::mat4 mvp = camera->projection * camera->view;

        shader.Bind();
        shader.SetUniform4f("u_TextColor", color.x, color.y, color.z, color.w);
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

    void TextRenderer::RenderTextObjects(const std::vector<types::TextData>& renderList, Camera* camera) const
    {
        for (const auto& text: renderList)
        {
            const float finalScale = Font::TextScaleFromPt(text.transform.scale.x, m_dpi.x, font.bakedRes);
            DrawTextObject(
                text.text,
                text.transform.pos.x,
                text.transform.pos.y,
                finalScale,
                text.color,
                camera
            );
        }
    }

    void TextRenderer::RenderTexts(const std::vector<ui::text::Text>& texts, const types::Vec2& windowDimensions) const
    {
        for (const auto& text: texts)
        {
            const types::Vec2 finalPos = getAnchoredPosition(text.pos, windowDimensions, text.anchor, text.text, text.scale);
            DrawText(text.text, finalPos.x, finalPos.y, text.scale, text.color, windowDimensions);
        }
    }

    void TextRenderer::render(const std::vector<types::TextData>& objects, const std::vector<ui::text::Text>& texts,
                              Camera* camera, const types::Vec2& windowDimensions) const
    {
        RenderTextObjects(objects, camera);
        RenderTexts(texts, windowDimensions);
    }

    void TextRenderer::InitFaceFromSource(const std::string &idOrPath, const FT_Library& library, FT_Face* face) {
        const bool is_embedded = (
            idOrPath == "@embed/Font/Default"
            || idOrPath == "@embed/Font/Base"
            || idOrPath == "@embed/Fonts/Default"
            || idOrPath == "@embed/Fonts/Base"
            );

        if (is_embedded) {
            if (const auto [data, size] = embedded::baseFont;
                FT_New_Memory_Face(library, data, size, 0, face)) {
                throw std::runtime_error("[Freetype Face] Could not initialize the freetype face");
            }
            return;
        }

        if (const FT_Error error = FT_New_Face(library, idOrPath.c_str(), 0, face)) {
            std::cout << "FreeType error code: " << error << std::endl;
            std::cout << "Font path was: " << idOrPath << std::endl;
            throw std::runtime_error("[Freetype Face] Could not initialize the freetype face");
        }
    }

    types::Vec2 TextRenderer::getAnchoredPosition(
    const types::Vec2& pos,
    const types::Vec2& windowDimensions,
    const ui::text::Anchor& anchor,
    const std::string& text,
    const float scale
    ) const {
        const float width  = MeasureTextWidth(text, scale);
        const float height = font.MeasureTextHeight(scale, m_dpi.x, font.bakedRes);

        switch (anchor)
        {
        case ui::text::Anchor::BottomLeft:
            return { pos.x, pos.y };

        case ui::text::Anchor::BottomRight:
            return { windowDimensions.x - pos.x - width,
                     pos.y };

        case ui::text::Anchor::TopLeft:
            return { pos.x,
                     windowDimensions.y - pos.y - height};

        case ui::text::Anchor::TopRight:
            return { windowDimensions.x - pos.x - width,
                     windowDimensions.y - pos.y - height};

        case ui::text::Anchor::Center:
            return {
                (windowDimensions.x - width)  * 0.5f + pos.x,
                (windowDimensions.y - height) * 0.5f + pos.y
            };

        default:
            return pos;
        }
    }

    float TextRenderer::MeasureTextWidth(const std::string& text, float scale) const
    {
        float width = 0.0f;

        for (char c : text) {
            if (!font.glyphs.count(c))
                continue;

            const Glyph& g = font.glyphs.at(c);
            width += g.advance * Font::TextScaleFromPt(scale, m_dpi.x, font.bakedRes);
        }

        return width;
    }
}
