#pragma once

#include <string>
#include <memory>

#include "types.h"
#include "GL-Core/IndexBuffer.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBuffer.h"
#include "GL-Core/VertexBufferLayout.h"
#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"
#include "Scene/Managers/Shapes.h"

namespace grafyte 
{
	class Scene;

	struct ObjectRenderData
	{
		IndexBuffer ib;
		VertexArray va;
		VertexBuffer vb;
		VertexBufferLayout layout;
		Shader shader;
		Texture texture;
		bool hasTexture;
		unsigned int textureSlot;
		int zIndex;
	};

	class Object
	{
	public:
		Object(std::shared_ptr<Scene> scene, const types::ObjectId& id, types::PrimitiveGeometry geo);
		~Object();

		void SetTexture(const std::string& textureSourcePath, unsigned int slot) const;
		void SetTint(const types::Color4& t) const;
		void SetColor(const types::Color4& c) const;

		/* Actual object functions */
		void Move(types::Vec2 offset) const;
		void MoveTo(types::Vec2 pos) const;
		void Rotate(float angle) const;
		void SetRotation(float angle) const;
		void SetScale(float scale) const;
		void SetScale(types::Vec2 scale) const;

		void AddCollisionBox(collision::AABB& b) const;
		// void AddCollisionCircle(collision::Circle& c) const;
		void EnableAutoCollides(const int &resolutionOrder) const;

		/* Getters */
		[[nodiscard]] types::ObjectId GetId() const {return m_id;};
		[[nodiscard]] Scene* GetScene() const {return m_scene.lock().get();};
		[[nodiscard]] types::Vec2 GetScale() const;
		[[nodiscard]] types::Vec2 GetPosition() const;
		[[nodiscard]] float GetRotation() const;

		/* Collisions */
		[[nodiscard]] bool CollidesWith(const Object& other) const;
		[[nodiscard]] bool IsColliding() const;

	private:
		std::weak_ptr<Scene> m_scene;
		types::ObjectId m_id = 0;
		types::PrimitiveGeometry m_geo = types::QUAD;
	};

}
