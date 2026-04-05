#include "TextObject.h"
#include "Scene/Scene.h"

namespace grafyte
{
    TextObject::TextObject(std::shared_ptr<Scene> scene, const types::ObjectId& id)
        :m_scene(std::move(scene)), m_id(id)
    {
    }

    TextObject::~TextObject()
    = default;

    void TextObject::SetText(const std::string& text) const
    {
        if (auto scene = m_scene.lock()) scene->text(m_id).text = text;
    }

    void TextObject::SetScale(const float& scale) const
    {
        if (auto scene = m_scene.lock()) scene->text(m_id).transform.scale = {scale, scale};
    }

    void TextObject::SetColor(const types::Color4 &color) const {
        if (auto scene = m_scene.lock()) scene->text(m_id).color = color;
    }

    void TextObject::Remove() const
    {
        if (auto scene = m_scene.lock()) scene->RemoveText(m_id);
    }
}
