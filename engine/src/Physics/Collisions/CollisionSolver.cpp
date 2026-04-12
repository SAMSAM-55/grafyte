#include "CollisionSolver.h"

namespace grafyte
{
std::optional<types::Vec2> CollisionSolver::computePushBackTranslation(const collision::AABB &a,
                                                                       const collision::AABB &b,
                                                                       const types::Vec2 &aPos, const types::Vec2 &bPos)
{
    const auto worldA = collision::AABB{a.pos + aPos, a.width, a.height};
    const auto worldB = collision::AABB{b.pos + bPos, b.width, b.height};

    const types::Vec2 delta = worldA.pos - worldB.pos;
    const float overlapX = (a.width + b.width) - std::abs(delta.x);
    const float overlapY = (a.height + b.height) - std::abs(delta.y);

    if (overlapX <= 0.0f || overlapY <= 0.0f)
        return std::nullopt;

    if (overlapX < overlapY)
    {
        const float sign = (delta.x > 0.0f) ? 1.0f : -1.0f;
        return types::Vec2{sign * overlapX, 0.0f};
    }

    const float sign = (delta.y > 0.0f) ? 1.0f : -1.0f;
    return types::Vec2{0.0f, sign * overlapY};
}

collision::Hit CollisionSolver::intersects(const collision::AABB &a, const collision::AABB &b)
{
    const float dx = a.pos.x - b.pos.x;
    const float dy = a.pos.y - b.pos.y;
    const float combinedWidth = a.width + b.width;
    const float combinedHeight = a.height + b.height;

    if (std::abs(dx) <= combinedWidth && std::abs(dy) <= combinedHeight)
    {
        const float overlapX = combinedWidth - std::abs(dx);
        const float overlapY = combinedHeight - std::abs(dy);

        collision::Direction dir;
        if (overlapX < overlapY)
        {
            dir = (dx > 0) ? collision::Right : collision::Left;
        }
        else
        {
            dir = (dy > 0) ? collision::Bottom : collision::Top;
        }

        return {a, b, true, dir};
    }

    return {a, b, false, collision::Top};
}
} // namespace grafyte
