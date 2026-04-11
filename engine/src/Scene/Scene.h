#pragma once

#include <memory>
#include <unordered_map>

#include "../World/World.h"
#include "Intermediate/Object.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Text/TextObject.h"
#include "types.h"

namespace grafyte
{
struct WorldContext;

class Scene : public std::enable_shared_from_this<Scene>
{
  public:
    explicit Scene(std::shared_ptr<WorldContext> ctx);

    types::ObjectId allocateId()
    {
        return m_NextId++;
    };
    types::ObjectId allocateTextId()
    {
        return m_NextTextId++;
    };
    std::shared_ptr<Object> spawnObject(const types::MeshAsset &mesh, const types::MaterialAsset &mat,
                                        const types::Vec2 &pos, const int &zIndex, types::PrimitiveGeometry geo);
    std::shared_ptr<TextObject> spawnTextObject(const types::Vec2 &pos, const std::string &text, const float &size);

    types::Transform &transform(const types::ObjectId &id)
    {
        return m_Transforms.at(id);
    };
    types::Color4 &color(const types::ObjectId &id)
    {
        return m_Colors.at(id);
    };
    types::TextData &text(const types::ObjectId &textId)
    {
        return m_Texts.at(textId);
    };
    types::RenderComponent &renderable(const types::ObjectId &id)
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
        return m_Ctx->collisions;
    };
    [[nodiscard]] const CollisionManager &collisions() const
    {
        return m_Ctx->collisions;
    };

    void setTransform(const types::ObjectId &id, const types::Transform &t)
    {
        m_Transforms.at(id) = t;
    };
    void setRenderable(const types::ObjectId &id, const types::RenderComponent &rc)
    {
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

    Camera &camera()
    {
        return m_Ctx->camera;
    };
    const Camera &camera() const
    {
        return m_Ctx->camera;
    };

    void computeCamera(const float &worldWidth, const float &worldHeight, const float &dt);
    bool inCamera(const types::ObjectId &id, const types::MeshHandle &mesh);

    bool itemsDirty = false;

  private:
    types::ObjectId m_NextId = 1;
    types::ObjectId m_NextTextId = 1;

    std::unordered_map<types::ObjectId, std::shared_ptr<Object>> m_Objects;
    std::unordered_map<types::ObjectId, std::shared_ptr<TextObject>> m_TextObjects;

    std::unordered_map<types::ObjectId, types::Transform> m_Transforms;
    std::unordered_map<types::ObjectId, types::Color4> m_Colors;
    std::unordered_map<types::ObjectId, types::RenderComponent> m_Renderables;
    std::unordered_map<types::ObjectId, types::TextData> m_Texts;

    std::shared_ptr<WorldContext> m_Ctx;

    std::vector<types::DrawItem> m_Items;
};

} // namespace grafyte
