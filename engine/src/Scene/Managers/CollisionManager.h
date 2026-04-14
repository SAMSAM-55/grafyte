#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Physics/Collisions/SpatialGrid.h"
#include "Shapes.h"

namespace grafyte
{
constexpr float EPSILON = 1e-8f;

class Scene;

class CollisionManager
{
  public:
    void addCollisionBox(const types::ObjectId &objId, collision::AABB &box)
    {
        m_CollisionBounds[objId].emplace_back(box);
    };
    void enableAutoCollides(const types::ObjectId &objId, const int &resolutionOrder)
    {
        m_AutoCollides.insert_or_assign(objId, resolutionOrder);
    };
    [[nodiscard]] bool autoCollides(const types::ObjectId &objId) const
    {
        return m_AutoCollides.contains(objId);
    }

    collision::Hit objectsCollide(const types::ObjectId &a, const types::ObjectId &b, Scene &scene);
    std::vector<collision::Hit> isColliding(const types::ObjectId &a, Scene &scene);
    types::Vec2 pushBackOnMove(const types::ObjectId &objId, const types::Vec2 &mov, Scene &scene);

    void rebuildGrid(Scene &scene);
    collision::AABB computeObjectWorldBounds(const types::ObjectId &id, Scene &scene) const;

    void markDirty(const types::ObjectId &id)
    {
        m_GridDirty.emplace_back(id);
    }

    void reset()
    {
        m_Colliding.clear();
    }

    void clear()
    {
        m_CollisionBounds.clear();
        m_AutoCollides.clear();
        m_Colliding.clear();
        m_Grid.clear();
        m_GridDirty.clear();
        m_Built = false;
    }

    void removeObject(const types::ObjectId &objId);

  private:
    void buildGridFromDirty(Scene &scene);

    static float clampf(const float v, const float lo, const float hi)
    {
        return (v < lo) ? lo : (v > hi) ? hi : v;
    }

    std::unordered_map<types::ObjectId, std::vector<collision::AABB>> m_CollisionBounds;
    std::unordered_map<types::ObjectId, int> m_AutoCollides;
    std::unordered_map<types::ObjectId, std::vector<collision::Hit>> m_Colliding;

    collision::SpatialGrid m_Grid{12.0f};
    std::vector<types::ObjectId> m_GridDirty;
    bool m_Built = false;
};
} // namespace grafyte
