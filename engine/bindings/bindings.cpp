// grafyte_bindings.cpp

#include <glad/glad.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "High/Application.h"
#include "Intermediate/Object.h"
#include <glm/gtc/type_ptr.hpp>

#include "Inputs/InputManager.h"

namespace py = pybind11;

#ifndef GRAFYTE_PY_MODULE_NAME
#error "GRAFYTE_PY_MODULE_NAME not defined"
#endif

#define PYBIND11_MODULE_NAME_IMPL(x) x
#define PYBIND11_MODULE_NAME(x) PYBIND11_MODULE_NAME_IMPL(x)

PYBIND11_MODULE(GRAFYTE_PY_MODULE_NAME, m)
{
    m.doc() = "Python bindings for the Grafyte engine";

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

    py::class_<grafyte::Object, std::shared_ptr<grafyte::Object>>(m, "Object")
        .def(py::init([](const py::buffer& positions, const unsigned int vertexCount, const py::buffer &indices,
                         const std::string &shaderSourcePath, const float &pos_x, const float &pos_y, const int &layer) {
            const py::buffer_info pos_info = positions.request();
            const py::buffer_info idx_info = indices.request();

            if (pos_info.format != py::format_descriptor<float>::value)
                throw std::runtime_error("Positions must be a float buffer");
            if (idx_info.format != py::format_descriptor<unsigned int>::value)
                throw std::runtime_error("Indices must be an unsigned int buffer");

            return new grafyte::Object(
                pos_info.ptr,
                static_cast<unsigned int>(pos_info.size * sizeof(float)),
                vertexCount,
                static_cast<const unsigned int*>(idx_info.ptr),
                static_cast<unsigned int>(idx_info.size),
                shaderSourcePath,
                pos_x,
                pos_y,
                layer
            );
        }), py::arg("positions"), py::arg("vertex_count"), py::arg("indices"), py::arg("shader_source_path"),
             py::arg("pos_x"), py::arg("pos_y"), py::arg("layer"))
        .def("set_texture", &grafyte::Object::SetTexture, py::arg("texture_source_path"), py::arg("slot"))
        .def("add_layout_slot", &grafyte::Object::AddLayoutSlot, py::arg("type"), py::arg("size"))
        .def("add_buffer_to_vertex_array", &grafyte::Object::AddBufferToVertexArray)
        .def("set_shader_uniform_1f", &grafyte::Object::SetShaderUniform1f, py::arg("name"), py::arg("value"))
        .def("set_shader_uniform_4f", &grafyte::Object::SetShaderUniform4f, py::arg("name"), py::arg("float_x"),
             py::arg("float_y"), py::arg("float_z"), py::arg("float_w"))
        .def("set_shader_uniform_mat4f", [](grafyte::Object& self, const std::string& name, const py::buffer& matrix) {
            const py::buffer_info info = matrix.request();
            if (info.format != py::format_descriptor<float>::value)
                throw std::runtime_error("Matrix must be a float buffer");
            if (info.size != 16)
                throw std::runtime_error("Matrix must have 16 elements");

            self.SetShaderUniformMat4f(name, glm::make_mat4(static_cast<const float*>(info.ptr)));
        }, py::arg("name"), py::arg("matrix"))
        .def("_native_move", &grafyte::Object::Move, py::arg("offset_x"), py::arg("offset_y"))
        .def("_native_move_to", &grafyte::Object::MoveTo, py::arg("pos_x"), py::arg("pos_y"));

    py::class_<grafyte::Renderer>(m, "Renderer")
        .def(py::init<>())
        .def("add_object", &grafyte::Renderer::AddObject, py::arg("obj"), "Add an object to the renderer")
        .def("render", &grafyte::Renderer::Render, "Render all objects")
        .def("clear", &grafyte::Renderer::Clear, "Clear the screen");

    py::class_<grafyte::Application>(m, "Application")
        .def(py::init<const std::string&, const std::string&>(), py::arg("name"), py::arg("font"))

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

        // use_renderer()
        .def(
            "use_renderer",
            &grafyte::Application::useRenderer,
            py::arg("renderer"),
            "Sets the current renderer used by the application"
        )

        // render()
        .def(
            "render",
            &grafyte::Application::render,
            "Render all objects using the internal renderer"
        )

        // add_text() -> int
        .def(
            "_native_add_text",
            &grafyte::Application::addText,
            py::arg("text"),
            py::arg("scale"),
            py::arg("pos_x"),
            py::arg("pos_y"),
            "Adds the given text to the app"
        )

        // remove_text()
        .def(
        "remove_text",
        &grafyte::Application::removeText,
        py::arg("id"),
        "Removes the given text from the app"
        )

        // inputs
        .def_static(
            "is_key_down",
            &grafyte::Application::isKeyDown,
            py::arg("key"),
            "Return current input state (placeholder for now)"
        )
        .def_static(
            "was_key_pressed",
            &grafyte::Application::wasKeyPressed,
            py::arg("key"),
            "Return current input state (placeholder for now)"
        )
        .def_static(
            "was_key_released",
            &grafyte::Application::wasKeyReleased,
            py::arg("key"),
            "Return current input state (placeholder for now)"
        )

        .def_static(
            "create_input_action",
            &grafyte::Application::createInputAction,
            py::arg("name"),
            py::arg("key"),
            "Creates a new input action for the current application"
        )
        .def_static(
            "is_action_down",
            &grafyte::Application::isActionDown,
            py::arg("action"),
            "Return current input state (placeholder for now)"
        )
        .def_static(
            "was_action_pressed",
            &grafyte::Application::wasActionPressed,
            py::arg("action"),
            "Return current input state (placeholder for now)"
        )
        .def_static(
            "was_action_released",
            &grafyte::Application::wasActionReleased,
            py::arg("action"),
            "Return current input state (placeholder for now)"
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
