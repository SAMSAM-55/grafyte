#include "UIManager.h"

#include <ranges>

namespace grafyte
{
UIManager::UIManager(std::shared_ptr<WorldContext> ctx, const types::UIId &id)
    : m_Id(id), m_Texts({}), m_Ctx(std::move(ctx))
{
}

std::shared_ptr<ui::text::Text> UIManager::addText(const types::Vec2 &pos, const std::string &text, const float &scale,
                                                   const ui::text::Anchor &anchor, const types::Color4 &color)
{
    m_NextTextId++;
    m_Texts.insert_or_assign(m_NextTextId,
                             std::make_shared<ui::text::Text>(m_NextTextId, text, pos, scale, color, anchor));
    return m_Texts.at(m_NextTextId);
}

void UIManager::removeText(const TextId &id)
{
    m_Texts.erase(id);
}

void UIManager::getTexts(std::vector<ui::text::Text> &out)
{
    out.clear();
    out.reserve(m_Texts.size());

    for (const auto &text : m_Texts | std::views::values)
        out.push_back(*text);
}

void UIManager::clear()
{
    m_Texts.clear();
    m_NextTextId = 0;
}
} // namespace grafyte
