#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>

namespace grafyte {
    Scene::Scene(std::shared_ptr<WorldContext> ctx)
        : m_ctx(std::move(ctx))
    {
    }

    std::shared_ptr<Object> Scene::spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                               const types::Vec2& pos, const int& zIndex,
                                               types::PrimitiveGeometry geo) {
        const types::ObjectId id = allocateId();

        const auto meshH = m_ctx->meshes.createAsset(mesh, id);
        const auto matH = m_ctx->materials.createAsset(mat, id);
        m_ctx->meshes.upload(meshH);
        m_ctx->materials.upload(matH);

        const auto rc = types::RenderComponent{meshH, matH, zIndex};
        m_renderables.insert_or_assign(id, rc);
        m_objects.insert_or_assign(id, std::make_shared<Object>(shared_from_this(), id, geo));
        m_transforms.insert_or_assign(id, types::Transform{
            .pos = pos,
            .rot = 0.0f,
            .scale = {1.0f, 1.0f}
        });
        m_colors.insert_or_assign(id, types::Color4{0.0f, 0.0f, 0.0f, 0.0f});

        itemsDirty = true;
        return m_objects.at(id);
    }

    std::shared_ptr<TextObject> Scene::spawnTextObject(const types::Vec2& pos, const std::string& text, const float& size)
    {
        const types::ObjectId id = allocateTextId();
        m_texts.insert_or_assign(id, types::TextData{text, {pos, 0.0f, size, size}, {0.0f, 0.0f, 0.0f, 1.0f}});

        m_textObjects.insert_or_assign(id, std::make_shared<TextObject>(shared_from_this(), id));
        return m_textObjects.at(id);
    }

    const std::vector<types::DrawItem>& Scene::buildRenderList()
    {
        if (itemsDirty) {
            m_items.clear();
            m_items.reserve(m_renderables.size());

            for (const auto& [id, rc] : m_renderables) {
                m_items.push_back(types::DrawItem{
                    .objectId = id,
                    .mesh = rc.mesh,
                    .material = rc.mat,
                    .zIndex = rc.zIndex,
                    .transform = m_transforms.at(id),
                    .color = {1.0f, 1.0f, 0.0f, 1.0f}
                });
            }
            std::ranges::sort(m_items, [](const types::DrawItem& a, const types::DrawItem& b)
            {
                return a.zIndex < b.zIndex;
            });
            itemsDirty = false;
        }

       return m_items;
    }

    std::vector<types::BatchGroup> Scene::getBatchedRenderList() {
        std::map<types::BatchingKey, std::vector<types::DrawItem>> groups;

        for (const auto& items = buildRenderList(); const auto& item : items) {
            const auto* mat = m_ctx->materials.mat(item.material);
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

    void Scene::GetTextRenderList(std::vector<types::TextData>& out) const
    {
        out.clear();
        out.reserve(m_texts.size());
        for (const auto& it : m_texts | std::views::values) out.push_back(it);
    }

    void Scene::clear()
    {
        m_renderables.clear();
        m_transforms.clear();
        m_objects.clear();
        m_textObjects.clear();
        m_colors.clear();
        m_items.clear();
        m_texts.clear();
    }
}
