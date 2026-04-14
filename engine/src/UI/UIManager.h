#pragma once
#include <memory>
#include <string>

#include "World/World.h"
#include "types.h"

namespace grafyte
{
using TextId = types::ObjectId;

namespace ui::text
{
enum class Anchor
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Center
};

struct Text
{
    TextId id;
    std::string text;
    types::Vec2 pos;
    float scale = 12.0f;
    types::Color4 color = {0.0f, 0.0f, 0.0f, 1.0f}; // default to black text
    Anchor anchor = Anchor::TopLeft;

    [[nodiscard]] TextId getId() const
    {
        return id;
    }
    void setText(const std::string &nText)
    {
        text = nText;
    }
    [[nodiscard]] std::string getText() const
    {
        return text;
    }
    void setScale(const float &nScale)
    {
        scale = nScale;
    }
    [[nodiscard]] float getScale() const
    {
        return scale;
    }
    [[nodiscard]] types::Color4 getColor() const
    {
        return color;
    }
    void setColor(const types::Color4 &nColor)
    {
        color = nColor;
    }
};
} // namespace ui::text

class UIManager
{
  public:
    explicit UIManager(std::shared_ptr<WorldContext> ctx, const types::UIId &id);

    std::shared_ptr<ui::text::Text> addText(const types::Vec2 &pos, const std::string &text, const float &scale = 1.0f,
                                            const ui::text::Anchor &anchor = ui::text::Anchor::TopLeft,
                                            const types::Color4 &color = {0.0f, 0.0f, 0.0f, 1.0f});
    void removeText(const TextId &id);

    void getTexts(std::vector<ui::text::Text> &out);

    void clear();

    [[nodiscard]] types::UIId getId() const
    {
        return m_Id;
    }

  private:
    [[nodiscard]] TextId composeTextId(const TextId localId) const
    {
        // allows for up to 255 UIs with roughly 16e6 texts each
        return (m_Id << 24U) | (localId & 0xFFFFFFU);
    }

    types::UIId m_Id = 0;
    std::unordered_map<TextId, std::shared_ptr<ui::text::Text>> m_Texts;
    TextId m_NextTextId = 0;
    std::shared_ptr<WorldContext> m_Ctx;
};
} // namespace grafyte
