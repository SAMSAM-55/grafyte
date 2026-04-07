#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Shapes.h"
#include "Physics/Collisions/SpatialGrid.h"

namespace grafyte {
    constexpr float EPSILON = 1e-8f;

    class Scene;

    class CollisionManager {
    public:
        void AddCollisionBox(const types::ObjectId& objId, collision::AABB& box)
        {
            m_collisionBounds[objId].emplace_back(box);
        };
        // void AddCollisionCircle(const types::ObjectId& objId, collision::Circle& c);
        void EnableAutoCollides(const types::ObjectId& objId, const int& resolutionOrder) {
            m_autoCollides.insert_or_assign(objId, resolutionOrder);
        };
        [[nodiscard]] bool AutoCollides(const types::ObjectId& objId) const {
            return m_autoCollides.contains(objId);
        }

        collision::Hit ObjectsCollides(const types::ObjectId& A, const types::ObjectId& B, Scene& scene);
        std::vector<collision::Hit> IsColliding(const types::ObjectId& A, Scene& scene);
        types::Vec2 PushBackOnMove(const types::ObjectId& objId, const types::Vec2& v, Scene& scene);

        void resolveAutoCollides(Scene &scene);

        void RebuildGrid(Scene& scene);
        collision::AABB ComputeObjectWorldBounds(const types::ObjectId& id, Scene& scene) const;

        void markDirty(const types::ObjectId& id) {m_gridDirty.emplace_back(id);}

        void reset() {
            m_colliding.clear();
        }
    private:
        void buildGridFromDirty(Scene &scene);

        static float clampf(const float v, const float lo, const float hi) {
            return (v < lo) ? lo : (v > hi) ? hi : v;
        }

        std::unordered_map<types::ObjectId, std::vector<collision::AABB>> m_collisionBounds;
        std::unordered_map<types::ObjectId, int> m_autoCollides;
        std::unordered_map<types::ObjectId, std::vector<collision::Hit>> m_colliding;

        collision::SpatialGrid m_grid{12.0f};
        std::vector<types::ObjectId> m_gridDirty;
        bool built = false;
    };
}
