#include "Renderer.h"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

#include "glad/glad.h"

#include "GL-Core/VertexBufferLayout.h"

#include <algorithm>
#include <array>

namespace grafyte
{
Renderer::Renderer(MeshManager &meshes, MaterialManager &materials)
    : m_Meshes(meshes), m_Materials(materials), m_Vb(nullptr, 0, GL_DYNAMIC_DRAW), m_Ib(nullptr, 0, GL_DYNAMIC_DRAW)
{
    m_Va.bind();
    VertexBufferLayout layout;
    layout.push<float>(2); // pos: Vec2
    layout.push<float>(2); // texPos: Vec2
    layout.push<float>(4); // color: glm::vec4
    m_Va.addBuffer(m_Vb, layout);
}

void Renderer::draw(const types::BatchGroup &group,
                    const std::unordered_map<types::ObjectId, types::Transform> &transforms,
                    const std::unordered_map<types::ObjectId, types::Color4> &colors, const Camera &camera)
{
    m_VertexScratch.clear();
    m_IndexScratch.clear();
    m_VertexScratch.reserve(group.second.size() * 4);
    m_IndexScratch.reserve(group.second.size() * 6);

    for (auto &item : group.second)
    {
        const auto *mesh = m_Meshes.mesh(item.mesh);
        const auto *asset = m_Meshes.asset(item.mesh);
        const auto &transform = transforms.at(item.objectId);

        if (!mesh || !asset)
            continue;

        if (!inCamera(transform.pos, transform.scale, asset->scale, transform.rot, camera))
            continue;

        const auto vertexOffset = static_cast<uint32_t>(m_VertexScratch.size());
        const auto &[cr, cg, cb, ca] = colors.at(item.objectId);
        glm::mat4 model = computeModel(transform);

        // Add transformed vertices
        for (const auto &[pos, texPos] : mesh->vertices)
        {
            const glm::vec4 worldPos = model * glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
            m_VertexScratch.push_back({{worldPos.x, worldPos.y}, texPos, {cr, cg, cb, ca}});
        }

        // Add m_indexScratch with offset
        for (const unsigned int index : mesh->indices)
        {
            m_IndexScratch.push_back(vertexOffset + index);
        }
    }

    if (m_VertexScratch.empty() || m_IndexScratch.empty())
        return;

    // Set up OpenGL objects
    m_Vb.updateData(m_VertexScratch.data(),
                    static_cast<unsigned int>(m_VertexScratch.size() * sizeof(types::BatchedVertex)));
    m_Ib.updateData(m_IndexScratch.data(), static_cast<unsigned int>(m_IndexScratch.size()));

    m_Va.bind();
    m_Ib.bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, m_Ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

glm::mat4 Renderer::computeModel(const types::Transform &t)
{
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(t.pos.x, t.pos.y, 0.0f));
    model = glm::rotate(model, glm::radians(t.rot), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(t.scale.x, t.scale.y, 1.0f));

    return model;
}
std::array<types::Vec2, 4> Renderer::computeConers(const types::Vec2 &pos, const types::Vec2 &scale,
                                                   const types::Vec2 &size, const float &angle)
{
    const auto [hx, hy] = abs(size * scale);
    const float c = std::cos(glm::radians(angle));
    const float s = std::sin(glm::radians(angle));

    std::array<types::Vec2, 4> result = {{{hx, hy}, {-hx, hy}, {-hx, -hy}, {hx, -hy}}};

    for (auto &p : result)
        p = types::Vec2{p.x * c - p.y * s, p.x * s + p.y * c} + pos;

    return result;
}

bool Renderer::inCamera(const types::Vec2 &pos, const types::Vec2 &scale, const types::Vec2 &size, const float &angle,
                        const Camera &camera)
{
    const float halfWidth = (camera.right - camera.left) * 0.5f / camera.zoom;
    const float halfHeight = (camera.top - camera.bottom) * 0.5f / camera.zoom;

    const float camLeft = camera.pos.x - halfWidth;
    const float camRight = camera.pos.x + halfWidth;
    const float camBottom = camera.pos.y - halfHeight;
    const float camTop = camera.pos.y + halfHeight;

    const auto [a, b, c, d] = computeConers(pos, scale, size, angle);
    const float objLeft = std::min(std::min(a.x, b.x), std::min(c.x, d.x));
    const float objRight = std::max(std::max(a.x, b.x), std::max(c.x, d.x));
    const float objBottom = std::min(std::min(a.y, b.y), std::min(c.y, d.y));
    const float objTop = std::max(std::max(a.y, b.y), std::max(c.y, d.y));

    return objRight >= camLeft && objLeft <= camRight && objTop >= camBottom && objBottom <= camTop;
}

void Renderer::render(const std::vector<types::BatchGroup> &groups,
                      const std::unordered_map<types::ObjectId, types::Transform> &transforms,
                      const std::unordered_map<types::ObjectId, types::Color4> &colors, const Camera &camera)
{

    for (const auto &group : groups)
    {
        if (group.second.empty())
            continue;

        const types::DrawItem &first = group.second[0];
        const auto *mat = m_Materials.mat(first.material);
        const auto *matA = m_Materials.asset(first.material);

        if (!mat || !matA)
            continue;

        const Shader *shader = m_Materials.shader(mat->shader);
        if (!shader)
            continue;
        shader->bind();

        if (matA->hasTexture)
        {
            const Texture *texture = m_Materials.texture(mat->texture);
            if (!texture)
                continue;
            texture->bind(matA->textureSlot);
            shader->setUniform1i("u_Texture", static_cast<int>(matA->textureSlot));
        }

        const glm::mat4 vp = camera.projection * camera.view;
        shader->setUniformMat4f("u_MVP", vp);
        shader->setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

        draw(group, transforms, colors, camera);
    }
}

void Renderer::clearScreen()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::clear()
{
    IndexBuffer::unbind();
    VertexArray::unbind();
    VertexBuffer::unbind();

    m_Ib.release();
    m_Va.release();
    m_Vb.release();

    m_IndexScratch.clear();
    m_VertexScratch.clear();

    m_Materials.clear();
    m_Meshes.clear();
}
} // namespace grafyte
