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
	}

	void Object::SetColor(const types::Color4& c) const
	{
		const types::MaterialHandle h = m_scene->renderable(m_id).mat;
		const types::Material* mat = m_scene->materials().mat(h);
		mat->shader.Bind();
		mat->shader.SetUniform4f("u_Color", c.x, c.y, c.z, c.w);
	}

	void Object::Move(const types::Vec2 offset) const {
		m_scene->transform(m_id).pos += offset;
		if (m_scene->collisions().AutoCollides(m_id))
			m_scene->transform(m_id).pos += m_scene->collisions().PushBackOnMove(m_id, offset, *m_scene);
	}

	void Object::MoveTo(const types::Vec2 pos) const {
		const types::Vec2 offset = pos - m_scene->transform(m_id).pos;
		m_scene->transform(m_id).pos = pos;
		if (m_scene->collisions().AutoCollides(m_id))
			m_scene->transform(m_id).pos += m_scene->collisions().PushBackOnMove(m_id, offset, *m_scene);
	}

	void Object::Rotate(const float angle) const
	{
		m_scene->transform(m_id).rot += angle;
	}

	void Object::SetRotation(const float angle) const
	{
		m_scene->transform(m_id).rot = angle;
	}

	void Object::SetScale(const float scale) const
	{
		m_scene->transform(m_id).scale = {scale, scale};
	}

	void Object::SetScale(const types::Vec2 scale) const
	{
		m_scene->transform(m_id).scale = scale;
	}

	void Object::AddCollisionBox(collision::AABB& b) const
	{
		m_scene->collisions().AddCollisionBox(m_id, b);
	}

	// void Object::AddCollisionCircle(collision::Circle& c) const
	// {
	// 	m_scene->collisions().AddCollisionCircle(m_id, c);
	// }

	void Object::EnableAutoCollides() const
	{
		m_scene->collisions().EnableAutoCollides(m_id);
	}

	types::Vec2 Object::GetScale() const
	{
		return m_scene->transform(m_id).scale;
	}

	types::Vec2 Object::GetPosition() const
	{
		return m_scene->transform(m_id).pos;
	}

	float Object::GetRotation() const
	{
		return m_scene->transform(m_id).rot;
	}

	bool Object::CollidesWith(const Object& other) const
	{
		return static_cast<bool>(m_scene->collisions().ObjectsCollides(m_id, other.GetId(), *m_scene));
	}

	bool Object::IsColliding() const
	{
		return static_cast<bool>(m_scene->collisions().IsColliding(m_id, *m_scene));
	}
}
