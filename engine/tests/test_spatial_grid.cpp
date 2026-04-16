#include "test_support.h"

#include "Physics/Collisions/SpatialGrid.h"

TEST_CASE("SpatialGrid returns inserted ids once even when a shape spans multiple cells")
{
    using namespace grafyte::collision;

    SpatialGrid grid(1.0f);
    const AABB first{{0.5f, 0.5f}, 1.0f, 1.0f};

    grid.insert(7, first);

    const auto firstCandidates = grid.queryCandidates(first);
    REQUIRE(firstCandidates.size() == 1);
    CHECK(firstCandidates.front() == 7);

    const AABB second{{0.0f, 0.0f}, 1.5f, 1.5f};
    grid.insert(1, second);

    const auto secondCandidates = grid.queryCandidates(first);
    REQUIRE(secondCandidates.size() == 2);
}

TEST_CASE("SpatialGrid ignores unrelated areas")
{
    using namespace grafyte::collision;

    SpatialGrid grid(1.0f);
    grid.insert(7, AABB{{0.5f, 0.5f}, 1.0f, 1.0f});

    const auto farQuery = grid.queryCandidates(AABB{{10.0f, 10.0f}, 0.5f, 0.5f});
    CHECK(farQuery.empty());
}

TEST_CASE("SpatialGrid cleanDirty removes only the requested object id")
{
    using namespace grafyte::collision;

    SpatialGrid grid(1.0f);
    const AABB areaA{{0.5f, 0.5f}, 1.0f, 1.0f};
    const AABB areaB{{4.5f, 0.5f}, 0.5f, 0.5f};

    grid.insert(1, areaA);
    grid.insert(2, areaB);

    grid.cleanDirty({1});

    CHECK(grid.queryCandidates(areaA).empty());

    const auto candidatesB = grid.queryCandidates(areaB);
    REQUIRE(candidatesB.size() == 1);
    CHECK(candidatesB.front() == 2);
}
