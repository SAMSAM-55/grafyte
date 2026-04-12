#include "Object.h"
#include "Scene/Scene.h"

namespace grafyte
{
Object::Object(const std::shared_ptr<Scene> &scene, const types::ObjectId &id, types::PrimitiveGeometry geo)
    : m_Scene(scene), m_Id(id), m_Geo(geo)
{
}

Object::~Object() = default;

bool Object::isAlive() const
{
    const auto scene = m_Scene.lock();
    return scene && scene->containsObject(m_Id);
}

void Object::setTexture(const std::string &textureSourcePath, const unsigned int slot) const
{
    const auto scene = m_Scene.lock();
    if (!scene || !scene->containsObject(m_Id))
        return;

    const types::MaterialHandle h = scene->renderable(m_Id).mat;
    types::MaterialAsset *mat = scene->mat(h);
    mat->textureSlot = slot;
    mat->textureSourcePath = textureSourcePath;

    scene->materials().upload(h);
}

void Object::setColor(const types::Color4 &c) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->color(m_Id) = c;
}

void Object::move(const types::Vec2 offset) const
{
    const auto scene = m_Scene.lock();
    if (!scene || !scene->containsObject(m_Id))
        return;

    scene->transform(m_Id).pos += offset;
    scene->collisions().markDirty(m_Id);
    if (scene->collisions().autoCollides(m_Id))
        scene->transform(m_Id).pos += scene->collisions().pushBackOnMove(m_Id, offset, *scene);
}

void Object::moveTo(const types::Vec2 pos) const
{
    const auto scene = m_Scene.lock();
    if (!scene || !scene->containsObject(m_Id))
        return;

    const types::Vec2 offset = pos - scene->transform(m_Id).pos;
    scene->transform(m_Id).pos = pos;
    scene->collisions().markDirty(m_Id);
    if (scene->collisions().autoCollides(m_Id))
        scene->transform(m_Id).pos += scene->collisions().pushBackOnMove(m_Id, offset, *scene);
}

void Object::rotate(const float angle) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->transform(m_Id).rot += angle;
}

void Object::setRotation(const float angle) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->transform(m_Id).rot = angle;
}

void Object::setScale(const float scale) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->transform(m_Id).scale = {scale, scale};
}

void Object::setScale(const types::Vec2 scale) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->transform(m_Id).scale = scale;
}

void Object::addCollisionBox(collision::AABB &b) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
    {
        scene->collisions().addCollisionBox(m_Id, b);
        scene->collisions().markDirty(m_Id);
    }
}

void Object::enableAutoCollides(const int &resolutionOrder) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->collisions().enableAutoCollides(m_Id, resolutionOrder);
}

types::Vec2 Object::getScale() const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        return scene->transform(m_Id).scale;
    return {0.0f, 0.0f};
}

types::Vec2 Object::getPosition() const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        return scene->transform(m_Id).pos;
    return {0.0f, 0.0f};
}

float Object::getRotation() const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        return scene->transform(m_Id).rot;
    return 0.0f;
}

bool Object::collidesWith(const Object &other) const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id) && scene->containsObject(other.getId()))
        return static_cast<bool>(scene->collisions().objectsCollide(m_Id, other.getId(), *scene));
    return false;
}

bool Object::isColliding() const
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
    {
        for (const auto &h : scene->collisions().isColliding(m_Id, *scene))
        {
            if (h.collision)
                return true;
        }
    }
    return false;
}
void Object::kill()
{
    if (const auto scene = m_Scene.lock(); scene && scene->containsObject(m_Id))
        scene->removeObject(getId());
}
} // namespace grafyte
