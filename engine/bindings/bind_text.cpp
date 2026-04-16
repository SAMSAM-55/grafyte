#include "bindings_common.hpp"

void bindText(const py::module_ &m)
{
    const auto textObjectColor = [](const grafyte::TextObject &self) {
        const auto [r, g, b, a] = self.getColor();
        return py::make_tuple(r, g, b, a);
    };

    const auto uiTextColor = [](const grafyte::ui::text::Text &self) {
        const auto [r, g, b, a] = self.getColor();
        return py::make_tuple(r, g, b, a);
    };

    py::class_<grafyte::TextObject, std::shared_ptr<grafyte::TextObject>>(m, "TextObject")
        .def_property_readonly("text", &grafyte::TextObject::getText)
        .def_property_readonly("scale", &grafyte::TextObject::getScale)
        .def_property_readonly("color", textObjectColor)

        // set_text(text)
        .def("set_text", &grafyte::TextObject::setText, py::arg("text"))

        // set_scale(scale)
        .def("set_scale", &grafyte::TextObject::setScale, py::arg("scale"))

        // set_color(r, g, b, a)
        .def(
            "set_color",
            [](const grafyte::TextObject &self, const float &r, const float &g, const float &b, const float &a) {
                self.setColor({r, g, b, a});
            },
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"));

    py::class_<grafyte::ui::text::Text, std::shared_ptr<grafyte::ui::text::Text>>(m, "Text")
        .def_property_readonly("text", &grafyte::ui::text::Text::getText)
        .def_property_readonly("scale", &grafyte::ui::text::Text::getScale)
        .def_property_readonly("color", uiTextColor)

        // set_text(text)
        .def("set_text", &grafyte::ui::text::Text::setText, py::arg("text"))

        // set_scale(scale)
        .def("set_scale", &grafyte::ui::text::Text::setScale, py::arg("scale"))

        // set_color(r, g, b, a)
        .def(
            "set_color",
            [](grafyte::ui::text::Text &self, const float r, const float g, const float b, const float a) {
                self.setColor({r, g, b, a});
            },
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"));
}
