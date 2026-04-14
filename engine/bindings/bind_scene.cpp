#include "bindings_common.hpp"

void bindScene(const py::module_ &m)
{
    py::class_<grafyte::Scene, std::shared_ptr<grafyte::Scene>>(m, "Scene")
        // spawn_object(pos_x, pos_y, scale_x, scale_y, shader_source_path, has_texture, z_index) -> Object
        .def(
            "spawn_object",
            [](grafyte::Scene &self, const float x, const float y, const float sx, const float sy,
               const std::string &shaderSourcePath, const bool hasTexture, const int zIndex) {
                grafyte::types::MeshAsset mesh;

                mesh.layoutSlots = {
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2},
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2},
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 4},
                };

                mesh.geo = grafyte::types::QUAD;
                mesh.scale = {sx, sy};
                mesh.indices = {0, 1, 2, 2, 3, 0};

                grafyte::types::MaterialAsset mat;
                mat.shaderSourcePath = shaderSourcePath;
                mat.hasTexture = hasTexture;

                if (hasTexture)
                {
                    mat.textureSlot = 1;
                    mat.textureSourcePath = "@embed/Textures/Default";
                }

                const grafyte::types::Vec2 pos{x, y};
                return self.spawnObject(mesh, mat, pos, zIndex, grafyte::types::QUAD);
            },
            py::arg("pos_x"), py::arg("pos_y"), py::arg("scale_x"), py::arg("scale_y"), py::arg("shader_source_path"),
            py::arg("has_texture"), py::arg("z_index"))

        // spawn_text_object(pos_x, pos_y, text, scale) -> TextObject
        .def(
            "spawn_text_object",
            [](grafyte::Scene &self, const float x, const float y, const std::string &text, const float scale) {
                return self.spawnTextObject({x, y}, text, scale);
            },
            py::arg("pos_x"), py::arg("pos_y"), py::arg("text"), py::arg("scale"))

        // get_camera() -> Camera
        .def("get_camera", py::overload_cast<>(&grafyte::Scene::camera), py::return_value_policy::reference);
}
