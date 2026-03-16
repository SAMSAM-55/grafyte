#include "TextObject.h"
#include "Scene/Scene.h"

namespace grafyte
{
    TextObject::TextObject(Scene* scene, const types::ObjectId& id)
        :m_scene(scene), m_id(id)
    {
    }

    TextObject::~TextObject()
    = default;

    void TextObject::SetText(const std::string& text) const
    {
        m_scene->text(m_id).text = text;
    }

    void TextObject::SetScale(const float& scale) const
    {
        m_scene->text(m_id).transform.scale = {scale, scale};
    }

    void TextObject::Remove() const
    {
        m_scene->RemoveText(m_id);
    }
}
