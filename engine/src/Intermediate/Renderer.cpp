#include "Renderer.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <glm/ext/matrix_transform.hpp>

#include "glad/glad.h"

#include "GL-Core/VertexBufferLayout.h"

namespace grafyte {
    Renderer::Renderer(MeshManager &meshes, MaterialManager &materials)
        : m_meshes(meshes), m_materials(materials),
          m_vb(nullptr, 0, GL_DYNAMIC_DRAW),
          m_ib(nullptr, 0, GL_DYNAMIC_DRAW)
    {
        m_va.Bind();
        VertexBufferLayout layout;
        layout.Push<float>(2); // pos: Vec2
        layout.Push<float>(2); // texPos: Vec2
        layout.Push<float>(4); // color: glm::vec4
        m_va.AddBuffer(m_vb, layout);
    }

    void Renderer::Draw(const types::BatchGroup& group,
        std::unordered_map<types::ObjectId, types::Transform>& transforms,
        std::unordered_map<types::ObjectId, types::Color4>& colors ) {
        m_vertexScratch.clear();
        m_indexScratch.clear();
        m_vertexScratch.reserve(group.second.size() * 4);
        m_indexScratch.reserve(group.second.size() * 6);

        for (auto& item : group.second) {
            const auto* mesh = m_meshes.mesh(item.mesh);
            if (!mesh) continue;

            const auto vertexOffset = static_cast<uint32_t>(m_vertexScratch.size());
            item.transform = transforms[item.objectId];
            item.color = colors[item.objectId];
            glm::mat4 model = computeModel(item.transform);

            // Add transformed vertices
            for (const auto&[pos, texPos] : mesh->vertices) {
                const glm::vec4 worldPos = model * glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
                m_vertexScratch.push_back({
                    {worldPos.x, worldPos.y},
                    texPos,
                    {item.color.x, item.color.y, item.color.z, item.color.w}
                });
            }

            // Add m_indexScratch with offset
            for (const unsigned int index : mesh->indices) {
                m_indexScratch.push_back(vertexOffset + index);
            }
        }

        if (m_vertexScratch.empty() || m_indexScratch.empty()) return;

        // Setup OpenGL objects
        m_vb.UpdateData(m_vertexScratch.data(), static_cast<unsigned int>(m_vertexScratch.size() * sizeof(types::BatchedVertex)));
        m_ib.UpdateData(m_indexScratch.data(), static_cast<unsigned int>(m_indexScratch.size()));

        m_va.Bind();
        m_ib.Bind();

        glDrawElements(GL_TRIANGLES, m_ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    glm::mat4 Renderer::computeModel(const types::Transform &t) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(t.pos.x, t.pos.y, 0.0f));
        model = glm::rotate(model, glm::radians(t.rot), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(t.scale.x, t.scale.y, 1.0f));

        return model;
    }

    void Renderer::Render(const std::vector<types::BatchGroup>& groups,
                          std::unordered_map<types::ObjectId, types::Transform>& transforms,
                          std::unordered_map<types::ObjectId, types::Color4>& colors,
                          const Camera &camera) {
        // std::cout << "[Renderer](Render): Starting render of " << items.size() << " items." << std::endl;
        for (const auto& group: groups) {
            // std::cout << "[Renderer](Render): Processing item for object ID: " << it.objectId << " at zIndex: " << it.zIndex << std::endl;
            const types::DrawItem& first = group.second[0];
            const auto* mat = m_materials.mat(first.material);
            const auto* matA = m_materials.asset(first.material);
            const Shader* shader = m_materials.shader(mat->shader);
            const Texture* texture = m_materials.texture(mat->texture);

            shader->Bind();
            texture->Bind(matA->textureSlot);

            const glm::mat4 vp = camera.projection * camera.view;
            shader->SetUniformMat4f("u_MVP", vp);
            shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

            if (matA->hasTexture) shader->SetUniform1i("u_Texture", matA->textureSlot);

            Draw(group, transforms, colors);
        }
        // std::cout << "[Renderer](Render): Render completed." << std::endl;
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}