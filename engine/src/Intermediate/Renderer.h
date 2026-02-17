#pragma once

#include "Object.h"
#include <memory>

namespace grafyte {
    class Renderer
    {
    private:
        std::vector<std::shared_ptr<Object>> m_obj;

        static void Draw(const grafyte::ObjectRenderData& data);
    public:
        inline void AddObject(const std::shared_ptr<Object>& obj) { m_obj.push_back(obj); }

        void Render(const glm::mat4& MVP);
        static void Clear() ;
    };
}