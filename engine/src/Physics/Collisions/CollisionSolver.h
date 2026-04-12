#pragma once

#include "Scene/Managers/Shapes.h"
#include <optional>

namespace grafyte
{
class CollisionSolver
{
  public:
    static std::optional<types::Vec2> computePushBackTranslation(const collision::AABB &a, const collision::AABB &b,
                                                                 const types::Vec2 &aPos, const types::Vec2 &bPos);

    static collision::Hit intersects(const collision::AABB &a, const collision::AABB &b);
};
} // namespace grafyte
