#include "UIManager.h"

#include <ranges>

namespace grafyte
{
    UIManager::UIManager(std::shared_ptr<WorldContext> ctx)
        : m_texts({}), m_ctx(std::move(ctx))
    {
    }

    std::shared_ptr<ui::text::Text> UIManager::addText(const types::Vec2& pos,
        const std::string& text,
        const float& scale,
        const ui::text::Anchor& anchor,
        const types::Color4& color)
    {
        m_nextTextId++;
        m_texts.insert_or_assign(m_nextTextId,
                                 std::make_shared<ui::text::Text>(m_nextTextId, text, pos, scale, color, anchor)
                                 );
        return m_texts.at(m_nextTextId);
    }

    void UIManager::removeText(const TextId& id)
    {
        m_texts.erase(id);
    }

    void UIManager::getTexts(std::vector<ui::text::Text>& out)
    {
        out.clear();
        out.reserve(m_texts.size());

        for (const auto& text : m_texts | std::views::values) out.push_back(*text);
    }

    void UIManager::clear()
    {
        m_texts.clear();
        m_nextTextId = 0;
    }
}
