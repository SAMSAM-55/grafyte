#include "bindings_common.hpp"

#include "bind_application.cpp"
#include "bind_camera.cpp"
#include "bind_input.cpp"
#include "bind_object.cpp"
#include "bind_scene.cpp"
#include "bind_text.cpp"
#include "bind_types.cpp"
#include "bind_ui.cpp"

#ifndef GRAFYTE_PY_MODULE_NAME
#error "GRAFYTE_PY_MODULE_NAME not defined"
#endif

#define PYBIND11_MODULE_NAME_IMPL(x) x
#define PYBIND11_MODULE_NAME(x) PYBIND11_MODULE_NAME_IMPL(x)

PYBIND11_MODULE(GRAFYTE_PY_MODULE_NAME, m)
{
    m.doc() = "Python bindings for the Grafyte engine";

    bindTypes(m);
    bindCamera(m);
    bindObject(m);
    bindScene(m);
    bindText(m);
    bindInput(m);
    bindUi(m);
    bindApplication(m);
}
