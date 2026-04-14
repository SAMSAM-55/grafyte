#include "bindings_common.hpp"

void bindCamera(const py::module_ &m)
{
    py::class_<grafyte::Camera, std::shared_ptr<grafyte::Camera>>(m, "Camera")
        .def(py::init<>())
        .def_property_readonly("zoom_v", &grafyte::Camera::getZoom)
        .def_property_readonly("follow_offset_v", &grafyte::Camera::getFollowOffset)
        .def_property_readonly("pos_v", &grafyte::Camera::getPos)

        // move(offset_x, offset_y)
        .def(
            "move",
            [](grafyte::Camera &self, const float &offsetX, const float &offsetY) { self.move({offsetX, offsetY}); },
            py::arg("offset_x"), py::arg("offset_y"))

        // move_to(pos_x, pos_y)
        .def(
            "move_to", [](grafyte::Camera &self, const float &posX, const float &posY) { self.moveTo({posX, posY}); },
            py::arg("pos_x"), py::arg("pos_y"))

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
            py::arg("offset_x"), py::arg("offset_y"));
}
