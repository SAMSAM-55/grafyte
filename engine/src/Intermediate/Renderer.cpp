#include "Renderer.h"

#include <algorithm>
#include "glad/glad.h"

namespace grafyte {

    // Source - https://stackoverflow.com/a/72025044
    // Posted by 463035818_is_not_an_ai
    // Retrieved 2026-02-15, License - CC BY-SA 4.0

    template <typename F>
    void my_sort(std::vector<std::shared_ptr<Object>>& objects, F f) {
        std::sort(objects.begin(), objects.end(),
                  [f](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) {
                      return f(a) < f(b);
        });
    }

    void Renderer::Draw(const grafyte::ObjectRenderData& data) {
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

    void Renderer::Render(const glm::mat4& MVP)
    {
        my_sort(m_obj,[](const std::shared_ptr<Object>& obj) { return obj->GetRenderData().zIndex; });

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