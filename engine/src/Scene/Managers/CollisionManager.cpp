#include "CollisionManager.h"
#include "Scene/Scene.h"

#include <stdexcept>
#include <iostream>
#include <ranges>

grafyte::collision::Hit grafyte::CollisionManager::ObjectsCollides(const types::ObjectId& A, const types::ObjectId& B, Scene& scene) {
    if (!(m_collisionBounds.contains(A) && m_collisionBounds.contains(B))) {
        throw std::runtime_error("[Collision Manager] Cannot check for collision between the two objects, at least one "
                                 "has no collision bounds");
    }

    const std::vector<collision::AABB>& AColliders = m_collisionBounds[A];
    const std::vector<collision::AABB>& BColliders = m_collisionBounds[B];

    const types::Vec2 aPos = scene.transform(A).pos;
    const types::Vec2 bPos = scene.transform(B).pos;

    for (const auto& a: AColliders)
    {
        const auto aWorld = collision::AABB{a.pos + aPos, a.width, a.height};
        for (const auto& b: BColliders)
        {
            const auto bWorld = collision::AABB{b.pos + bPos, b.width, b.height};
            if (const auto hit = Intersects(aWorld, bWorld)) return hit;
        }
    }

    return collision::Hit{collision::AABB{}, collision::AABB{}, false, collision::Top};
}

grafyte::collision::Hit grafyte::CollisionManager::IsColliding(const types::ObjectId& A, Scene& scene)
{
    if (m_colliding.contains(A)) {
        return m_colliding.at(A);
    }

    for (const auto& B : m_collisionBounds | std::views::keys)
    {
        if (B == A) continue;

        if (const auto hit = ObjectsCollides(A, B, scene))
        {
            m_colliding.insert_or_assign(A, hit);
            m_colliding.insert_or_assign(B, hit);
            return hit;
        }
    }

    return collision::Hit{collision::AABB{}, collision::AABB{}, false, collision::Top};
}

grafyte::types::Vec2 grafyte::CollisionManager::PushBackOnMove(const types::ObjectId& objId, const types::Vec2& v, Scene& scene)
{

    if (v.x == 0 && v.y == 0) return {0.0f, 0.0f};

    const float len = sqrt(v.x*v.x + v.y*v.y);
    if (len == 0.0f) return {0.0f, 0.0f};

    const types::Vec2 d = -1.0f * (v/len);

    if (!m_collisionBounds.contains(objId)) return {0.0f, 0.0f};

    const types::Vec2 aPos = scene.transform(objId).pos;
    const std::vector<collision::AABB> colliders = m_collisionBounds[objId];

    bool found = false;
    float bestFinalT = INFINITY;

    for (const auto&[id, bounds]: m_collisionBounds)
    {
        if (id == objId) {
            // Skip self-collision
            continue;
        }

        const types::Vec2 bPos = scene.transform(id).pos;

        for (const auto& A: colliders)
        {
            for (const auto& B: bounds)
            {
                const auto worldA = collision::AABB{A.pos + aPos, A.width, A.height};
                const auto worldB = collision::AABB{B.pos + bPos, B.width, B.height};

                const types::Vec2 hA = {A.width, A.height};
                const types::Vec2 hB = {B.width, B.height};

                const types::Vec2 p = worldA.pos - worldB.pos;
                const types::Vec2 h = hA + hB;

                if (std::abs(p.x) < h.x && std::abs(p.y) < h.y)
                {
                    types::Vec2 t = {INFINITY, INFINITY};

                    // For x
                    if (d.x > 0) t.x = (h.x - p.x)/d.x;
                    else if (d.x < 0) t.x = (- h.x - p.x)/d.x;

                    // For y
                    if (d.y > 0) t.y = (h.y - p.y)/d.y;
                    else if (d.y < 0) t.y = (- h.y - p.y)/d.y;

                    const float tmin = std::min(t.x, t.y);
                    if (tmin < 0.0f) continue;

                    if (const float finalT = tmin + EPSILON; finalT < bestFinalT)
                    {
                        found = true;
                        bestFinalT = finalT;
                    }
                }
            }
        }
    }
    if (!found) {
        return {0, 0};
    }

    const types::Vec2 result = d * bestFinalT;
    m_colliding.push_back(objId);
    return result;
}

grafyte::collision::Hit grafyte::CollisionManager::Intersects(const collision::AABB& a, const collision::AABB& b) {
    const float dx = a.pos.x - b.pos.x;
    const float dy = a.pos.y - b.pos.y;
    const float combinedWidth = a.width + b.width;
    const float combinedHeight = a.height + b.height;

    if (std::abs(dx) <= combinedWidth && std::abs(dy) <= combinedHeight) {
        const float overlapX = combinedWidth - std::abs(dx);
        const float overlapY = combinedHeight - std::abs(dy);

        collision::Direction dir;
        if (overlapX < overlapY) {
            dir = (dx > 0) ? collision::Right : collision::Left;
        } else {
            dir = (dy > 0) ? collision::Bottom : collision::Top;
        }

        return {a, b, true, dir};
    }

    return {a, b, false, collision::Top};
}

// bool grafyte::CollisionManager::Intersects(const collision::AABB &a, const collision::Circle &b) {
//     const types::Vec2 bl = {a.pos.x - a.width, a.pos.y - a.height};
//     const types::Vec2 tr = {a.pos.x + a.width, a.pos.y + a.height};
//
//     const types::Vec2 pt = {clampf(b.pos.x, bl.x, tr.x), clampf(b.pos.y, bl.y, tr.y)};
//
//     const float dx = pt.x - b.pos.x;
//     const float dy = pt.y - b.pos.y;
//     // Avoid using square root (as we don't need it here)
//     return (dx * dx + dy * dy) <= (b.radius*b.radius);
// }
//
// bool grafyte::CollisionManager::Intersects(const collision::Circle &a, const collision::Circle &b) {
//     const float dx = b.pos.x - a.pos.x;
//     const float dy = b.pos.y - a.pos.y;
//     const float r = a.radius + b.radius;
//
//     return (dx * dx + dy * dy) <= (r * r);
// }
//
// bool grafyte::CollisionManager::Intersects(const collision::Collider& a, const collision::Collider& b) const
// {
//     return std::visit([](auto const& lhs, auto const& rhs) -> bool {
//         return Intersects(lhs, rhs);
//     }, a, b);
// }
