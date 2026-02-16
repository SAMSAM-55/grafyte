// grafyte_bindings.cpp

#include <glad/glad.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "High/Application.h"
#include "Intermediate/Object.h"
#include <glm/gtc/type_ptr.hpp>

namespace py = pybind11;

#ifndef GRAFYTE_PY_MODULE_NAME
#error "GRAFYTE_PY_MODULE_NAME not defined"
#endif

#define PYBIND11_MODULE_NAME_IMPL(x) x
#define PYBIND11_MODULE_NAME(x) PYBIND11_MODULE_NAME_IMPL(x)

PYBIND11_MODULE(GRAFYTE_PY_MODULE_NAME, m)
{
    m.doc() = "Python bindings for the Grafyte engine";

    py::class_<Grafyte::Object, std::shared_ptr<Grafyte::Object>>(m, "Object")
        .def(py::init([](const py::buffer& positions, const unsigned int vertexCount, const py::buffer &indices,
                         const std::string &shaderSourcePath, const float &pos_x, const float &pos_y, const int &layer) {
            const py::buffer_info pos_info = positions.request();
            const py::buffer_info idx_info = indices.request();

            if (pos_info.format != py::format_descriptor<float>::value)
                throw std::runtime_error("Positions must be a float buffer");
            if (idx_info.format != py::format_descriptor<unsigned int>::value)
                throw std::runtime_error("Indices must be an unsigned int buffer");

            return new Grafyte::Object(
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
        .def("set_texture", &Grafyte::Object::SetTexture, py::arg("texture_source_path"), py::arg("slot"))
        .def("add_layout_slot", &Grafyte::Object::AddLayoutSlot, py::arg("type"), py::arg("size"))
        .def("add_buffer_to_vertex_array", &Grafyte::Object::AddBufferToVertexArray)
        .def("set_shader_uniform_1f", &Grafyte::Object::SetShaderUniform1f, py::arg("name"), py::arg("value"))
        .def("set_shader_uniform_4f", &Grafyte::Object::SetShaderUniform4f, py::arg("name"), py::arg("float_x"),
             py::arg("float_y"), py::arg("float_z"), py::arg("float_w"))
        .def("set_shader_uniform_mat4f", [](Grafyte::Object& self, const std::string& name, const py::buffer& matrix) {
            const py::buffer_info info = matrix.request();
            if (info.format != py::format_descriptor<float>::value)
                throw std::runtime_error("Matrix must be a float buffer");
            if (info.size != 16)
                throw std::runtime_error("Matrix must have 16 elements");

            self.SetShaderUniformMat4f(name, glm::make_mat4(static_cast<const float*>(info.ptr)));
        }, py::arg("name"), py::arg("matrix"))
        .def("_native_move", &Grafyte::Object::Move, py::arg("offset_x"), py::arg("offset_y"))
        .def("_native_move_to", &Grafyte::Object::MoveTo, py::arg("pos_x"), py::arg("pos_y"));

    py::class_<Grafyte::Renderer>(m, "Renderer")
        .def(py::init<>())
        .def("add_object", &Grafyte::Renderer::AddObject, py::arg("obj"), "Add an object to the renderer")
        .def("render", &Grafyte::Renderer::Render, "Render all objects")
        .def("clear", &Grafyte::Renderer::Clear, "Clear the screen");

    py::class_<Grafyte::Application>(m, "Application")
        // Default constructor (you call init() from Python)
        .def(py::init<const std::string&>(), py::arg("name"))

        // init(winWidth, winHeight) -> int
        .def(
            "init",
            &Grafyte::Application::init,
            py::arg("win_width"),
            py::arg("win_height"),
            "Initialize GLFW, OpenGL and ImGui for this application"
        )

        // shouldClose() -> bool
        .def(
            "should_close",
            &Grafyte::Application::shouldClose,
            "Return True if the window should close and update timing"
        )

        // quit()
        .def(
            "quit",
            &Grafyte::Application::quit,
            "Exit and kills the application"
        )

        // getNow() -> float
        .def(
            "get_now",
            &Grafyte::Application::getNow,
            "Get the current time (glfwGetTime)"
        )

        // get_delta_time() -> float
        .def(
            "get_delta_time",
            &Grafyte::Application::getDeltaTime,
            "Get the interval between last and current frame"
        )

        // use_renderer()
        .def(
            "use_renderer",
            &Grafyte::Application::useRenderer,
            py::arg("renderer"),
            "Sets the current renderer used by the application"
        )

        // render()
        .def(
            "render",
            &Grafyte::Application::render,
            "Render all objects using the internal renderer"
        )

        // getCurrentInput() -> int
        .def(
            "get_current_input",
            &Grafyte::Application::getCurrentInput,
            "Return current input state (placeholder for now)"
        )

        // setClearColor()
        .def(
            "set_clear_color",
            &Grafyte::Application::setClearColor,
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a"),
            "Set clear color (currently no-op in C++ implementation)"
        );
}
