#include "TextRenderer.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>

#include "GlContextState.h"
#include "High/Application.h"
#include "embedd/EmbeddedAsset.h"
#include "macros.hpp"

namespace grafyte
{
TextRenderer::TextRenderer(const std::string &fontPath, const int pixelSize)
    : m_Shader("@embed/Shaders/Text"), m_Vao(0), m_Vbo(0)
{
    // --- Load FreeType ---
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        throw std::runtime_error("[TextRenderer] Could not initialize FreeType.");
    }

    FT_Face face;
    initFaceFromSource(fontPath, ft, &face);

    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    m_Font.atlasWidth = 2048;
    m_Font.atlasHeight = 2048;
    m_Font.bakedRes = pixelSize;
    m_Font.ascent = face->size->metrics.ascender >> 6;
    m_Font.descent = face->size->metrics.descender >> 6;
    m_Font.lineHeight = face->size->metrics.height >> 6;
    std::vector<unsigned char> pixels(m_Font.atlasWidth * m_Font.atlasHeight, 0);

    int xOffset = 0;
    int yOffset = 0;
    int maxRowHeight = 0;

    constexpr int first = 32;
    constexpr int last = 126;

    for (unsigned char c = first; c <= last; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        FT_GlyphSlot g = face->glyph;

        if (xOffset + g->bitmap.width >= m_Font.atlasWidth)
        {
            xOffset = 0;
            yOffset += maxRowHeight;
            maxRowHeight = 0;
        }

        if (yOffset + PAD + g->bitmap.rows > m_Font.atlasHeight)
        {
            throw std::runtime_error("[TextRenderer] Font atlas is too small for the requested glyph set.");
        }

        for (int y = 0; y < g->bitmap.rows; y++)
        {
            for (int x = 0; x < g->bitmap.width; x++)
            {
                const int dstY = yOffset + PAD + y;
                if (const int dstX = xOffset + PAD + x;
                    dstX < 0 || dstX >= m_Font.atlasWidth || dstY < 0 || dstY >= m_Font.atlasHeight)
                {
                    throw std::runtime_error("[TextRenderer] Font atlas write would exceed allocated bounds.");
                }
                pixels[(yOffset + PAD + y) * m_Font.atlasWidth + (xOffset + PAD + x)] =
                    g->bitmap.buffer[y * g->bitmap.pitch + x];
            }
        }

        Glyph glyph{};
        glyph.u0 = static_cast<float>(xOffset + PAD) / static_cast<float>(m_Font.atlasWidth);
        glyph.v0 = static_cast<float>(yOffset + PAD + g->bitmap.rows) / static_cast<float>(m_Font.atlasHeight);
        glyph.u1 = static_cast<float>(xOffset + PAD + g->bitmap.width) / static_cast<float>(m_Font.atlasWidth);
        glyph.v1 = static_cast<float>(yOffset + PAD) / static_cast<float>(m_Font.atlasHeight);

        glyph.width = static_cast<int>(g->bitmap.width);
        glyph.height = static_cast<int>(g->bitmap.rows);
        glyph.bearingX = g->bitmap_left;
        glyph.bearingY = g->bitmap_top;
        glyph.advance = g->advance.x >> 6; // pixels

        m_Font.glyphs[static_cast<char>(c)] = glyph;

        xOffset += static_cast<int>(g->bitmap.width + PAD * 2);
        maxRowHeight = std::max(maxRowHeight, static_cast<int>(g->bitmap.rows) + PAD * 2);
    }

    // Upload atlas
    GL_CALL(glGenTextures(1, &m_Font.textureID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Font.textureID));
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Font.atlasWidth, m_Font.atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE,
                         pixels.data()));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Setup VAO/VBO for quads
    GL_CALL(glGenVertexArrays(1, &m_Vao));
    GL_CALL(glGenBuffers(1, &m_Vbo));

    GL_CALL(glBindVertexArray(m_Vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW));

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, static_cast<void *>(nullptr)));

    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                                  reinterpret_cast<void *>(sizeof(float) * 2)));

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

TextRenderer::~TextRenderer()
{
    if (glContextAlive())
    {
        if (m_Font.textureID)
        {
            GL_CALL(glDeleteTextures(1, &m_Font.textureID));
        }
        if (m_Vbo)
        {
            GL_CALL(glDeleteBuffers(1, &m_Vbo));
        }
        if (m_Vao)
        {
            GL_CALL(glDeleteVertexArrays(1, &m_Vao));
        }
    }
}

const Font &TextRenderer::getFont()
{
    return m_Font;
}

// ==================================================
// [SECTION] Rendering
// ==================================================

void TextRenderer::drawText(const std::string &text, const float x, const float y, const float scale,
                            const types::Color4 &color, const types::Vec2 &windowDimensions) const
{

    const float finalScale = Font::textScaleFromPt(scale, m_Dpi.x, m_Font.bakedRes);

    const glm::mat4 uiProj = glm::ortho(0.0f, windowDimensions.x, 0.0f, windowDimensions.y);
    constexpr auto uiView = glm::mat4(1.0f);
    const glm::mat4 mvp = uiProj * uiView;

    m_Shader.bind();
    m_Shader.setUniform4f("u_TextColor", color.x, color.y, color.z, color.w);
    m_Shader.setUniformMat4f("u_MVP", mvp);

    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Font.textureID));
    m_Shader.setUniform1i("u_TextTexture", 0);

    GL_CALL(glBindVertexArray(m_Vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));

    draw(text, x, y, finalScale);
}

