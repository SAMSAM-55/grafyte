#include "test_support.h"

#include "Physics/Collisions/CollisionSolver.h"

TEST_CASE("CollisionSolver detects overlap on the intended axis")
{
    using namespace grafyte;

    const collision::AABB a{{0.5f, 0.0f}, 1.0f, 1.0f};
    const collision::AABB b{{0.0f, 0.0f}, 1.0f, 1.0f};
    const collision::AABB farAway{{10.0f, 0.0f}, 1.0f, 1.0f};

    const auto hit = CollisionSolver::intersects(a, b);
    CHECK(hit);
    CHECK(hit.direction == collision::Right);

    const auto miss = CollisionSolver::intersects(a, farAway);
    CHECK_FALSE(miss);
}

TEST_CASE("CollisionSolver computes a minimal push-back translation")
{
    using namespace grafyte;

    const collision::AABB a{{0.6f, 0.0f}, 1.0f, 2.0f};
    const collision::AABB b{{0.0f, 0.0f}, 1.0f, 2.0f};

    const auto translation = CollisionSolver::computePushBackTranslation(a, b, {0.0f, 0.0f}, {0.0f, 0.0f});
    REQUIRE(translation.has_value());
    expectVec2(*translation, {1.4f, 0.0f});

    const collision::AABB separated{{5.0f, 5.0f}, 1.0f, 1.0f};
    const auto none = CollisionSolver::computePushBackTranslation(a, separated, {0.0f, 0.0f}, {0.0f, 0.0f});
    CHECK_FALSE(none.has_value());
}
