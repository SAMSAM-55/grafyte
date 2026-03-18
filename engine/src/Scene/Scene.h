#pragma once

#include <unordered_map>
#include <memory>

#include "types.h"
#include "../World/World.h"
#include "Intermediate/Object.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Text/TextObject.h"

namespace grafyte {
    class Scene {
    public:
        explicit Scene(WorldContext* ctx);

        types::ObjectId allocateId() {return m_nextId++;};
        types::ObjectId allocateTextId() {return m_nextTextId++;};
        std::shared_ptr<Object> spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                            const types::Vec2& pos, const int& zIndex);
        std::shared_ptr<TextObject> spawnTextObject(const types::Vec2& pos, const std::string& text, const float& size);

        types::Transform& transform(const types::ObjectId& id) {return m_transforms[id];};
        types::TextData& text(const types::ObjectId& textId) {return m_texts[textId];};
        types::RenderComponent& renderable(const types::ObjectId& id) {return m_renderables[id];};

        [[nodiscard]] types::MaterialAsset *mat(const types::MaterialHandle &h) const {
            return m_ctx->materials.asset(h);
        };

        [[nodiscard]] MaterialManager& materials() {return m_ctx->materials;};
        [[nodiscard]] const MaterialManager& materials() const {return m_ctx->materials;};

        [[nodiscard]] CollisionManager& collisions() {return m_ctx->collisions;};
        [[nodiscard]] const CollisionManager& collisions() const {return m_ctx->collisions;};

        void setTransform(const types::ObjectId& id, const types::Transform& t) {m_transforms[id] = t;};
        void setRenderable(const types::ObjectId& id, const types::RenderComponent& rc) {m_renderables[id] = rc;};

        const auto& GetTransforms() {return m_transforms;};

        // void destroyObject(types::ObjectId id);
        void RemoveText(const types::ObjectId id) {m_texts.erase(id);};

        const std::vector<types::DrawItem>& buildRenderList();
        void GetTextRenderList(std::vector<types::TextData>& out) const;
        void clear();

        bool itemsDirty = false;
    private:
        types::ObjectId m_nextId = 1;
        types::ObjectId m_nextTextId = 1;

        std::unordered_map<types::ObjectId, std::shared_ptr<Object>> m_objects;
        std::unordered_map<types::ObjectId, std::shared_ptr<TextObject>> m_textObjects;

        std::unordered_map<types::ObjectId, types::Transform> m_transforms;
        std::unordered_map<types::ObjectId, types::RenderComponent> m_renderables;
        std::unordered_map<types::ObjectId, types::TextData> m_texts;

        WorldContext* m_ctx;

        std::vector<types::DrawItem> m_items;
    };

}
