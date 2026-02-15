cmake_minimum_required(VERSION 3.20)
project(Grafyte LANGUAGES C CXX)

cmake_policy(SET CMP0148 NEW)

set(PYBIND11_FINDPYTHON ON)
set(Python_FIND_REGISTRY NEVER)
set(Python_FIND_STRATEGY LOCATION)

# Le tag est fourni par preset (py310/py311/py312/py313)
set(GRAFYTE_PY_TAG "py313" CACHE STRING "Python tag: py310/py311/py312/py313")
set(MODNAME "grafyte_${GRAFYTE_PY_TAG}")

# IMPORTANT : dans un preset on passe Python_EXECUTABLE
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)

include(FetchContent)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v3.0.1
)
FetchContent_MakeAvailable(pybind11)

# --- Core ---
add_library(grafyte_core STATIC
        vendor/stb_image/stb_image.cpp

        src/High/Application.cpp
        src/Intermediate/Object.cpp
        src/Intermediate/Renderer.cpp
        src/GL-Core/IndexBuffer.cpp
        src/GL-Core/Shader.cpp
        src/GL-Core/Texture.cpp
        src/GL-Core/VertexArray.cpp
        src/GL-Core/VertexBuffer.cpp
)

target_include_directories(grafyte_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
target_compile_features(grafyte_core PUBLIC cxx_std_20)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(grafyte_core PUBLIC
        glfw
        GLEW::GLEW
        Freetype::Freetype
        OpenGL::GL
)

# --- Python module ---
pybind11_add_module(${MODNAME} MODULE bindings/bindings.cpp)
target_link_libraries(${MODNAME} PRIVATE grafyte_core)

target_compile_definitions(${MODNAME} PRIVATE
        GRAFYTE_PY_MODULE_NAME=${MODNAME}
)

# où sort le .pyd dans ce build-dir
set_target_properties(${MODNAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
)

# dossier commun où tu copies les 4 .pyd
set(GRAFYTE_INTERNAL_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/__grafyte_internal"
        CACHE PATH "Destination folder for built .pyd modules"
)

message(STATUS "Will copy .pyd to: ${GRAFYTE_INTERNAL_DIR}")

# Copie + renommage stable (ex: grafyte_py313.pyd)
add_custom_command(TARGET ${MODNAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GRAFYTE_INTERNAL_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${MODNAME}>"
        "${GRAFYTE_INTERNAL_DIR}/${MODNAME}.pyd"
        COMMENT "Copy + rename -> ${GRAFYTE_INTERNAL_DIR}/${MODNAME}.pyd"
)
