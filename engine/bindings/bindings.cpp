#include "glad/glad.h"

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "High/Application.h"
#include "Intermediate/Object.h"

#include "Inputs/InputManager.h"

#include "Scene/Managers/CollisionManager.h"
#include "Scene/Scene.h"

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
        .def(py::init([](const py::sequence &seq) {
            if (py::len(seq) != 2)
                throw std::runtime_error("Vec2 must have exactly two elements");
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

    py::enum_<grafyte::ui::text::Anchor>(m, "Anchor")
        .value("TopLeft", grafyte::ui::text::Anchor::TopLeft)
        .value("BottomLeft", grafyte::ui::text::Anchor::BottomLeft)
        .value("TopRight", grafyte::ui::text::Anchor::TopRight)
        .value("BottomRight", grafyte::ui::text::Anchor::BottomRight)
        .value("Center", grafyte::ui::text::Anchor::Center)
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
        .def("__bool__", [](const grafyte::collision::Hit &self) { return self.collision; });

    py::enum_<grafyte::InputTrigger>(m, "InputTrigger")
        .value("Press", grafyte::Press)
        .value("Hold", grafyte::Hold)
        .value("Release", grafyte::Release)
        .export_values();

    py::class_<grafyte::Camera, std::shared_ptr<grafyte::Camera>>(m, "Camera")
        .def(py::init<>())

        // move(offset_x, offset_y)
        .def(
            "move",
            [](grafyte::Camera &self, const float &offsetX, const float &offsetY) { self.move({offsetX, offsetY}); },
            py::arg("offset_y"), py::arg("offset_x"))
        // move_to(pos_x, pos_y)
        .def(
            "move_to", [](grafyte::Camera &self, const float &posX, const float &posY) { self.moveTo({posX, posY}); },
            py::arg("pos_y"), py::arg("pos_x"))

        // zoom(zoom)
        .def("zoom", &grafyte::Camera::setZoom, py::arg("zoom"))

        // follow(object)
        .def(
            "follow", [](grafyte::Camera &self, const grafyte::Object &object) { self.follow(object.getId()); },
            py::arg("object"))

        // follow_offset(offset_x, offset_y)
        .def(
            "follow_offset",
            [](grafyte::Camera &self, const float &offsetX, const float &offsetY) {
                self.setFollowOffset({offsetX, offsetY});
            },
            py::arg("offset_y"), py::arg("offset_x"));

    py::class_<grafyte::Object, std::shared_ptr<grafyte::Object>>(m, "Object")
        .def_property_readonly("scale", &grafyte::Object::getScale)
        .def_property_readonly("pos", &grafyte::Object::getPosition)
        .def_property_readonly("rot", &grafyte::Object::getRotation)

        // use_texture(texture_source_path, slot)
        .def("use_texture", &grafyte::Object::setTexture, py::arg("texture_source_path"), py::arg("slot"))

        // set_color(color_r, color_g, color_b, color_a)
        .def(
            "set_color",
            [](const grafyte::Object &self, const float &r, const float &g, const float &b, const float &a) {
                self.setColor({r, g, b, a});
            },
            py::arg("color_r"), py::arg("color_g"), py::arg("color_b"), py::arg("color_a"))

        // add_collision_box(pos_x, pos_y, size_x, size_y)
        .def(
            "add_collision_box",
            [](const grafyte::Object &self, const float &posX, const float &posY, const float &sizeX,
               const float &sizeY) {
                auto box = grafyte::collision::AABB{{posX, posY}, sizeX, sizeY};
                self.addCollisionBox(box);
            },
            py::arg("pos_x"), py::arg("pos_y"), py::arg("size_x"), py::arg("size_y"))

        // collides_with(other) -> Hit
        .def("collides_with", &grafyte::Object::collidesWith, py::arg("other"))

        // is_colliding() -> list[Hit]
        .def("is_colliding",
             [](const grafyte::Object &self) -> std::vector<grafyte::collision::Hit> {
                 auto *scene = self.getScene();
                 if (!scene)
                 {
                     return {grafyte::collision::Hit{grafyte::collision::AABB{}, grafyte::collision::AABB{}, false,
                                                     grafyte::collision::Top}};
                 }
                 return scene->collisions().isColliding(self.getId(), *scene);
             })

        // enable_auto_collides(order)
        .def("enable_auto_collides", &grafyte::Object::enableAutoCollides, py::arg("order"))

        // move(offset_x, offset_y)
        .def(
            "move",
            [](const grafyte::Object &self, const float &offsetX, const float &offsetY) {
                self.move({offsetX, offsetY});
            },
            py::arg("offset_x"), py::arg("offset_y"))

        // move_to(pos_x, pos_y)
        .def(
            "move_to",
            [](const grafyte::Object &self, const float &posX, const float &posY) { self.moveTo({posX, posY}); },
            py::arg("pos_x"), py::arg("pos_y"))

        // rotate(angle)
        .def("rotate", &grafyte::Object::rotate, py::arg("angle"))

        // set_rotation(angle)
        .def("set_rotation", &grafyte::Object::setRotation, py::arg("angle"))

        // set_scale(scale_x, scale_y)
        .def(
            "set_scale",
            [](const grafyte::Object &self, const float &scaleX, const float &scaleY) {
                self.setScale({scaleX, scaleY});
            },
            py::arg("scale_x"), py::arg("scale_y"))

        // set_scale(scale)
        .def("set_scale", py::overload_cast<grafyte::types::Vec2>(&grafyte::Object::setScale, py::const_),
             py::arg("scale"));

    py::class_<grafyte::Scene, std::shared_ptr<grafyte::Scene>>(m, "Scene")
        // spawn_object(scale_x, scale_y, shader_source_path, pos_x, pos_y, has_texture, z_index) -> Object
        .def(
            "spawn_object",
            [](grafyte::Scene &self, const float sx, const float sy, const std::string &shaderSourcePath, const float x,
               const float y, const bool hasTexture, const int zIndex) {
                grafyte::types::MeshAsset mesh;

                mesh.layoutSlots = {
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2}, // pos
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 2}, // uv
                    grafyte::types::LayoutSlot{grafyte::types::AttribType::Float, 4}, // color
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
            py::arg("scale_x"), py::arg("scale_y"), py::arg("shader_source_path"), py::arg("pos_x"), py::arg("pos_y"),
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

    py::class_<grafyte::TextObject, std::shared_ptr<grafyte::TextObject>>(m, "TextObject")
        // set_text(text)
        .def("set_text", &grafyte::TextObject::setText, py::arg("text"))

        // set_scale(scale)
        .def("set_scale", &grafyte::TextObject::setScale, py::arg("scale"))

        // set_color(color_r, color_g, color_b, color_a)
        .def(
            "set_color",
            [](const grafyte::TextObject &self, const float &r, const float &g, const float &b, const float &a) {
                self.setColor({r, g, b, a});
            },
            py::arg("color_r"), py::arg("color_g"), py::arg("color_b"), py::arg("color_a"));

    py::class_<grafyte::InputManager>(m, "InputManager")
        .def(py::init())

        // inputs
        .def_static("is_key_down", &grafyte::InputManager::isKeyDown, py::arg("key"))
        .def_static("was_key_pressed", &grafyte::InputManager::wasKeyPressed, py::arg("key"))
        .def_static("was_key_released", &grafyte::InputManager::wasKeyReleased, py::arg("key"))

        .def_static("create_action", &grafyte::InputManager::createAction, py::arg("name"), py::arg("keys"),
                    py::arg("trigger"), "Creates a new input action for the current application")
        .def_static("is_action_active", &grafyte::InputManager::isActionActive, py::arg("action"));

    py::class_<grafyte::UIManager, std::shared_ptr<grafyte::UIManager>>(m, "UIManager")
        // add_text(pos_x, pos_y, text, scale)
        .def(
            "add_text",
            [](grafyte::UIManager &self, const float posX, const float posY, const std::string &text, const float scale,
               const grafyte::ui::text::Anchor &anchor) { return self.addText({posX, posY}, text, scale, anchor); },
            py::arg("pos_x"), py::arg("pos_y"), py::arg("text"), py::arg("scale"), py::arg("anchor"))

        // remove_text(text)
        .def(
            "remove_text",
            [](grafyte::UIManager &self, const grafyte::ui::text::Text &text) { self.removeText(text.getId()); },
            py::arg("text"));

    py::class_<grafyte::ui::text::Text, std::shared_ptr<grafyte::ui::text::Text>>(m, "Text")
        // set_text(text)
        .def("set_text", &grafyte::ui::text::Text::setText, py::arg("text"))

        // set_color(color_r, color_g, color_b, color_a)
        .def(
            "set_color",
            [](grafyte::ui::text::Text &self, const float colorR, const float colorG, const float colorB,
               const float colorA) { self.setColor({colorR, colorG, colorB, colorA}); },
            py::arg("color_r"), py::arg("color_g"), py::arg("color_b"), py::arg("color_a"))

        // set_scale(scale)
        .def("set_scale", &grafyte::ui::text::Text::setScale, py::arg("scale"));

    py::class_<grafyte::Application>(m, "Application")
        .def_property_readonly_static(
            "input", [](const py::object &self) { return static_cast<grafyte::InputManager *>(nullptr); })

        .def(py::init<const std::string &, const std::string &>(), py::arg("name"), py::arg("font"))
        .def(py::init<const std::string &>(), py::arg("name"))

        // init(win_width, win_height) -> int
        .def("init", &grafyte::Application::init, py::arg("win_width"), py::arg("win_height"))

        // shouldClose() -> bool
        .def("should_close", &grafyte::Application::shouldClose)

        // quit()
        .def("quit", &grafyte::Application::quit)

        // getNow() -> float
        .def("get_now", &grafyte::Application::getNow)

        // get_delta_time() -> float
        .def("get_delta_time", &grafyte::Application::getDeltaTime)

        // make_new_scene()
        .def("make_new_scene", &grafyte::Application::makeNewScene)

        // make_new_ui()
        .def("make_new_ui", &grafyte::Application::makeNewUI)

        // render()
        .def("render", &grafyte::Application::render)

        // set_clear_color(r, g, b, a)
        .def("set_clear_color", &grafyte::Application::setClearColor, py::arg("r"), py::arg("g"), py::arg("b"),
             py::arg("a"));
}
