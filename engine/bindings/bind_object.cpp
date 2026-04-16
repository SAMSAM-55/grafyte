#include "bindings_common.hpp"

void bindObject(const py::module_ &m)
{
    const auto objectColor = [](const grafyte::Object &self) {
        const auto [r, g, b, a] = self.getColor();
        return py::make_tuple(r, g, b, a);
    };

    py::class_<grafyte::Object, std::shared_ptr<grafyte::Object>>(m, "Object")
        .def_property_readonly("scale", &grafyte::Object::getScale)
        .def_property_readonly("pos", &grafyte::Object::getPosition)
        .def_property_readonly("rot", &grafyte::Object::getRotation)
        .def_property_readonly("alive", &grafyte::Object::isAlive)
        .def_property_readonly("color", objectColor)

        // use_texture(texture_source_path, slot)
        .def("use_texture", &grafyte::Object::setTexture, py::arg("texture_source_path"), py::arg("slot"))

        // set_color(r, g, b, a)
        .def(
            "set_color",
            [](const grafyte::Object &self, const float &r, const float &g, const float &b, const float &a) {
                self.setColor({r, g, b, a});
            },
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))

        // add_collision_bow(pos_x, pos_y, size_x, size_y)
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
             py::arg("scale"))

        // kill()
        .def("kill", &grafyte::Object::kill);
}
