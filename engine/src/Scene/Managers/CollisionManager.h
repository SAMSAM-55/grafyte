#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <__msvc_ranges_to.hpp>

#include "Shapes.h"

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
        collision::Hit IsColliding(const types::ObjectId& A, Scene& scene);
        types::Vec2 PushBackOnMove(const types::ObjectId& objId, const types::Vec2& v, Scene& scene);

        void resolveAutoCollides(Scene &scene);

        void reset() {
            m_colliding.clear();
        }
    private:
        static float clampf(const float v, const float lo, const float hi) {
            return (v < lo) ? lo : (v > hi) ? hi : v;
        }

        static collision::Hit Intersects(const collision::AABB& a, const collision::AABB& b);
        // static bool Intersects(const collision::AABB& a, const collision::Circle& b);
        // static bool Intersects(const collision::Circle& a, const collision::AABB& b) {return Intersects(b, a);};
        // static bool Intersects(const collision::Circle& a, const collision::Circle& b);
        // [[nodiscard]] bool Intersects(const collision::Collider& a, const collision::Collider& b) const;

        std::unordered_map<types::ObjectId, std::vector<collision::AABB>> m_collisionBounds;
        std::unordered_map<types::ObjectId, int> m_autoCollides;
        std::unordered_map<types::ObjectId, collision::Hit> m_colliding;
    };
}
