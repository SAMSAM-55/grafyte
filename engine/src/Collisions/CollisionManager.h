#pragma once

#include <unordered_map>

#include "Shapes.h"

namespace grafyte {
    class CollisionManager {
    public:
        static void setCollisionBox(const int& objId, const collision::AABB& box) {s_collisionBounds[objId] = box;}
        static void setCollisionCircle(const int& objId, const collision::Circle& c) {s_collisionBounds[objId] = c;}

        static bool objectCollidesWith(const int& objId1, const int& objId2);
    private:
        static float clampf(const float v, const float lo, const float hi) {
            return (v < lo) ? lo : (v > hi) ? hi : v;
        }

        static bool intersects(const collision::AABB& a, const collision::AABB& b);
        static bool intersects(const collision::AABB& a, const collision::Circle& b);
        static bool intersects(const collision::Circle& a, const collision::AABB& b) {return intersects(b, a);};
        static bool intersects(const collision::Circle& a, const collision::Circle& b);
        static bool intersects(const collision::Collider& a, const collision::Collider& b);


        static std::unordered_map<int, collision::Collider> s_collisionBounds;
    };
}
