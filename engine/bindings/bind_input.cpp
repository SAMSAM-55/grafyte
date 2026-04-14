#include "bindings_common.hpp"

void bindInput(const py::module_ &m)
{
    py::class_<grafyte::InputManager>(m, "InputManager")
        .def(py::init())

        // is_key_down() -> bool
        .def_static("is_key_down", &grafyte::InputManager::isKeyDown, py::arg("key"))

        // was_key_pressed() -> bool
        .def_static("was_key_pressed", &grafyte::InputManager::wasKeyPressed, py::arg("key"))

        // was_key_released() -> bool
        .def_static("was_key_released", &grafyte::InputManager::wasKeyReleased, py::arg("key"))

        // create_action(name, trigger, keys)
        .def_static("create_action", &grafyte::InputManager::createAction, py::arg("name"), py::arg("trigger"),
                    py::arg("keys"))

        // is_action_active(action) -> bool
        .def_static("is_action_active", &grafyte::InputManager::isActionActive, py::arg("action"));
}
