//
// Created by samis on 2/21/2026.
//

#include "MeshManager.h"
#include <iostream>

namespace grafyte {
    void MeshManager::init() {
        m_unitQuad = {m_nextBuiltinId};

        const types::MeshAsset qAsset = {

        };
    }

    types::MeshHandle MeshManager::resolve(const types::DrawItem &item) {
    }

    types::MeshHandle MeshManager::createAsset(const types::MeshAsset &asset, const types::ObjectId& id) {
        const types::MeshHandle h = {id};
        m_assets[h] = std::make_unique<types::MeshAsset>(asset);
        return h;
    }

    types::MeshAsset * MeshManager::asset(const types::MeshHandle& h) {
        const auto it = m_assets.find(h);
        if (it == m_assets.end()) throw std::runtime_error("[Mesh Manager] Invalid MeshHandle provided.");
        return it->second.get();
    }

    types::Mesh * MeshManager::mesh(const types::MeshHandle &h) {
        const auto it = m_meshes.find(h);
        if (it == m_meshes.end()) throw std::runtime_error("[Mesh Manager] Invalid MeshHandle provided.");
        return &it->second;
    }

    void MeshManager::upload(const types::MeshHandle &h) {
        // std::cout << "[MeshManager](Upload): Uploading mesh for ID: " << h.id << std::endl;
        const types::MeshAsset* a = asset(h);
        // std::cout << "[MeshManager](Upload): Mesh size: " << a->bytes.size() << " bytes. Indices count: " << a->indices.size() << std::endl;
        auto vb = VertexBuffer(a->bytes.data(), a->bytes.size());
        auto va = VertexArray();
        auto ib = IndexBuffer(a->indices.data(), a->indices.size());

        auto layout = VertexBufferLayout();
        for (const auto&[type, count]: a->layoutSlots) {
            switch (type) {
                case types::AttribType::Float: layout.Push<float>(count); break;
                case types::AttribType::UInt: layout.Push<unsigned int>(count); break;
                case types::AttribType::UByte: layout.Push<unsigned char>(count); break;
            }
        }
        va.Bind();
        va.AddBuffer(vb, layout);

        m_meshes.insert_or_assign(h, types::Mesh{std::move(va), std::move(ib), std::move(vb), std::move(layout)});
        // std::cout << "[MeshManager](Upload): Mesh uploaded successfully for ID: " << h.id << std::endl;
    }

    void MeshManager::clear()
    {
        m_assets.clear();
        m_meshes.clear();
    }
}
