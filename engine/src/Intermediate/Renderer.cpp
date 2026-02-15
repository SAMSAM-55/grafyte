#include "Renderer.h"

#include "GL/glew.h"

namespace Grafyte {

void Renderer::Draw(const Grafyte::ObjectRenderData& data) {
    data.shader.Bind();
    data.va.Bind();
    data.ib.Bind();

    if (data.hasTexture) {
        glActiveTexture(GL_TEXTURE0 + data.textureSlot);
        data.texture.Bind(data.textureSlot);
        data.shader.SetUniform1i("u_Texture", data.textureSlot);
    }

    glDrawElements(GL_TRIANGLES, data.ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Render(const glm::mat4& MVP) const
{
    for (const std::shared_ptr<Object>& obj : m_obj) {
        obj->SetShaderUniformMat4f("u_MVP", MVP);
        Draw(obj->GetRenderData());
    }
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

}