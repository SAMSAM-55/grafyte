#include "MeshManager.h"
#include <iostream>

namespace grafyte
{
types::MeshHandle MeshManager::createAsset(const types::MeshAsset &asset, const types::ObjectId &id)
{
    const types::MeshHandle h = {id};
    m_Assets[h] = std::make_unique<types::MeshAsset>(asset);
    return h;
}

types::MeshAsset *MeshManager::asset(const types::MeshHandle &h)
{
    const auto it = m_Assets.find(h);
    if (it == m_Assets.end())
        throw std::runtime_error("[Mesh Manager](asset) Invalid MeshHandle provided.");
    return it->second.get();
}

types::Mesh *MeshManager::mesh(const types::MeshHandle &h)
{
    const auto it = m_Meshes.find(h);
    if (it == m_Meshes.end())
        throw std::runtime_error("[Mesh Manager](mesh) Invalid MeshHandle provided.");
    return &it->second;
}

void MeshManager::upload(const types::MeshHandle &h)
{
    const types::MeshAsset *a = asset(h);
    std::vector<types::Vertex> vertices;
    switch (a->geo)
    {
    case types::QUAD:
        vertices = makeUnitQuad();
        break;
    case types::TRIANGLE:
        vertices = makeUnitTriangle();
        break;
    case types::CUSTOM:
        throw std::runtime_error("[INFO](MeshManager): Custom geometries are not supported yet.");
    }

    for (auto &[pos, _] : vertices)
    {
        pos.x *= a->scale.x;
        pos.y *= a->scale.y;
    }

    m_Meshes.insert_or_assign(h, types::Mesh{std::move(vertices), a->layoutSlots, a->indices});
}

void MeshManager::clear()
{
    m_Assets.clear();
    m_Meshes.clear();
}

std::vector<types::Vertex> MeshManager::makeUnitQuad()
{
    return {{-1.0f, -1.0f, 0.0f, 0.0f},{1.0f, -1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 0.0f, 1.0f}};
}

std::vector<types::Vertex> MeshManager::makeUnitTriangle()
{
    return {
        {0.0f, 1.0f, 0.5f, 1.0f},
        {-1.0f, -1.0f, 0.0f, 0.0f},
        {1.0f, -1.0f, 1.0f, 0.0f},
    };
}
} // namespace grafyte
