#include "bindings_common.hpp"

void bindUi(const py::module_ &m)
{
    py::class_<grafyte::UIManager, std::shared_ptr<grafyte::UIManager>>(m, "UIManager")
        // add_text(pos_x, pos_y, text, scale, anchor)
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
}
