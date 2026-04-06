// grafyte_bindings.cpp

#include <glad/glad.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "High/Application.h"
#include "Intermediate/Object.h"

#include "Inputs/InputManager.h"

#include "Scene/Scene.h"
#include "Scene/Managers/CollisionManager.h"

namespace py = pybind11;

#ifndef GRAFYTE_PY_MODULE_NAME
#error "GRAFYTE_PY_MODULE_NAME not defined"
#endif

#define PYBIND11_MODULE_NAME_IMPL(x) x
#define PYBIND11_MODULE_NAME(x) PYBIND11_MODULE_NAME_IMPL(x)

PYBIND11_MODULE(GRAFYTE_PY_MODULE_NAME, m)
{
    m.doc() = "Python bindings for the Grafyte engine";

    // Expose Vec2 as a Python type that can be built from a tuple
    py::class_<grafyte::types::Vec2>(m, "Vec2")
        .def(py::init<float, float>(), py::arg("x") = 0.0f, py::arg("y") = 0.0f)
        .def(py::init([](const py::sequence& seq)
        {
            if (py::len(seq) != 2) throw std::runtime_error("Vec2 must have exactly two elements");
            return grafyte::types::Vec2{seq[0].cast<float>(), seq[1].cast<float>()};
        }))
        .def_readonly("x", &grafyte::types::Vec2::x)
        .def_readonly("y", &grafyte::types::Vec2::y);

    py::implicitly_convertible<py::tuple, grafyte::types::Vec2>();
    py::implicitly_convertible<py::list, grafyte::types::Vec2>();

    // Expose grafyte::inputs::Key as a Python enum
    py::enum_<grafyte::inputs::Key>(m, "Key")
        .value("Space", grafyte::inputs::Key::Space)
        .value("Num0", grafyte::inputs::Key::Num0)
        .value("Num1", grafyte::inputs::Key::Num1)
        .value("Num2", grafyte::inputs::Key::Num2)
        .value("Num3", grafyte::inputs::Key::Num3)
        .value("Num4", grafyte::inputs::Key::Num4)
        .value("Num5", grafyte::inputs::Key::Num5)
        .value("Num6", grafyte::inputs::Key::Num6)
        .value("Num7", grafyte::inputs::Key::Num7)
        .value("Num8", grafyte::inputs::Key::Num8)
        .value("Num9", grafyte::inputs::Key::Num9)
        .value("A", grafyte::inputs::Key::A)
        .value("B", grafyte::inputs::Key::B)
        .value("C", grafyte::inputs::Key::C)
        .value("D", grafyte::inputs::Key::D)
        .value("E", grafyte::inputs::Key::E)
        .value("F", grafyte::inputs::Key::F)
        .value("G", grafyte::inputs::Key::G)
        .value("H", grafyte::inputs::Key::H)
        .value("I", grafyte::inputs::Key::I)
        .value("J", grafyte::inputs::Key::J)
        .value("K", grafyte::inputs::Key::K)
        .value("L", grafyte::inputs::Key::L)
        .value("M", grafyte::inputs::Key::M)
        .value("N", grafyte::inputs::Key::N)
        .value("O", grafyte::inputs::Key::O)
        .value("P", grafyte::inputs::Key::P)
        .value("Q", grafyte::inputs::Key::Q)
        .value("R", grafyte::inputs::Key::R)
        .value("S", grafyte::inputs::Key::S)
        .value("T", grafyte::inputs::Key::T)
        .value("U", grafyte::inputs::Key::U)
        .value("V", grafyte::inputs::Key::V)
        .value("W", grafyte::inputs::Key::W)
        .value("X", grafyte::inputs::Key::X)
        .value("Y", grafyte::inputs::Key::Y)
        .value("Z", grafyte::inputs::Key::Z)
        .value("Right", grafyte::inputs::Key::Right)
        .value("Left", grafyte::inputs::Key::Left)
        .value("Down", grafyte::inputs::Key::Down)
        .value("Up", grafyte::inputs::Key::Up)
        .value("LeftShift", grafyte::inputs::Key::LeftShift)
        .value("RightShift", grafyte::inputs::Key::RightShift)
        .export_values();

    py::enum_<grafyte::collision::Direction>(m, "Direction")
        .value("Top", grafyte::collision::Direction::Top)
        .value("Bottom", grafyte::collision::Direction::Bottom)
        .value("Right", grafyte::collision::Direction::Right)
        .value("Left", grafyte::collision::Direction::Left)
        .export_values();

    py::class_<grafyte::collision::AABB>(m, "AABB")
        .def_readonly("pos", &grafyte::collision::AABB::pos)
        .def_readonly("width", &grafyte::collision::AABB::width)
        .def_readonly("height", &grafyte::collision::AABB::height);

    py::class_<grafyte::collision::Hit>(m, "Hit")
        .def_readonly("A", &grafyte::collision::Hit::A)
        .def_readonly("B", &grafyte::collision::Hit::B)
        .def_readonly("collision", &grafyte::collision::Hit::collision)
        .def_readonly("direction", &grafyte::collision::Hit::direction)
        .def("__bool__", [](const grafyte::collision::Hit& self) { return self.collision; });

    py::enum_<grafyte::InputTrigger>(m, "InputTrigger")
        .value("Press", grafyte::Press)
        .value("Hold", grafyte::Hold)
        .value("Release", grafyte::Release)
        .export_values();

    py::class_<grafyte::Object, std::shared_ptr<grafyte::Object>>(m, "Object")
        .def_property_readonly("scale", &grafyte::Object::GetScale)
        .def_property_readonly("pos", &grafyte::Object::GetPosition)
        .def_property_readonly("rot", &grafyte::Object::GetRotation)

        .def("use_texture", &grafyte::Object::SetTexture, py::arg("texture_source_path"), py::arg("slot"))

        .def("set_tint", [](const grafyte::Object& self, const float& r, const float& g, const float& b, const float& strength)
        {
            self.SetTint({r, g, b, strength});
        }, py::arg("tint_r"), py::arg("tint_g"), py::arg("tint_b"), py::arg("strength"))
        .def("set_color", [](const grafyte::Object& self, const float& r, const float& g, const float& b, const float& a)
        {
            self.SetColor({r, g, b, a});
        }, py::arg("color_r"), py::arg("color_g"), py::arg("color_b"), py::arg("color_a"))

        .def("add_collision_box", [](const grafyte::Object& self, const float& pos_x, const float& pos_y,
                                     const float& size_x, const float& size_y)
        {
            auto box = grafyte::collision::AABB{{pos_x, pos_y}, size_x, size_y};
            self.AddCollisionBox(box);
        }, py::arg("size_x"), py::arg("size_y"), py::arg("scale_x"), py::arg("scale_y"))
        .def("collides_with", &grafyte::Object::CollidesWith, py::arg("other"))
        .def("is_colliding", [](const grafyte::Object& self)
        {
            auto* scene = self.GetScene();
            if (!scene) {
                return grafyte::collision::Hit{grafyte::collision::AABB{}, grafyte::collision::AABB{}, false, grafyte::collision::Top};
            }
            return scene->collisions().IsColliding(self.GetId(), *scene);
        })
        .def("enable_auto_collides", &grafyte::Object::EnableAutoCollides, py::arg("order"))

        .def("move", [](const grafyte::Object& self, const float& offset_x, const float& offset_y)
        {
            self.Move({offset_x, offset_y});
        }, py::arg("offset_x"), py::arg("offset_y"))
        .def("move_to", [](const grafyte::Object& self, const float& pos_x, const float& pos_y)
        {
            self.MoveTo({pos_x, pos_y});
        }, py::arg("pos_x"), py::arg("pos_y"))
        .def("rotate", &grafyte::Object::Rotate, py::arg("angle"))
        .def("set_rotation", &grafyte::Object::SetRotation, py::arg("angle"))
        .def("set_scale", py::overload_cast<float>(&grafyte::Object::SetScale, py::const_), py::arg("scale"))
        .def("set_scale", py::overload_cast<grafyte::types::Vec2>(&grafyte::Object::SetScale, py::const_), py::arg("scale"));


    py::class_<grafyte::Scene, std::shared_ptr<grafyte::Scene>>(m, "Scene")
        .def("spawn_object", [](grafyte::Scene& self,
                            const float sx, const float sy,
                            const std::string& shaderSourcePath,
                            const float x, const float y,
                            const bool hasTexture,
                            const int zIndex) {
            grafyte::types::MeshAsset mesh;

            mesh.layoutSlots = {
                grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2}, // pos
                grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2}, // uv
                grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 4}, // color
            };

            mesh.geo = grafyte::types::QUAD;
            mesh.scale = {sx, sy};
            mesh.indices = {
                0, 1, 2,
                2, 3, 0
            };

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
        }, py::arg("scale_x"), py::arg("scale_y"), py::arg("shader_source_path"), py::arg("pos_x"),
        py::arg("pos_y"), py::arg("has_texture"), py::arg("z_index"))
        .def("spawn_text_object", [](grafyte::Scene& self,
            const float x,  const float y,
            const std::string& text,
            const float scale)
        {
            return self.spawnTextObject({x, y}, text, scale);
        }, py::arg("pos_x"), py::arg("pos_y"), py::arg("text"), py::arg("scale"));

    py::class_<grafyte::TextObject, std::shared_ptr<grafyte::TextObject>>(m, "TextObject")
        .def("set_text", &grafyte::TextObject::SetText, py::arg("text"))
        .def("set_scale", &grafyte::TextObject::SetScale, py::arg("scale"))
        .def("set_color", [](const grafyte::TextObject& self, const float& r, const float& g, const float& b, const float& a)
        {
            self.SetColor({r, g, b, a});
        }, py::arg("color_r"), py::arg("color_g"), py::arg("color_b"), py::arg("color_a"));

    py::class_<grafyte::InputManager>(m, "InputManager")
    .def(py::init())

    // inputs
    .def_static(
        "is_key_down",
        &grafyte::InputManager::isKeyDown,
        py::arg("key"),
        "Return current input state"
    )
    .def_static(
        "was_key_pressed",
        &grafyte::InputManager::wasKeyPressed,
        py::arg("key"),
        "Return current input state"
    )
    .def_static(
        "was_key_released",
        &grafyte::InputManager::wasKeyReleased,
        py::arg("key"),
        "Return current input state"
    )

    .def_static(
        "create_action",
        &grafyte::InputManager::createAction,
        py::arg("name"),
        py::arg("keys"),
        py::arg("trigger"),
        "Creates a new input action for the current application"
    )
    .def_static(
        "is_action_active",
        &grafyte::InputManager::isActionActive,
        py::arg("action"),
        "Return current input state"
    );

    py::class_<grafyte::Application>(m, "Application")
        .def_property_readonly_static("input", [](const py::object& self) {
            return static_cast<grafyte::InputManager *>(nullptr);
        })

        .def(py::init<const std::string&, const std::string&>(), py::arg("name"), py::arg("font"))
        .def(py::init<const std::string&>(), py::arg("name"))

        // init(winWidth, winHeight) -> int
        .def(
            "init",
            &grafyte::Application::init,
            py::arg("win_width"),
            py::arg("win_height"),
            "Initialize GLFW, OpenGL and ImGui for this application"
        )

        // shouldClose() -> bool
        .def(
            "should_close",
            &grafyte::Application::shouldClose,
            "Return True if the window should close and update timing"
        )

        // quit()
        .def(
            "quit",
            &grafyte::Application::quit,
            "Exit and kills the application"
        )

        // getNow() -> float
        .def(
            "get_now",
            &grafyte::Application::getNow,
            "Get the current time (glfwGetTime)"
        )

        // get_delta_time() -> float
        .def(
            "get_delta_time",
            &grafyte::Application::getDeltaTime,
            "Get the interval between last and current frame"
        )

        // make_new_scene()
        .def(
            "make_new_scene",
            &grafyte::Application::makeNewScene,
            "Sets the current renderer used by the application"
        )

        // render()
        .def(
            "render",
            &grafyte::Application::render,
            "Render all objects using the internal renderer"
        )

        // inputs
        .def_static(
            "is_key_down",
            &grafyte::Application::isKeyDown,
            py::arg("key"),
            "Return current input state"
        )
        .def_static(
            "was_key_pressed",
            &grafyte::Application::wasKeyPressed,
            py::arg("key"),
            "Return current input state"
        )
        .def_static(
            "was_key_released",
            &grafyte::Application::wasKeyReleased,
            py::arg("key"),
            "Return current input state"
        )

        .def_static(
            "create_input_action",
            &grafyte::Application::createInputAction,
            py::arg("name"),
            py::arg("key"),
            py::arg("trigger"),
            "Creates a new input action for the current application"
        )
        .def_static(
            "is_action_active",
            &grafyte::Application::isActionActive,
            py::arg("action"),
            "Return current input state"
        )

        // setClearColor()
        .def(
            "set_clear_color",
            &grafyte::Application::setClearColor,
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a"),
            "Set clear color (currently no-op in C++ implementation)"
        );
}
