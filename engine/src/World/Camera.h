#pragma once

#include "glm/glm.hpp"
#include "types.h"

namespace grafyte
{
struct Camera
{
    glm::mat4 projection{};
    glm::mat4 view = glm::mat4(1.0f);

    types::Vec2 pos;
    float left = 1.0f, right = 1.0f, top = 1.0f, bottom = 0.0f;

    float zoom = 1.0f;

    float followSpeed = 5.0f;
    types::ObjectId followObject = -1;
    types::Vec2 followOffset = types::Vec2(0, 0);

    void move(const types::Vec2 &offset)
    {
        pos += offset;
    }
    void moveTo(const types::Vec2 &nPos)
    {
        pos = nPos;
    }

    [[nodiscard]] types::Vec2 getPos() const
    {
        return pos;
    }

    void setZoom(const float &nZoom)
    {
        zoom = nZoom;
    }

    [[nodiscard]] float getZoom() const
    {
        return zoom;
    }
    [[nodiscard]] types::Vec2 getFollowOffset() const
    {
        return followOffset;
    }

    void follow(const types::ObjectId &id)
    {
        followObject = id;
    }
    void setFollowOffset(const types::Vec2 &offset)
    {
        followOffset = offset;
    }
};
} // namespace grafyte