void TextRenderer::drawTextObject(const ::std::string &text, float x, const float y, const float scale,
                                  const types::Color4 &color, const Camera *camera) const
{

    const glm::mat4 mvp = camera->projection * camera->view;

    m_Shader.bind();
    m_Shader.setUniform4f("u_TextColor", color.x, color.y, color.z, color.w);
    m_Shader.setUniformMat4f("u_MVP", mvp);

    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Font.textureID));
    m_Shader.setUniform1i("u_TextTexture", 0);

    GL_CALL(glBindVertexArray(m_Vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));

    draw(text, x, y, scale);
}

void TextRenderer::renderTextObjects(const std::vector<types::TextData> &renderList, const Camera *camera) const
{
    for (const auto &text : renderList)
    {
        const float finalScale = Font::textScaleFromPt(text.transform.scale.x, m_Dpi.x, m_Font.bakedRes);
        drawTextObject(text.text, text.transform.pos.x, text.transform.pos.y, finalScale, text.color, camera);
    }
}

void TextRenderer::renderTexts(const std::vector<ui::text::Text> &texts, const types::Vec2 &windowDimensions) const
{
    for (const auto &text : texts)
    {
        const types::Vec2 finalPos =
            getAnchoredPosition(text.pos, windowDimensions, text.anchor, text.text, text.scale);
        drawText(text.text, finalPos.x, finalPos.y, text.scale, text.color, windowDimensions);
    }
}

void TextRenderer::render(const std::vector<types::TextData> &objects, const std::vector<ui::text::Text> &texts,
                          const Camera *camera, const types::Vec2 &windowDimensions) const
{
    renderTextObjects(objects, camera);
    renderTexts(texts, windowDimensions);
}

// FreeType loading

void TextRenderer::initFaceFromSource(const std::string &idOrPath, const FT_Library &library, FT_Face *face)
{
    const bool isEmbedded = (idOrPath == "@embed/Font/Default" || idOrPath == "@embed/Font/Base" ||
                             idOrPath == "@embed/Fonts/Default" || idOrPath == "@embed/Fonts/Base");

    if (isEmbedded)
    {
        if (const auto [data, size] = embedded::BaseFont;
            FT_New_Memory_Face(library, data, static_cast<FT_Long>(size), 0, face))
        {
            throw std::runtime_error("[Freetype Face] Could not initialize the freetype face");
        }
        return;
    }

    if (const FT_Error error = FT_New_Face(library, idOrPath.c_str(), 0, face))
    {
        std::cout << "FreeType error code: " << error << std::endl;
        std::cout << "Font path was: " << idOrPath << std::endl;
        throw std::runtime_error("[Freetype Face] Could not initialize the freetype face");
    }
}

// ==================================================
// [SECTION] Helpers
// ==================================================

types::Vec2 TextRenderer::getAnchoredPosition(const types::Vec2 &pos, const types::Vec2 &windowDimensions,
                                              const ui::text::Anchor &anchor, const std::string &text,
                                              const float scale) const
{
    const float width = measureTextWidth(text, scale);
    const float height = m_Font.measureTextHeight(scale, m_Dpi.x, m_Font.bakedRes);

    switch (anchor)
    {
    case ui::text::Anchor::BottomLeft:
        return {pos.x, pos.y};

    case ui::text::Anchor::BottomRight:
        return {windowDimensions.x - pos.x - width, pos.y};

    case ui::text::Anchor::TopLeft:
        return {pos.x, windowDimensions.y - pos.y - height};

    case ui::text::Anchor::TopRight:
        return {windowDimensions.x - pos.x - width, windowDimensions.y - pos.y - height};

    case ui::text::Anchor::Center:
        return {(windowDimensions.x - width) * 0.5f + pos.x, (windowDimensions.y - height) * 0.5f + pos.y};

    default:
        return pos;
    }
}
void TextRenderer::draw(const std::string &text, float x, float y, float scale) const
{
    for (char c : text)
    {
        if (!m_Font.glyphs.contains(c))
            continue;

        const Glyph &g = m_Font.glyphs.at(c);

        const float xPos = x + static_cast<float>(g.bearingX) * scale;
        const float yPos = y - static_cast<float>(g.height - g.bearingY) * scale;

        const float w = static_cast<float>(g.width) * scale;
        const float h = static_cast<float>(g.height) * scale;

        const float vertices[] = {

            xPos, yPos + h, g.u0, g.v1, xPos + w, yPos, g.u1, g.v0, xPos + w, yPos + h, g.u1, g.v1,

            xPos, yPos + h, g.u0, g.v1, xPos,     yPos, g.u0, g.v0, xPos + w, yPos,     g.u1, g.v0,
        };

        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

        x += static_cast<float>(g.advance) * scale; // Move cursor
    }
}

float TextRenderer::measureTextWidth(const std::string &text, float scale) const
{
    float width = 0.0f;

    for (char c : text)
    {
        if (!m_Font.glyphs.contains(c))
            continue;

        const Glyph &g = m_Font.glyphs.at(c);
        width += static_cast<float>(g.advance) * Font::textScaleFromPt(scale, m_Dpi.x, m_Font.bakedRes);
    }

    return width;
}
} // namespace grafyte
