#include "test_support.h"

#include "World/Camera.h"

TEST_CASE("Camera updates position and zoom through its public API")
{
    grafyte::Camera camera;

    camera.move({2.0f, -1.0f});
    expectVec2(camera.getPos(), {2.0f, -1.0f});

    camera.moveTo({10.0f, 20.0f});
    expectVec2(camera.getPos(), {10.0f, 20.0f});

    camera.setZoom(1.75f);
    expectNear(camera.getZoom(), 1.75f);
}

TEST_CASE("Camera stores follow target and follow offset")
{
    grafyte::Camera camera;

    camera.follow(42);
    CHECK(camera.followObject == 42);

    camera.setFollowOffset({-3.0f, 4.0f});
    expectVec2(camera.getFollowOffset(), {-3.0f, 4.0f});
}
