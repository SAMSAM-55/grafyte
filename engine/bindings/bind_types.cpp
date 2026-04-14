#include "bindings_common.hpp"

void bindTypes(const py::module_ &m)
{
    // Vec2
    py::class_<grafyte::types::Vec2>(m, "Vec2")
        // __init__(x, y)
        .def(py::init<float, float>(), py::arg("x") = 0.0f, py::arg("y") = 0.0f)

        // __init__(tuple(x, y))
        .def(py::init([](const py::sequence &seq) {
            if (py::len(seq) != 2)
                throw std::runtime_error("Vec2 must have exactly two elements");
            return grafyte::types::Vec2{seq[0].cast<float>(), seq[1].cast<float>()};
        }))

        .def_readonly("x", &grafyte::types::Vec2::x)
        .def_readonly("y", &grafyte::types::Vec2::y);

    py::implicitly_convertible<py::tuple, grafyte::types::Vec2>();
    py::implicitly_convertible<py::list, grafyte::types::Vec2>();

    // Key
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

    // Direction
    py::enum_<grafyte::collision::Direction>(m, "Direction")
        .value("Top", grafyte::collision::Direction::Top)
        .value("Bottom", grafyte::collision::Direction::Bottom)
        .value("Right", grafyte::collision::Direction::Right)
        .value("Left", grafyte::collision::Direction::Left)
        .export_values();

    // Anchor
    py::enum_<grafyte::ui::text::Anchor>(m, "Anchor")
        .value("TopLeft", grafyte::ui::text::Anchor::TopLeft)
        .value("BottomLeft", grafyte::ui::text::Anchor::BottomLeft)
        .value("TopRight", grafyte::ui::text::Anchor::TopRight)
        .value("BottomRight", grafyte::ui::text::Anchor::BottomRight)
        .value("Center", grafyte::ui::text::Anchor::Center)
        .export_values();

    // AABB
    py::class_<grafyte::collision::AABB>(m, "AABB")
        .def_readonly("pos", &grafyte::collision::AABB::pos)
        .def_readonly("width", &grafyte::collision::AABB::width)
        .def_readonly("height", &grafyte::collision::AABB::height);

    // Hit
    py::class_<grafyte::collision::Hit>(m, "Hit")
        .def_readonly("A", &grafyte::collision::Hit::A)
        .def_readonly("B", &grafyte::collision::Hit::B)
        .def_readonly("collision", &grafyte::collision::Hit::collision)
        .def_readonly("direction", &grafyte::collision::Hit::direction)
        .def("__bool__", [](const grafyte::collision::Hit &self) { return self.collision; });

    // InputTrigger
    py::enum_<grafyte::InputTrigger>(m, "InputTrigger")
        .value("Press", grafyte::Press)
        .value("Hold", grafyte::Hold)
        .value("Release", grafyte::Release)
        .export_values();
}
