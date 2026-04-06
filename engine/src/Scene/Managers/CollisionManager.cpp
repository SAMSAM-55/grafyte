#include "CollisionManager.h"
#include "Scene/Scene.h"

#include <stdexcept>
#include <iostream>
#include <ranges>

#include "Physics/Collisions/CollisionSolver.h"

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
            if (const auto hit = CollisionSolver::Intersects(aWorld, bWorld)) return hit;
        }
    }

    return collision::Hit{collision::AABB{}, collision::AABB{}, false, collision::Top};
}

grafyte::collision::Hit grafyte::CollisionManager::IsColliding(const types::ObjectId& A, Scene& scene)
{
    if (m_colliding.contains(A)) {
        return m_colliding.at(A);
    }

    const auto worldBounds = ComputeObjectWorldBounds(A, scene);
    const auto queryCandidates = m_grid.queryCandidates(worldBounds);

    for (const auto& B : queryCandidates)
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

    const types::Vec2 d = -(v/len);

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

        if (m_autoCollides.contains(id) && m_autoCollides.contains(objId)) {
            if (m_autoCollides.at(id) < m_autoCollides.at(objId)) {
                const types::Vec2 opposite = PushBackOnMove(id, -v, scene);
                if (opposite.x != 0.0f || opposite.y != 0.0f) {
                    scene.transform(id).pos += opposite;
                    return {0.0f, 0.0f};
                }
            }
        }

        const types::Vec2 bPos = scene.transform(id).pos;

        for (const auto& A: colliders)
        {
            for (const auto& B: bounds)
            {
                const float finalT = CollisionSolver::ComputePushBackT(A, B, aPos, bPos, d);

                if (finalT == -1.0f) continue;

                if (finalT < bestFinalT)
                {
                    found = true;
                    bestFinalT = finalT;
                }
            }
        }
    }
    if (!found) {
        return {0, 0};
    }

    const types::Vec2 result = d * bestFinalT;
    return result;
}

void grafyte::CollisionManager::resolveAutoCollides(Scene& scene) {
    for (const auto& id: m_autoCollides | std::views::keys) {
        PushBackOnMove(id, {0.0f, 0.0f}, scene);
    }
}

void grafyte::CollisionManager::RebuildGrid(Scene &scene) {
    m_grid.clear();

    for (const auto& [id, bounds] : m_collisionBounds) {
        if (bounds.empty()) continue;
        const auto worldBounds = ComputeObjectWorldBounds(id, scene);
        m_grid.insert(id, worldBounds);
    }
}

grafyte::collision::AABB grafyte::CollisionManager::ComputeObjectWorldBounds(const types::ObjectId& id, Scene& scene) const {
    const auto& colliders = m_collisionBounds.at(id);
    const auto objPos = scene.transform(id).pos;

    float minX = INFINITY;
    float minY = INFINITY;
    float maxX = -INFINITY;
    float maxY = -INFINITY;

    for (const auto& c : colliders) {
        const float cMinX = objPos.x + c.pos.x - c.width;
        const float cMaxX = objPos.x + c.pos.x + c.width;
        const float cMinY = objPos.y + c.pos.y - c.height;
        const float cMaxY = objPos.y + c.pos.y + c.height;

        minX = std::min(minX, cMinX);
        minY = std::min(minY, cMinY);
        maxX = std::max(maxX, cMaxX);
        maxY = std::max(maxY, cMaxY);
    }

    const float centerX = (minX + maxX) * 0.5f;
    const float centerY = (minY + maxY) * 0.5f;
    const float halfW = (maxX - minX) * 0.5f;
    const float halfH = (maxY - minY) * 0.5f;

    return {{centerX, centerY}, halfW, halfH};
}
