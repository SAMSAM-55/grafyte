#include "Object.h"
#include "Scene/Scene.h"

namespace grafyte 
{
	Object::Object(std::shared_ptr<Scene> scene, const types::ObjectId& id, types::PrimitiveGeometry geo)
		: m_scene(std::move(scene)), m_id(id), m_geo(geo)
	{
	}

	Object::~Object() = default;

	void Object::SetTexture(const std::string& textureSourcePath, const unsigned int slot) const {
		auto scene = m_scene.lock();
		if (!scene) return;

		const types::MaterialHandle h = scene->renderable(m_id).mat;
		types::MaterialAsset* mat = scene->mat(h);
		mat->textureSlot = slot;
		mat->textureSourcePath = textureSourcePath;

		scene->materials().upload(h);
	}

	void Object::SetColor(const types::Color4& c) const
	{
		if (auto scene = m_scene.lock()) scene->color(m_id) = c;
	}

	void Object::Move(const types::Vec2 offset) const {
		auto scene = m_scene.lock();
		if (!scene) return;

		scene->transform(m_id).pos += offset;
		if (scene->collisions().AutoCollides(m_id))
			scene->transform(m_id).pos += scene->collisions().PushBackOnMove(m_id, offset, *scene);
	}

	void Object::MoveTo(const types::Vec2 pos) const {
		auto scene = m_scene.lock();
		if (!scene) return;

		const types::Vec2 offset = pos - scene->transform(m_id).pos;
		scene->transform(m_id).pos = pos;
		if (scene->collisions().AutoCollides(m_id))
			scene->transform(m_id).pos += scene->collisions().PushBackOnMove(m_id, offset, *scene);
	}

	void Object::Rotate(const float angle) const
	{
		if (auto scene = m_scene.lock()) scene->transform(m_id).rot += angle;
	}

	void Object::SetRotation(const float angle) const
	{
		if (auto scene = m_scene.lock()) scene->transform(m_id).rot = angle;
	}

	void Object::SetScale(const float scale) const
	{
		if (auto scene = m_scene.lock()) scene->transform(m_id).scale = {scale, scale};
	}

	void Object::SetScale(const types::Vec2 scale) const
	{
		if (auto scene = m_scene.lock()) scene->transform(m_id).scale = scale;
	}

	void Object::AddCollisionBox(collision::AABB& b) const
	{
		if (auto scene = m_scene.lock()) scene->collisions().AddCollisionBox(m_id, b);
	}

	// void Object::AddCollisionCircle(collision::Circle& c) const
	// {
	// 	m_scene->collisions().AddCollisionCircle(m_id, c);
	// }

	void Object::EnableAutoCollides(const int& resolutionOrder) const
	{
		if (auto scene = m_scene.lock()) scene->collisions().EnableAutoCollides(m_id, resolutionOrder);
	}

	types::Vec2 Object::GetScale() const
	{
		if (auto scene = m_scene.lock()) return scene->transform(m_id).scale;
		return {0.0f, 0.0f};
	}

	types::Vec2 Object::GetPosition() const
	{
		if (auto scene = m_scene.lock()) return scene->transform(m_id).pos;
		return {0.0f, 0.0f};
	}

	float Object::GetRotation() const
	{
		if (auto scene = m_scene.lock()) return scene->transform(m_id).rot;
		return 0.0f;
	}

	bool Object::CollidesWith(const Object& other) const
	{
		if (auto scene = m_scene.lock()) return static_cast<bool>(scene->collisions().ObjectsCollides(m_id, other.GetId(), *scene));
		return false;
	}

	bool Object::IsColliding() const
	{
		if (auto scene = m_scene.lock()) return static_cast<bool>(scene->collisions().IsColliding(m_id, *scene));
		return false;
	}
}
