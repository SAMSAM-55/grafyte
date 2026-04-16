cmake_minimum_required(VERSION 3.20)
project(Grafyte LANGUAGES C CXX)

if (POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif ()

if (UNIX AND NOT APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

# ==================================================
# [SECTION] PyBind11 & grafyte config
# ==================================================

set(PYBIND11_FINDPYTHON ON)
set(Python_FIND_REGISTRY NEVER)
set(Python_FIND_STRATEGY LOCATION)

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


# ==================================================
# [SECTION] grafyte core
# ==================================================

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
        src/Text/TextRenderer.cpp
        utils/embedd/EmbeddedAsset.cpp
        src/Inputs/InputManager.cpp
        src/Scene/Managers/CollisionManager.cpp
        src/Scene/Scene.cpp
        src/Scene/Managers/MeshManager.cpp
        src/Scene/Managers/MaterialManager.cpp
        src/Scene/Text/TextObject.h
        src/Scene/Text/TextObject.cpp
        src/Physics/Collisions/CollisionSolver.cpp
        src/Physics/Collisions/CollisionSolver.h
        src/Physics/Collisions/CollisionSolver.h
        src/Physics/Collisions/SpatialGrid.cpp
        src/Physics/Collisions/SpatialGrid.h
        src/UI/UIManager.cpp
        src/UI/UIManager.h
        src/UI/UIManager.h
)

target_include_directories(grafyte_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
target_compile_features(grafyte_core PUBLIC cxx_std_20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(grafyte_core PUBLIC
        glfw
        glad::glad
        Freetype::Freetype
        OpenGL::GL
)

# ==================================================
# [SECTION] Python module
# ==================================================

pybind11_add_module(${MODNAME} MODULE bindings/bindings.cpp)
target_link_libraries(${MODNAME} PRIVATE grafyte_core)

target_compile_definitions(${MODNAME} PRIVATE
        GRAFYTE_PY_MODULE_NAME=${MODNAME}
)

set_target_properties(${MODNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# The output of the python extension inside the build dir
set_target_properties(${MODNAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
)

# Detecting OS for python extension file extension (.so/.pyd)
if (WIN32)
    set(PY_EXT "pyd")
    set(GRAFYTE_INTERNAL_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/built-win"
            CACHE PATH "Destination folder for built modules"
    )
else ()
    set(PY_EXT "so")
    set(GRAFYTE_INTERNAL_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/built-linux"
            CACHE PATH "Destination folder for built modules"
    )
endif ()

message(STATUS "Will copy .${PY_EXT} to: ${GRAFYTE_INTERNAL_DIR}")

# Copy & rename
add_custom_command(TARGET ${MODNAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GRAFYTE_INTERNAL_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${MODNAME}>"
        "${GRAFYTE_INTERNAL_DIR}/${MODNAME}.${PY_EXT}"
        COMMENT "Copy + rename to ${GRAFYTE_INTERNAL_DIR}/${MODNAME}.${PY_EXT}"
)

# ==================================================
# [SECTION] Tests
# ==================================================

include(CTest)
enable_testing()

find_package(doctest CONFIG REQUIRED)

add_executable(grafyte_tests
        tests/test_main.cpp
        tests/test_camera.cpp
        tests/test_collision_solver.cpp
        tests/test_spatial_grid.cpp
        tests/test_types.cpp
)

target_link_libraries(grafyte_tests PRIVATE
        grafyte_core
        doctest::doctest
)

target_include_directories(grafyte_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(NAME grafyte_tests COMMAND grafyte_tests)