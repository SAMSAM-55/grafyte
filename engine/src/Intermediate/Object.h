#pragma once

#include <memory>
#include <string>

#include "GL-Core/IndexBuffer.h"
#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBuffer.h"
#include "GL-Core/VertexBufferLayout.h"
#include "Scene/Managers/Shapes.h"
#include "types.h"

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
    Object(const std::shared_ptr<Scene> &scene, const types::ObjectId &id, types::PrimitiveGeometry geo);
    ~Object();

    void setTexture(const std::string &textureSourcePath, unsigned int slot) const;
    void setColor(const types::Color4 &c) const;

    /* Actual object functions */
    void move(types::Vec2 offset) const;
    void moveTo(types::Vec2 pos) const;
    void rotate(float angle) const;
    void setRotation(float angle) const;
    void setScale(float scale) const;
    void setScale(types::Vec2 scale) const;

    void addCollisionBox(collision::AABB &b) const;
    void enableAutoCollides(const int &resolutionOrder) const;

    /* Getters */
    [[nodiscard]] types::ObjectId getId() const
    {
        return m_Id;
    };
    [[nodiscard]] Scene *getScene() const
    {
        return m_Scene.lock().get();
    };
    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] types::Vec2 getScale() const;
    [[nodiscard]] types::Vec2 getPosition() const;
    [[nodiscard]] float getRotation() const;

    /* Collisions */
    [[nodiscard]] bool collidesWith(const Object &other) const;
    [[nodiscard]] bool isColliding() const;

    void kill();

  private:
    std::weak_ptr<Scene> m_Scene;
    types::ObjectId m_Id = 0;
    types::PrimitiveGeometry m_Geo = types::QUAD;
};

} // namespace grafyte
