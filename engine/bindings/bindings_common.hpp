#pragma once

#include "glad/glad.h"

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "High/Application.h"
#include "Inputs/InputManager.h"
#include "Intermediate/Object.h"
#include "Scene/Managers/CollisionManager.h"
#include "Scene/Scene.h"

namespace py = pybind11;

void bindTypes(const py::module_ &m);
void bindCamera(const py::module_ &m);
void bindObject(const py::module_ &m);
void bindScene(const py::module_ &m);
void bindText(const py::module_ &m);
void bindInput(const py::module_ &m);
void bindUi(const py::module_ &m);
void bindApplication(const py::module_ &m);
