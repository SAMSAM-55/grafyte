#include "Scene.h"

#include <algorithm>
#include <map>
#include <ranges>

#include "glm/gtc/matrix_transform.hpp"

namespace grafyte
{
Scene::Scene(std::shared_ptr<WorldContext> ctx, const types::SceneId &id) : m_Id(id), m_Ctx(std::move(ctx))
{
}

std::shared_ptr<Object> Scene::spawnObject(const types::MeshAsset &mesh, const types::MaterialAsset &mat,
                                           const types::Vec2 &pos, const int &zIndex, types::PrimitiveGeometry geo)
{
    const types::ObjectId id = allocateId();

    const auto meshH = m_Ctx->meshes.createAsset(mesh, id);
    const auto matH = m_Ctx->materials.createAsset(mat, id);
    m_Ctx->meshes.upload(meshH);
    m_Ctx->materials.upload(matH);

    const auto rc = types::RenderComponent{meshH, matH, zIndex};
    m_Renderables.insert_or_assign(id, rc);
    m_Objects.insert_or_assign(id, std::make_shared<Object>(shared_from_this(), id, geo));
    m_Transforms.insert_or_assign(id, types::Transform{.pos = pos, .rot = 0.0f, .scale = {1.0f, 1.0f}});
    m_Colors.insert_or_assign(id, types::Color4{0.0f, 0.0f, 0.0f, 1.0f});

    itemsDirty = true;
    return m_Objects.at(id);
}

std::shared_ptr<TextObject> Scene::spawnTextObject(const types::Vec2 &pos, const std::string &text, const float &size)
{
    const types::ObjectId id = allocateTextId();
    m_Texts.insert_or_assign(id, types::TextData{text, {pos, 0.0f, size, size}, {0.0f, 0.0f, 0.0f, 1.0f}});

    m_TextObjects.insert_or_assign(id, std::make_shared<TextObject>(shared_from_this(), id));
    return m_TextObjects.at(id);
}

const std::vector<types::DrawItem> &Scene::buildRenderList()
{
    if (itemsDirty && !frozen)
    {
        m_Items.clear();
        m_Items.reserve(m_Renderables.size());

        for (const auto &[id, rc] : m_Renderables)
        {
            m_Items.push_back(types::DrawItem{.objectId = id,
                                              .mesh = rc.mesh,
                                              .material = rc.mat,
                                              .zIndex = rc.zIndex,
                                              .transform = m_Transforms.at(id),
                                              .color = {1.0f, 1.0f, 0.0f, 1.0f}});
        }
        std::ranges::sort(m_Items,
                          [](const types::DrawItem &a, const types::DrawItem &b) { return a.zIndex < b.zIndex; });
        itemsDirty = false;
    }

    return m_Items;
}

std::vector<types::BatchGroup> Scene::getBatchedRenderList()
{
    std::map<types::BatchingKey, std::vector<types::DrawItem>> groups;

    for (const auto &items = buildRenderList(); const auto &item : items)
    {
        const auto *mat = m_Ctx->materials.mat(item.material);
        types::BatchingKey key{.th = mat->texture, .sh = mat->shader, .zIndex = item.zIndex};
        groups[key].push_back(item);
    }

    std::vector<types::BatchGroup> result;
    for (auto &pair : groups)
    {
        result.emplace_back(std::move(pair));
    }
    return result;
}

void Scene::getTextRenderList(std::vector<types::TextData> &out) const
{
    out.clear();
    out.reserve(m_Texts.size());
    for (const auto &it : m_Texts | std::views::values)
        out.push_back(it);
}

void Scene::clear()
{
    m_Renderables.clear();
    m_Transforms.clear();
    m_Objects.clear();
    m_TextObjects.clear();
    m_Colors.clear();
    m_Items.clear();
    m_Texts.clear();
    m_Collisions.clear();
    if (m_Camera.followObject != static_cast<types::ObjectId>(-1))
        m_Camera.followObject = static_cast<types::ObjectId>(-1);
    itemsDirty = false;
}

void Scene::removeObject(const types::ObjectId &objId)
{
    if (!m_Objects.contains(objId))
        throw std::runtime_error("Trying to delete Object with invalid id: " + std::to_string(objId));

    m_Renderables.erase(objId);
    m_Transforms.erase(objId);
    m_Colors.erase(objId);
    m_Objects.erase(objId);
    if (m_Camera.followObject == objId)
        m_Camera.followObject = static_cast<types::ObjectId>(-1);

    collisions().removeObject(objId);

    itemsDirty = true;
}

void Scene::computeCamera(const float &worldWidth, const float &worldHeight, const float &dt)
{
    if (frozen)
        return;

    m_Camera.right = worldWidth / 2.0f;
    m_Camera.left = -worldWidth / 2.0f;
    m_Camera.top = worldHeight / 2.0f;
    m_Camera.bottom = -worldHeight / 2.0f;

    m_Camera.projection = glm::ortho(m_Camera.left, m_Camera.right, m_Camera.bottom, m_Camera.top, -1.0f, 1.0f);

    if (m_Camera.followObject != static_cast<types::ObjectId>(-1))
    {
        if (!containsObject(m_Camera.followObject))
        {
            m_Camera.followObject = static_cast<types::ObjectId>(-1);
            return;
        }
        const float t = 1.0f - std::exp(-m_Camera.followSpeed * dt);
        const auto [followX, followY] = transform(m_Camera.followObject).pos + m_Camera.followOffset;

        m_Camera.pos.x = std::lerp(m_Camera.pos.x, followX, t);
        m_Camera.pos.y = std::lerp(m_Camera.pos.y, followY, t);
    }

    m_Camera.view = glm::scale(glm::mat4(1.0f), glm::vec3(m_Camera.zoom)) *
                    glm::translate(glm::mat4(1.0f), -glm::vec3(m_Camera.pos.x, m_Camera.pos.y, 0.0f));
}
} // namespace grafyte
