#include "Scene.h"

#include <algorithm>
#include <map>
#include <ranges>

#include "glm/gtc/matrix_transform.hpp"

namespace grafyte {
    Scene::Scene(std::shared_ptr<WorldContext> ctx)
        : m_Ctx(std::move(ctx))
    {
    }

    std::shared_ptr<Object> Scene::spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                               const types::Vec2& pos, const int& zIndex,
                                               types::PrimitiveGeometry geo) {
        const types::ObjectId id = allocateId();

        const auto meshH = m_Ctx->meshes.createAsset(mesh, id);
        const auto matH = m_Ctx->materials.createAsset(mat, id);
        m_Ctx->meshes.upload(meshH);
        m_Ctx->materials.upload(matH);

        const auto rc = types::RenderComponent{meshH, matH, zIndex};
        m_Renderables.insert_or_assign(id, rc);
        m_Objects.insert_or_assign(id, std::make_shared<Object>(shared_from_this(), id, geo));
        m_Transforms.insert_or_assign(id, types::Transform{
            .pos = pos,
            .rot = 0.0f,
            .scale = {1.0f, 1.0f}
        });
        m_Colors.insert_or_assign(id, types::Color4{0.0f, 0.0f, 0.0f, 0.0f});

        itemsDirty = true;
        return m_Objects.at(id);
    }

    std::shared_ptr<TextObject> Scene::spawnTextObject(const types::Vec2& pos, const std::string& text, const float& size)
    {
        const types::ObjectId id = allocateTextId();
        m_Texts.insert_or_assign(id, types::TextData{text, {pos, 0.0f, size, size}, {0.0f, 0.0f, 0.0f, 1.0f}});

        m_TextObjects.insert_or_assign(id, std::make_shared<TextObject>(shared_from_this(), id));
        return m_TextObjects.at(id);
    }

    const std::vector<types::DrawItem>& Scene::buildRenderList()
    {
        if (itemsDirty) {
            m_Items.clear();
            m_Items.reserve(m_Renderables.size());

            for (const auto& [id, rc] : m_Renderables) {
                if (!inCamera(id, rc.mesh)) continue;

                m_Items.push_back(types::DrawItem{
                    .objectId = id,
                    .mesh = rc.mesh,
                    .material = rc.mat,
                    .zIndex = rc.zIndex,
                    .transform = m_Transforms.at(id),
                    .color = {1.0f, 1.0f, 0.0f, 1.0f}
                });
            }
            std::ranges::sort(m_Items, [](const types::DrawItem& a, const types::DrawItem& b)
            {
                return a.zIndex < b.zIndex;
            });
            itemsDirty = false;
        }

       return m_Items;
    }

    std::vector<types::BatchGroup> Scene::getBatchedRenderList() {
        std::map<types::BatchingKey, std::vector<types::DrawItem>> groups;

        for (const auto& items = buildRenderList(); const auto& item : items) {
            const auto* mat = m_Ctx->materials.mat(item.material);
            types::BatchingKey key{
                .th = mat->texture,
                .sh = mat->shader,
                .zIndex = item.zIndex
            };
            groups[key].push_back(item);
        }

        std::vector<types::BatchGroup> result;
        for (auto& pair : groups) {
            result.emplace_back(std::move(pair));
        }
        return result;
    }

    void Scene::getTextRenderList(std::vector<types::TextData>& out) const
    {
        out.clear();
        out.reserve(m_Texts.size());
        for (const auto& it : m_Texts | std::views::values) out.push_back(it);
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
    }

    void Scene::computeCamera(const float& worldWidth, const float& worldHeight, const float& dt)
    {
        m_Ctx->camera.right =   worldWidth  / 2.0f;
        m_Ctx->camera.left =   -worldWidth  / 2.0f;
        m_Ctx->camera.top =     worldHeight / 2.0f;
        m_Ctx->camera.bottom = -worldHeight / 2.0f;

        m_Ctx->camera.projection = glm::ortho(
            m_Ctx->camera.left,
            m_Ctx->camera.right,
            m_Ctx->camera.bottom,
            m_Ctx->camera.top,
            -1.0f, 1.0f
            );

        if (m_Ctx->camera.followObject != -1)
        {
            const float t = 1.0f - std::exp(-m_Ctx->camera.followSpeed * dt);
            const auto [followX, followY] = transform(m_Ctx->camera.followObject).pos + m_Ctx->camera.followOffset;

            m_Ctx->camera.pos.x = std::lerp(m_Ctx->camera.pos.x, followX, t);
            m_Ctx->camera.pos.y = std::lerp(m_Ctx->camera.pos.y, followY, t);
        }

        m_Ctx->camera.view =
            glm::scale(glm::mat4(1.0f), glm::vec3(m_Ctx->camera.zoom)) *
            glm::translate(glm::mat4(1.0f), -glm::vec3(m_Ctx->camera.pos.x, m_Ctx->camera.pos.y, 0.0f));
        }

    bool Scene::inCamera(const types::ObjectId& id, const types::MeshHandle& mesh)
    {
        const types::Vec2& pos = transform(id).pos;
        const types::Vec2& size = m_Ctx->meshes.asset(mesh)->scale;

        return pos.x - size.x >= m_Ctx->camera.left
        || pos.x + size.x <= m_Ctx->camera.right
        || pos.y - size.y >= m_Ctx->camera.bottom
        || pos.y + size.y <= m_Ctx->camera.top;
    }
}
