#pragma once

#include <optional>
#include "Scene/Managers/Shapes.h"

namespace grafyte {
    class CollisionSolver {
    public:
        static std::optional<types::Vec2> ComputePushBackTranslation(const collision::AABB& A,
                                                                     const collision::AABB& B,
                                                                     const types::Vec2& aPos,
                                                                     const types::Vec2& bPos);

        static collision::Hit Intersects(const collision::AABB& a, const collision::AABB& b);
        // static bool Intersects(const collision::AABB& a, const collision::Circle& b);
        // static bool Intersects(const collision::Circle& a, const collision::AABB& b) {return Intersects(b, a);};
        // static bool Intersects(const collision::Circle& a, const collision::Circle& b);
        // [[nodiscard]] bool Intersects(const collision::Collider& a, const collision::Collider& b) const;

    private:

    };
}
