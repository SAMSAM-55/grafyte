#pragma once

#include <string>
#include <memory>

#include "types.h"
namespace grafyte {
    class Scene;

    class TextObject {
    public:
        explicit TextObject(std::shared_ptr<Scene> scene, const types::ObjectId& id);
        ~TextObject();

        void SetText(const std::string& text) const;
        void SetScale(const float& scale) const;
        void SetColor(const types::Color4& color) const;
        void Remove() const;

    private:
        std::weak_ptr<Scene> m_scene;
        types::ObjectId m_id;
    };
}
