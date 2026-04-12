#include "CollisionManager.h"
#include "Scene/Scene.h"

#include <cmath>
#include <iostream>
#include <ranges>
#include <stdexcept>

#include "Physics/Collisions/CollisionSolver.h"

grafyte::collision::Hit grafyte::CollisionManager::objectsCollide(const types::ObjectId &a, const types::ObjectId &b,
                                                                  Scene &scene)
{
    if (!(m_CollisionBounds.contains(a) && m_CollisionBounds.contains(b)))
    {
        throw std::runtime_error("[Collision Manager] Cannot check for collision between the two objects, at least one "
                                 "has no collision bounds");
    }

    const std::vector<collision::AABB> &aColliders = m_CollisionBounds[a];
    const std::vector<collision::AABB> &bColliders = m_CollisionBounds[b];

    const types::Vec2 aPos = scene.transform(a).pos;
    const types::Vec2 bPos = scene.transform(b).pos;

    for (const auto &a : aColliders)
    {
        const auto aWorld = collision::AABB{a.pos + aPos, a.width, a.height};
        for (const auto &b : bColliders)
        {
            const auto bWorld = collision::AABB{b.pos + bPos, b.width, b.height};
            if (const auto hit = CollisionSolver::intersects(aWorld, bWorld))
                return hit;
        }
    }

    return collision::Hit{collision::AABB{}, collision::AABB{}, false, collision::Top};
}

std::vector<grafyte::collision::Hit> grafyte::CollisionManager::isColliding(const types::ObjectId &a, Scene &scene)
{
    const auto worldBounds = computeObjectWorldBounds(a, scene);

    for (const auto queryCandidates = m_Grid.queryCandidates(worldBounds); const auto &b : queryCandidates)
    {
        if (b == a)
            continue;

        if (!(m_CollisionBounds.contains(b)))
            continue;

        if (const auto hit = objectsCollide(a, b, scene))
        {
            m_Colliding[a].push_back(hit);
        }
    }

    if (!m_Colliding[a].empty())
        return m_Colliding[a];

    return {collision::Hit{collision::AABB{}, collision::AABB{}, false, collision::Top}};
}

// ==================================================
// [SECTION] Auto collisions
// ==================================================

grafyte::types::Vec2 grafyte::CollisionManager::pushBackOnMove(const types::ObjectId &objId, const types::Vec2 &mov,
                                                               Scene &scene)
{
    if (mov.x == 0 && mov.y == 0)
        return {0.0f, 0.0f};

    if (!m_CollisionBounds.contains(objId))
        return {0.0f, 0.0f};

    const types::Vec2 aPos = scene.transform(objId).pos;
    const std::vector<collision::AABB> colliders = m_CollisionBounds[objId];

    bool found = false;
    types::Vec2 bestTranslation{0.0f, 0.0f};
    float bestLenSq = INFINITY;

    for (const auto &[id, bounds] : m_CollisionBounds)
    {
        if (id == objId)
        {
            // Skip self-collision
            continue;
        }

        if (m_AutoCollides.contains(id) && m_AutoCollides.contains(objId))
        {
            if (m_AutoCollides.at(id) < m_AutoCollides.at(objId))
            {
                if (const types::Vec2 opposite = pushBackOnMove(id, -mov, scene);
                    opposite.x != 0.0f || opposite.y != 0.0f)
                {
                    scene.transform(id).pos += opposite;
                    return {0.0f, 0.0f};
                }
            }
        }

        const types::Vec2 bPos = scene.transform(id).pos;

        for (const auto &aCollider : colliders)
        {
            for (const auto &bCollider : bounds)
            {
                if (const auto translation =
                        CollisionSolver::computePushBackTranslation(aCollider, bCollider, aPos, bPos))
                {
                    if (const float lenSq = translation->x * translation->x + translation->y * translation->y;
                        !found || lenSq < bestLenSq)
                    {
                        found = true;
                        bestLenSq = lenSq;
                        bestTranslation = *translation;
                    }
                }
            }
        }
    }
    if (!found)
    {
        return {0, 0};
    }

    return bestTranslation;
}

void grafyte::CollisionManager::rebuildGrid(Scene &scene)
{
    if (m_Built)
    {
        buildGridFromDirty(scene);
        return;
    }

    m_Grid.clear();

    for (const auto &[id, bounds] : m_CollisionBounds)
    {
        if (bounds.empty())
            continue;
        const auto worldBounds = computeObjectWorldBounds(id, scene);
        m_Grid.insert(id, worldBounds);
    }

    m_Built = true;
}

grafyte::collision::AABB grafyte::CollisionManager::computeObjectWorldBounds(const types::ObjectId &id,
                                                                             Scene &scene) const
{
    const auto &colliders = m_CollisionBounds.at(id);
    const auto objPos = scene.transform(id).pos;

    float minX = INFINITY;
    float minY = INFINITY;
    float maxX = -INFINITY;
    float maxY = -INFINITY;

    for (const auto &c : colliders)
    {
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

void grafyte::CollisionManager::removeObject(const types::ObjectId &objId)
{
    m_AutoCollides.erase(objId);
    m_Colliding.erase(objId);
    m_CollisionBounds.erase(objId);
    m_GridDirty.push_back(objId);
}

void grafyte::CollisionManager::buildGridFromDirty(Scene &scene)
{
    m_Grid.cleanDirty(m_GridDirty);

    for (const auto &id : m_GridDirty)
    {
        const auto it = m_CollisionBounds.find(id);
        if (it == m_CollisionBounds.end() || it->second.empty())
            continue;

        const auto worldBounds = computeObjectWorldBounds(id, scene);
        m_Grid.insert(id, worldBounds);
    }

    m_GridDirty.clear();
}
