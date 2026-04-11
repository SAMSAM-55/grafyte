#include "TextObject.h"
#include "Scene/Scene.h"

namespace grafyte
{
TextObject::TextObject(const std::shared_ptr<Scene> &scene, const types::ObjectId &id) : m_Scene(scene), m_Id(id)
{
}

TextObject::~TextObject() = default;

void TextObject::setText(const std::string &text) const
{
    if (const auto scene = m_Scene.lock())
        scene->text(m_Id).text = text;
}

void TextObject::setScale(const float &scale) const
{
    if (const auto scene = m_Scene.lock())
        scene->text(m_Id).transform.scale = {scale, scale};
}

void TextObject::setColor(const types::Color4 &color) const
{
    if (const auto scene = m_Scene.lock())
        scene->text(m_Id).color = color;
}

void TextObject::remove() const
{
    if (const auto scene = m_Scene.lock())
        scene->removeText(m_Id);
}
} // namespace grafyte
