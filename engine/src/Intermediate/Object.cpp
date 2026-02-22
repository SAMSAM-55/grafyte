#include "Object.h"
#include "Scene/Scene.h"

namespace grafyte 
{
	Object::Object(const types::ObjectId& id, Scene* scene)
		: m_scene(scene), m_id(id)
	{
	}

	Object::~Object() = default;

	void Object::SetTexture(const std::string& textureSourcePath, const unsigned int slot) const {
		const types::MaterialHandle h = m_scene->renderable(m_id).mat;
		types::MaterialAsset* mat = m_scene->mat(h);
		mat->textureSlot = slot;
		mat->textureSourcePath = textureSourcePath;

		m_scene->materials().upload(h);
	}

	void Object::SetTint(const types::Color4& t) const
	{
		const types::MaterialHandle h = m_scene->renderable(m_id).mat;
		const types::Material* mat = m_scene->materials().mat(h);
		mat->shader.Bind();
		mat->shader.SetUniform4f("u_Tint", t.x, t.y, t.z, t.w);

		// m_scene->materials().upload(h);
	}

	void Object::SetColor(const types::Color4& c) const
	{
		const types::MaterialHandle h = m_scene->renderable(m_id).mat;
		const types::Material* mat = m_scene->materials().mat(h);
		mat->shader.Bind();
		mat->shader.SetUniform4f("u_Color", c.x, c.y, c.z, c.w);

		// m_scene->materials().upload(h);
	}

	void Object::Move(const types::Vec2 offset) const {
		m_scene->transform(m_id).pos += offset;
	}

	void Object::MoveTo(const types::Vec2 pos) const {
		m_scene->transform(m_id).pos = pos;
	}

}
