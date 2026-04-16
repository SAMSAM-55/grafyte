#include "bindings_common.hpp"

void bindApplication(const py::module_ &m)
{
    py::class_<grafyte::Application>(m, "Application")
        .def_property_readonly_static(
            "input", [](const py::object &self) { return static_cast<grafyte::InputManager *>(nullptr); })

        .def(py::init<const std::string &, const std::string &>(), py::arg("name"), py::arg("font"))

        // init(win_width, win_height)
        .def("init", &grafyte::Application::init, py::arg("win_width"), py::arg("win_height"))

        // should_close() -> bool
        .def("should_close", &grafyte::Application::shouldClose)

        // quit()
        .def("quit", &grafyte::Application::quit)

        // get_now() -> float
        .def("get_now", &grafyte::Application::getNow)

        // get_delta_time() -> float
        .def("get_delta_time", &grafyte::Application::getDeltaTime)

        // make_new_scene() -> Scene
        .def("make_new_scene", &grafyte::Application::makeNewScene)

        // set_active_scene(scene)
        .def(
            "set_active_scene",
            [](grafyte::Application &self, const grafyte::Scene &scene) { self.setActiveScene(scene.getId()); },
            py::arg("scene"))

        // get_active_scene() -> Scene
        .def("get_active_scene", &grafyte::Application::getActiveScene)

        // make_new_ui() -> UIManager
        .def("make_new_ui", &grafyte::Application::makeNewUI)

        // set_active_ui(ui)
        .def(
            "set_active_ui",
            [](grafyte::Application &self, const grafyte::UIManager &ui) { self.setActiveUI(ui.getId()); },
            py::arg("ui"))

        // get_active_ui() -> UI
        .def("get_active_ui", &grafyte::Application::getActiveUI)

        // render()
        .def("render", &grafyte::Application::render)

        // set_clear_color(r, g, b, a)
        .def("set_clear_color", &grafyte::Application::setClearColor, py::arg("r"), py::arg("g"), py::arg("b"),
             py::arg("a"));
}
