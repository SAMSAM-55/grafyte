#pragma once

#include <memory>
#include <unordered_map>

#include "../World/World.h"
#include "Intermediate/Object.h"
#include "Managers/CollisionManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Text/TextObject.h"
#include "World/Camera.h"
#include "types.h"

namespace grafyte
{
struct WorldContext;

class Scene : public std::enable_shared_from_this<Scene>
{
  public:
    explicit Scene(std::shared_ptr<WorldContext> ctx, const types::SceneId &id);

    types::SceneId getId() const
    {
        return m_Id;
    }

    types::ObjectId allocateId()
    {
        return composeObjectId(m_NextId++);
    };
    types::ObjectId allocateTextId()
    {
        return composeObjectId(m_NextTextId++);
    };
    std::shared_ptr<Object> spawnObject(const types::MeshAsset &mesh, const types::MaterialAsset &mat,
                                        const types::Vec2 &pos, const int &zIndex, types::PrimitiveGeometry geo);
    std::shared_ptr<TextObject> spawnTextObject(const types::Vec2 &pos, const std::string &text, const float &size);

    [[nodiscard]] bool containsObject(const types::ObjectId &id) const
    {
        return m_Objects.contains(id);
    }
    [[nodiscard]] bool containsText(const types::ObjectId &id) const
    {
        return m_TextObjects.contains(id);
    }

    const types::Transform &transform(const types::ObjectId &id) const
    {
        return m_Transforms.at(id);
    };
    const types::Color4 &color(const types::ObjectId &id) const
    {
        return m_Colors.at(id);
    };
    types::TextData &text(const types::ObjectId &textId)
    {
        return m_Texts.at(textId);
    };
    const types::RenderComponent &renderable(const types::ObjectId &id) const
    {
        return m_Renderables.at(id);
    };

    [[nodiscard]] types::MaterialAsset *mat(const types::MaterialHandle &h) const
    {
        return m_Ctx->materials.asset(h);
    };

    [[nodiscard]] MaterialManager &materials()
    {
        return m_Ctx->materials;
    };
    [[nodiscard]] const MaterialManager &materials() const
    {
        return m_Ctx->materials;
    };

    [[nodiscard]] CollisionManager &collisions()
    {
        return m_Collisions;
    };
    [[nodiscard]] const CollisionManager &collisions() const
    {
        return m_Collisions;
    };

    void setTransform(const types::ObjectId &id, const types::Transform &t)
    {
        if (frozen || paused)
            return;
        m_Transforms.at(id) = t;
    };
    void setColor(const types::ObjectId &id, const types::Color4 &c)
    {
        if (frozen)
            return;
        m_Colors.at(id) = c;
    };
    void setRenderable(const types::ObjectId &id, const types::RenderComponent &rc)
    {
        if (frozen)
            return;
        m_Renderables.at(id) = rc;
    };

    auto &getTransforms()
    {
        return m_Transforms;
    };
    auto &getColors()
    {
        return m_Colors;
    };

    // void destroyObject(types::ObjectId id);
    void removeText(const types::ObjectId id)
    {
        m_Texts.erase(id);
    };

    const std::vector<types::DrawItem> &buildRenderList();

    std::vector<types::BatchGroup> getBatchedRenderList();

    void getTextRenderList(std::vector<types::TextData> &out) const;
    void clear();

    void removeObject(const types::ObjectId &objId);

    Camera &camera()
    {
        return m_Camera;
    };
    const Camera &camera() const
    {
        return m_Camera;
    };

    void computeCamera(const float &worldWidth, const float &worldHeight, const float &dt);

    bool itemsDirty = false;
    bool paused = false;
    bool frozen = false;

  private:
    types::ObjectId composeObjectId(const types::ObjectId localId) const
    {
        // allows for up to 255 scenes with roughly 16e6 objects each
        return (m_Id << 24U) | (localId & 0xFFFFFFU);
    }

    types::SceneId m_Id;
    types::ObjectId m_NextId = 1;
    types::ObjectId m_NextTextId = 1;

    std::unordered_map<types::ObjectId, std::shared_ptr<Object>> m_Objects;
    std::unordered_map<types::ObjectId, std::shared_ptr<TextObject>> m_TextObjects;

    std::unordered_map<types::ObjectId, types::Transform> m_Transforms;
    std::unordered_map<types::ObjectId, types::Color4> m_Colors;
    std::unordered_map<types::ObjectId, types::RenderComponent> m_Renderables;
    std::unordered_map<types::ObjectId, types::TextData> m_Texts;

    std::shared_ptr<WorldContext> m_Ctx;
    CollisionManager m_Collisions;
    Camera m_Camera;

    std::vector<types::DrawItem> m_Items;
};

} // namespace grafyte
