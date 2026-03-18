#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <ranges>

namespace grafyte {
    Scene::Scene(WorldContext *ctx)
        : m_ctx(ctx)
    {
    }

    std::shared_ptr<Object> Scene::spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                               const types::Vec2& pos, const int& zIndex) {
        // std::cout << "[Scene](SpawnObject): this = " << this << std::endl;

        const types::ObjectId id = allocateId();

        // std::cout << "[Scene](SpawnObject): Spawning object with ID: " << id
        //           << " at position: (" << pos.x << ", " << pos.y << ")"
        //           << " with zIndex: " << zIndex << std::endl;

        const auto meshH = m_ctx->meshes.createAsset(mesh, id);
        const auto matH = m_ctx->materials.createAsset(mat, id);
        m_ctx->meshes.upload(meshH);
        m_ctx->materials.upload(matH);

        const auto rc = types::RenderComponent{meshH, matH, zIndex};
        m_renderables.insert_or_assign(id, rc);
        m_objects.insert_or_assign(id, std::make_shared<Object>(id, this));
        m_transforms.insert_or_assign(id, types::Transform{
            .pos = pos,
            .rot = 0.0f,
            .scale = {1.0f, 1.0f}
        });

        const auto& t = m_transforms[id];
        // std::cout << "[Scene](SpawnObject): Object transform set: "
        //           << "pos=(" << t.pos.x << ", " << t.pos.y << "), "
        //           << "rot=" << t.rot << ", "
        //           << "scale=(" << t.scale.x << ", " << t.scale.y << ")" << std::endl;

        itemsDirty = true;
        return m_objects[id];
    }

    std::shared_ptr<TextObject> Scene::spawnTextObject(const types::Vec2& pos, const std::string& text, const float& size)
    {
        const types::ObjectId id = allocateTextId();
        m_texts.insert_or_assign(id, types::TextData{text, {pos, 0.0f, size, size}, {0.0f, 0.0f, 0.0f, 1.0f}});

        m_textObjects.insert_or_assign(id, std::make_shared<TextObject>(this, id));
        return m_textObjects[id];
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
                    .zIndex = rc.zIndex
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
    }
}
