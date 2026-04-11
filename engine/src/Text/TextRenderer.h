#pragma once

#include "Font.h"

#include <ft2build.h>

#include "types.h"

#include FT_FREETYPE_H

#include "GL-Core/Shader.h"
#include "UI/UIManager.h"
#include "World/Camera.h"
#include "glm/glm.hpp"

namespace grafyte
{
constexpr int PAD = 1;

class TextRenderer
{
  public:
    TextRenderer(const std::string &fontPath, int pixelSize);
    ~TextRenderer();

    void setDpi(const types::Vec2 &dpi)
    {
        m_Dpi = dpi;
    };

    const Font &getFont();

    float measureTextWidth(const std::string &text, float scale) const;

    void drawText(const std::string &text, float x, float y, float scale, const types::Color4 &color,
                  const types::Vec2 &windowDimensions) const;

    void drawTextObject(const std::string &text, float x, float y, float scale, const types::Color4 &color,
                        const Camera *camera) const;

    void renderTextObjects(const std::vector<types::TextData> &renderList, const Camera *camera) const;
    void renderTexts(const std::vector<ui::text::Text> &texts, const types::Vec2 &windowDimensions) const;

    void render(const std::vector<types::TextData> &objects, const std::vector<ui::text::Text> &texts,
                const Camera *camera, const types::Vec2 &windowDimensions) const;

  private:
    Font m_Font;
    Shader m_Shader;
    GLuint m_Vao, m_Vbo;
    types::Vec2 m_Dpi = {0.0f, 0.0f};

    static void initFaceFromSource(const std::string &idOrPath, const FT_Library &library, FT_Face *face);
    types::Vec2 getAnchoredPosition(const types::Vec2 &pos, const types::Vec2 &windowDimensions,
                                    const ui::text::Anchor &anchor, const std::string &text, float scale) const;
    void draw(const std::string& text, float x, float y, float scale) const;
};
} // namespace grafyte
