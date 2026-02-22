#include "CollisionManager.h"

#include <stdexcept>

bool grafyte::CollisionManager::objectCollidesWith(const int &objId1, const int &objId2) {
    if (!(s_collisionBounds.contains(objId1) && s_collisionBounds.contains(objId2))) {
        throw std::runtime_error("[Collision Manager] Cannot check for collision between the two objects, at least one "
                                 "has no collision bounds");
    }

    return intersects(s_collisionBounds[objId1], s_collisionBounds[objId2]);
}

bool grafyte::CollisionManager::intersects(const collision::AABB& a, const collision::AABB& b) {
    return
        std::abs(a.pos.x - b.pos.x) <= (a.width  + b.width) &&
        std::abs(a.pos.y - b.pos.y) <= (a.height + b.height);
}

bool grafyte::CollisionManager::intersects(const collision::AABB &a, const collision::Circle &b) {
    const types::Vec2 bl = {a.pos.x - a.width, a.pos.y - a.height};
    const types::Vec2 tr = {a.pos.x + a.width, a.pos.y + a.height};

    const types::Vec2 pt = {clampf(b.pos.x, bl.x, tr.x), clampf(b.pos.y, bl.y, tr.y)};

    const float dx = pt.x - b.pos.x;
    const float dy = pt.y - b.pos.y;
    // Avoid using square root (as we don't need it here)
    return (dx * dx + dy * dy) <= (b.radius*b.radius);
}

bool grafyte::CollisionManager::intersects(const collision::Circle &a, const collision::Circle &b) {
    const float dx = b.pos.x - a.pos.x;
    const float dy = b.pos.y - a.pos.y;
    const float r = a.radius + b.radius;

    return (dx * dx + dy * dy) <= (r * r);
}

bool grafyte::CollisionManager::intersects(const collision::Collider& a, const collision::Collider& b)
{
    return std::visit([](auto const& lhs, auto const& rhs) {
        return intersects(lhs, rhs);
    }, a, b);
}
