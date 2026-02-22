#include "Renderer.h"

#include <algorithm>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>

#include "glad/glad.h"

namespace grafyte {
    void Renderer::Draw(const types::DrawItem& item) const {
        std::cout << "[Renderer](Draw): Drawing item for object ID: " << item.objectId << std::endl;
        const types::Material *mat = m_materials.mat(item.material);
        const types::MaterialAsset *matA= m_materials.asset(item.material);
        const types::Mesh *mesh = m_meshes.mesh(item.mesh);


        mat->shader.Bind();
        mesh->va.Bind();
        mesh->ib.Bind();

        if (matA->hasTexture) {
            std::cout << "[Renderer](Draw): Binding texture to slot: " << matA->textureSlot << std::endl;
            glActiveTexture(GL_TEXTURE0 + matA->textureSlot);
            mat->texture.Bind(matA->textureSlot);
            mat->shader.SetUniform1i("u_Texture", matA->textureSlot);
        }

        std::cout << "[Renderer](Draw): glDrawElements. Count: " << mesh->ib.GetCount() << std::endl;
        glDrawElements(GL_TRIANGLES, mesh->ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    glm::mat4 Renderer::computeModel(const types::Transform &t) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(t.pos.x, t.pos.y, 0.0f));
        model = glm::rotate(model, glm::radians(t.rot), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(t.scale.x, t.scale.y, 1.0f));

        return model;
    }

    void Renderer::Render(std::vector<types::DrawItem>& items, const Camera& camera) const
    {
        std::cout << "[Renderer](Render): Starting render of " << items.size() << " items." << std::endl;
        std::ranges::sort(items,
                          [](const types::DrawItem& a, const types::DrawItem& b)
                          {
                              return a.zIndex < b.zIndex;
                          });

        for (const auto& it: items) {
            std::cout << "[Renderer](Render): Processing item for object ID: " << it.objectId << " at zIndex: " << it.zIndex << std::endl;
            const auto* mat = m_materials.mat(it.material);
            if (!mat) throw std::runtime_error("No material found");

            mat->shader.Bind();

            const glm::mat4 model = computeModel(it.transform);
            const glm::mat4 mvp = camera.projection * camera.view * model;
            mat->shader.SetUniformMat4f("u_MVP", mvp);

            Draw(it);
        }
        std::cout << "[Renderer](Render): Render completed." << std::endl;
    }

    Renderer::Renderer(MeshManager &meshes, MaterialManager &materials)
        : m_meshes(meshes), m_materials(materials)
    {
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

}