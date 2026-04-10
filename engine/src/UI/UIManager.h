#pragma once
#include <memory>
#include <string>

#include "types.h"
#include "World/World.h"


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

            [[nodiscard]] TextId getId() const {return id;}
            void setText(const std::string& nText) {text = nText;}
            void setScale(const float& nScale) {scale = nScale;}
            void setColor(const types::Color4& nColor) {color = nColor;}
        };
    }

    class UIManager {
    public:
        explicit UIManager(std::shared_ptr<WorldContext> ctx);

        std::shared_ptr<ui::text::Text> addText(const types::Vec2& pos,
            const std::string& text,
            const float& scale = 1.0f,
            const ui::text::Anchor& anchor = ui::text::Anchor::TopLeft,
            const types::Color4& color = {0.0f, 0.0f, 0.0f, 1.0f});
        void removeText(const TextId& id);

        void getTexts(std::vector<ui::text::Text>& out);

        void clear();
    private:
        std::unordered_map<TextId, std::shared_ptr<ui::text::Text>> m_texts;
        TextId m_nextTextId = 0;
        std::shared_ptr<WorldContext> m_ctx;
    };
}
